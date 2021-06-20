/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include "WidgetDoc.h"
#include "ui_WidgetDoc.h"

#include <QFileDialog>

#include <Languages.h>

#include <gui/DialogEditObject.h>

////////////////////////////////////////////////////////////////////////////////

WidgetDoc::WidgetDoc( QWidget *parent ) :
    QWidget( parent ),
    m_ui( new Ui::WidgetDoc ),

    m_currentObjectIndex ( -1 ),
    m_currentRouteIndex  ( -1 ),
    m_currentUnitIndex   ( -1 ),
    m_currentStageIndex  ( -1 )
{
    m_ui->setupUi( this );

    for ( int i = 0; i < Languages::instance()->getCount(); i++ )
    {
        m_ui->comboBoxLang->addItem( Languages::instance()->getCodeByIndex( i ) );
    }

    m_ui->comboBoxLang->setCurrentIndex( 0 );

    m_ui->labelIcon       ->hide();
    m_ui->labelIconLocked ->hide();

    m_ui->lineEditIcon       ->hide();
    m_ui->lineEditIconLocked ->hide();
}

////////////////////////////////////////////////////////////////////////////////

WidgetDoc::~WidgetDoc()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::newEmpty()
{
    Mission::getInstance()->newEmpty();

    m_ui->stackedWidget->setCurrentIndex( 0 );

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetDoc::readFile( QString file )
{
    bool result = Mission::getInstance()->readFile( file );

    if ( result )
    {
        updateGUI();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetDoc::saveFile( QString file )
{
    return Mission::getInstance()->saveFile( file );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::editObject()
{
    if ( m_currentObjectIndex >= 0 && m_currentObjectIndex < (int)Mission::getInstance()->getObjects().size() )
    {
        QString objectFile = Mission::getInstance()->getObjects().at( m_currentObjectIndex ).c_str();

        objectFile = DialogEditObject::getObjectFile( objectFile, this );

        Mission::getInstance()->getObjects().at( m_currentObjectIndex ) = objectFile.toStdString();

        updateGUI();

        emit( sceneryReloaded() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::editRoute()
{
    if ( m_currentRouteIndex >= 0 && m_currentRouteIndex < (int)Mission::getInstance()->getRoutes().size() )
    {
        m_ui->stackedWidget->setCurrentIndex( 1 );
        m_ui->pageRoute->edit( m_currentRouteIndex );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::editStage()
{
    if ( m_currentStageIndex >= 0 && m_currentStageIndex < (int)Mission::getInstance()->getStages().size() )
    {
        m_ui->stackedWidget->setCurrentIndex( 3 );
        m_ui->pageStage->edit( m_currentStageIndex );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::editUnit()
{
    if ( m_currentUnitIndex >= 0 && m_currentUnitIndex < (int)Mission::getInstance()->getUnits().size() )
    {
        m_ui->stackedWidget->setCurrentIndex( 2 );
        m_ui->pageUnit->edit( m_currentUnitIndex );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::updateGUI()
{
    m_ui->checkBoxTutorial->setChecked( Mission::getInstance()->getTutorial() );

    m_ui->listWidgetObjects->clear();
    m_ui->listWidgetRoutes->clear();
    m_ui->listWidgetUnits->clear();
    m_ui->listWidgetStages->clear();

    m_ui->lineEditIcon->setText( Mission::getInstance()->getMissionIcon().c_str() );
    m_ui->lineEditIconLocked->setText( Mission::getInstance()->getMissionIconLocked().c_str() );
    m_ui->lineEditImage->setText( Mission::getInstance()->getMissionImage().c_str() );
    m_ui->lineEditImageLocked->setText( Mission::getInstance()->getMissionImageLocked().c_str() );
    m_ui->lineEditName->setText( Mission::getInstance()->getMissionName().get().c_str() );
    m_ui->textEditIntroduction->setPlainText( Mission::getInstance()->getMissionIntroduction().get().c_str() );
    m_ui->textEditSummaryFailure->setPlainText( Mission::getInstance()->getMissionSummaryFailure().get().c_str() );
    m_ui->textEditSummarySuccess->setPlainText( Mission::getInstance()->getMissionSummarySuccess().get().c_str() );

    m_ui->lineEditTerrain->setText( Mission::getInstance()->getTerrainFile().c_str() );
    m_ui->lineEditGeneric->setText( Mission::getInstance()->getGenericFile().c_str() );
    m_ui->lineEditElevation->setText( Mission::getInstance()->getElevationFile().c_str() );
    m_ui->lineEditSkyDome->setText( Mission::getInstance()->getSkyDomeFile().c_str() );
    m_ui->spinBoxVisibility->setValue( Mission::getInstance()->getVisibility() );
    m_ui->spinBoxSunCoef->setValue( Mission::getInstance()->getSunCoef() );

    for ( int i = 0; i < (int)Mission::getInstance()->getObjects().size(); i++ )
        {
            QString objectFile = "Object #" + QString::number( i ) + ": ";

            objectFile += Mission::getInstance()->getObjects().at( i ).c_str();

            m_ui->listWidgetObjects->addItem( new QListWidgetItem( objectFile, m_ui->listWidgetObjects ) );
        }

    for ( int i = 0; i < (int)Mission::getInstance()->getRoutes().size(); i++ )
    {
        Route *route = Mission::getInstance()->getRoutes().at( i );

        QString name = "Route #" + QString::number( i ) + ": ";
        name += route->name.c_str();

        m_ui->listWidgetRoutes->addItem( new QListWidgetItem( name, m_ui->listWidgetRoutes ) );
    }

    for ( int i = 0; i < (int)Mission::getInstance()->getUnits().size(); i++ )
    {
        Unit *unit = Mission::getInstance()->getUnits().at( i );

        QString name = "Unit #" + QString::number( i ) + ": ";
        name += QString( Unit::typeToName( unit->getType() ).c_str() ).toUpper();
        name += " - \"" + QString( unit->getName().c_str() ) + "\"";

        switch ( unit->getAffiliation() )
        {
            case Unit::Friend:  name += " - Friend"; break;
            case Unit::Hostile: name += " - Hostile"; break;
            case Unit::Neutral: name += " - Neutral"; break;
        }

        if ( unit->isOwnship() )
        {
            name += " (ownship)";
        }

        m_ui->listWidgetUnits->addItem( new QListWidgetItem( name, m_ui->listWidgetUnits ) );
    }

    for ( int i = 0; i < (int)Mission::getInstance()->getStages().size(); i++ )
    {
        //Stage *stage = Mission::getInstance()->getStages().at( i );

        QString name = "Stage #" + QString::number( i ) + ": ";
        name += Mission::getInstance()->getStages().at( i )->getName().c_str();

        m_ui->listWidgetStages->addItem( new QListWidgetItem( name, m_ui->listWidgetStages ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pageRoute_changed()
{
    updateGUI();
    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pageUnit_changed()
{
    updateGUI();
    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pageStage_changed()
{
    updateGUI();
    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pageRoute_done()
{
    m_ui->stackedWidget->setCurrentIndex( 0 );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pageUnit_done()
{
    m_ui->stackedWidget->setCurrentIndex( 0 );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pageStage_done()
{
    m_ui->stackedWidget->setCurrentIndex( 0 );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_comboBoxLang_currentIndexChanged(int index)
{
    Languages::instance()->setCurrent( index );

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_toolBox_currentChanged(int /*index*/)
{
    SceneRoot::getInstance()->hideAll();

    m_ui->listWidgetRoutes->setCurrentRow( -1 );
    m_ui->listWidgetUnits->setCurrentRow( -1 );
    m_ui->listWidgetStages->setCurrentRow( -1 );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_checkBoxTutorial_toggled(bool checked)
{
    Mission::getInstance()->setTutorial( checked );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditIcon_textChanged(const QString &arg1)
{
    Mission::getInstance()->setMissionIcon( arg1.toStdString() );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditIconLocked_textChanged(const QString &arg1)
{
    Mission::getInstance()->setMissionIconLocked( arg1.toStdString() );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditImage_textChanged(const QString &arg1)
{
    Mission::getInstance()->setMissionImage( arg1.toStdString() );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditImageLocked_textChanged(const QString &arg1)
{
    Mission::getInstance()->setMissionImageLocked( arg1.toStdString() );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditName_textChanged(const QString &arg1)
{
    Text text = Mission::getInstance()->getMissionName();

    text.set( arg1 );

    Mission::getInstance()->setMissionName( text );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_textEditIntroduction_textChanged()
{
    Text text = Mission::getInstance()->getMissionIntroduction();

    text.set( m_ui->textEditIntroduction->toPlainText() );

    Mission::getInstance()->setMissionIntroduction( text );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_textEditSummaryFailure_textChanged()
{
    Text text = Mission::getInstance()->getMissionSummaryFailure();

    text.set( m_ui->textEditSummaryFailure->toPlainText() );

    Mission::getInstance()->setMissionSummaryFailure( text );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_textEditSummarySuccess_textChanged()
{
    Text text = Mission::getInstance()->getMissionSummarySuccess();

    text.set( m_ui->textEditSummarySuccess->toPlainText() );

    Mission::getInstance()->setMissionSummarySuccess( text );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditTerrain_textChanged(const QString &arg1)
{
    Mission::getInstance()->setTerrainFile( arg1.toStdString() );

    SceneRoot::getInstance()->reloadScenery();

    emit( sceneryReloaded() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditGeneric_textChanged(const QString &arg1)
{
    Mission::getInstance()->setGenericFile( arg1.toStdString() );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditElevation_textChanged(const QString &arg1)
{
    Mission::getInstance()->setElevationFile( arg1.toStdString() );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_lineEditSkyDome_textChanged(const QString &arg1)
{
    Mission::getInstance()->setSkyDomeFile( arg1.toStdString() );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonTerrain_clicked()
{
    QString file = "data/" + m_ui->lineEditTerrain->text();

    QString caption = "Browse";
    QString dir = ( file.length() > 0 ) ? QFileInfo( file ).path() : ".";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "OpenSceneGraph (*.osg *.osga *.osgb *.osgt *.ive)";

    QString newFile = QFileDialog::getOpenFileName( this, caption, dir, filter, &selectedFilter );
    QString workDir = QDir::currentPath();

    // relative path
    if ( 1 )
    {
        int n = newFile.length() - workDir.length() - 1;
        newFile = newFile.right( n );
        newFile = newFile.right( newFile.length() - 5 );
    }

    if ( newFile.length() > 0 )
    {
        m_ui->lineEditTerrain->setText( newFile );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonGeneric_clicked()
{
    QString file = "data/" + m_ui->lineEditGeneric->text();

    QString caption = "Browse";
    QString dir = ( file.length() > 0 ) ? QFileInfo( file ).path() : ".";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "OpenSceneGraph (*.osg *.osga *.osgb *.osgt *.ive)";

    QString newFile = QFileDialog::getOpenFileName( this, caption, dir, filter, &selectedFilter );
    QString workDir = QDir::currentPath();

    // relative path
    if ( 1 )
    {
        int n = newFile.length() - workDir.length() - 1;
        newFile = newFile.right( n );
        newFile = newFile.right( newFile.length() - 5 );
    }

    if ( newFile.length() > 0 )
    {
        m_ui->lineEditGeneric->setText( newFile );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonElevation_clicked()
{
    QString file = "data/" + m_ui->lineEditElevation->text();

    QString caption = "Browse";
    QString dir = ( file.length() > 0 ) ? QFileInfo( file ).path() : ".";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "Comma-Separated Values (*.csv)";

    QString newFile = QFileDialog::getOpenFileName( this, caption, dir, filter, &selectedFilter );
    QString workDir = QDir::currentPath();

    // relative path
    if ( 1 )
    {
        int n = newFile.length() - workDir.length() - 1;
        newFile = newFile.right( n );
        newFile = newFile.right( newFile.length() - 5 );
    }

    if ( newFile.length() > 0 )
    {
        m_ui->lineEditElevation->setText( newFile );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonSkyDome_clicked()
{
    QString file = "data/" + m_ui->lineEditSkyDome->text();

    QString caption = "Browse";
    QString dir = ( file.length() > 0 ) ? QFileInfo( file ).path() : ".";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "Red-Green-Blue (*.rgb)";

    QString newFile = QFileDialog::getOpenFileName( this, caption, dir, filter, &selectedFilter );
    QString workDir = QDir::currentPath();

    // relative path
    if ( 1 )
    {
        int n = newFile.length() - workDir.length() - 1;
        newFile = newFile.right( n );
        newFile = newFile.right( newFile.length() - 5 );
    }

    if ( newFile.length() > 0 )
    {
        m_ui->lineEditSkyDome->setText( newFile );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_spinBoxVisibility_valueChanged(int arg1)
{
    Mission::getInstance()->setVisibility( arg1 );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_spinBoxSunCoef_valueChanged(double arg1)
{
    Mission::getInstance()->setSunCoef( arg1 );

    emit( changed() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonObjectAdd_clicked()
{
    Mission::getInstance()->getObjects().push_back( "" );

    updateGUI();

    emit( sceneryReloaded() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonObjectEdit_clicked()
{
    editObject();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonObjectRemove_clicked()
{
    if ( m_currentObjectIndex >= 0 && m_currentObjectIndex < (int)Mission::getInstance()->getObjects().size() )
    {
        Mission::ObjectFiles::iterator it = Mission::getInstance()->getObjects().begin() + m_currentObjectIndex;

        m_ui->listWidgetObjects->setCurrentRow( -1 );

        Mission::getInstance()->getObjects().erase( it );

        updateGUI();

        emit( sceneryReloaded() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetObjects_currentRowChanged(int currentRow)
{
    if ( currentRow >= 0 && currentRow < (int)Mission::getInstance()->getObjects().size() )
    {
        m_ui->pushButtonObjectEdit->setEnabled( true );
        m_ui->pushButtonObjectRemove->setEnabled( true );

        m_currentObjectIndex = currentRow;
    }
    else
    {
        m_ui->pushButtonObjectEdit->setEnabled( false );
        m_ui->pushButtonObjectRemove->setEnabled( false );

        m_currentObjectIndex = -1;
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetObjects_doubleClicked(const QModelIndex &)
{
    editObject();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonRouteAdd_clicked()
{
    Mission::getInstance()->getRoutes().push_back( new Route() );

    SceneRoot::getInstance()->reloadRoutes();

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonRouteEdit_clicked()
{
    editRoute();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonRouteRemove_clicked()
{
    if ( m_currentRouteIndex >= 0 && m_currentRouteIndex < (int)Mission::getInstance()->getRoutes().size() )
    {
        Mission::Routes::iterator it = Mission::getInstance()->getRoutes().begin() + m_currentRouteIndex;

        m_ui->listWidgetRoutes->setCurrentRow( -1 );

        Unit *unit = Mission::getInstance()->getOwnship();

        if ( unit )
        {
            if ( QString( (*it)->name.c_str() ) == QString( unit->getRoute().c_str() ) )
            {
                (*it)->removeWaypointObjectives();
            }
        }

        (*it)->removeFromAllUnits();

        if ( *it ) delete (*it);
        (*it) = 0;

        Mission::getInstance()->getRoutes().erase( it );

        SceneRoot::getInstance()->reloadRoutes();

        updateGUI();

        emit( changed() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetRoutes_currentRowChanged(int currentRow)
{
    if ( currentRow >= 0 && currentRow < (int)Mission::getInstance()->getRoutes().size() )
    {
        m_ui->pushButtonRouteEdit->setEnabled( true );
        m_ui->pushButtonRouteRemove->setEnabled( true );

        m_currentRouteIndex = currentRow;

        SceneRoot::getInstance()->showRoute( m_currentRouteIndex );
    }
    else
    {
        m_ui->pushButtonRouteEdit->setEnabled( false );
        m_ui->pushButtonRouteRemove->setEnabled( false );

        m_currentRouteIndex = -1;

        SceneRoot::getInstance()->hideRoute();
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetRoutes_doubleClicked(const QModelIndex &)
{
    editRoute();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonUnitAdd_clicked()
{
    Mission::getInstance()->getUnits().push_back( new Unit() );

    emit( changed() );

    updateGUI();
    SceneRoot::getInstance()->reloadUnits();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonUnitEdit_clicked()
{
    editUnit();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonUnitRemove_clicked()
{
    if ( m_currentUnitIndex >= 0 && m_currentUnitIndex < (int)Mission::getInstance()->getUnits().size() )
    {
        Mission::Units::iterator it = Mission::getInstance()->getUnits().begin() + m_currentUnitIndex;

        m_ui->listWidgetUnits->setCurrentRow( -1 );

        if ( *it ) delete (*it);
        (*it) = 0;

        Mission::getInstance()->getUnits().erase( it );

        updateGUI();
        SceneRoot::getInstance()->reloadUnits();

        emit( changed() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetUnits_currentRowChanged(int currentRow)
{
    if ( currentRow >= 0 && currentRow < (int)Mission::getInstance()->getUnits().size() )
    {
        m_ui->pushButtonUnitEdit->setEnabled( true );
        m_ui->pushButtonUnitRemove->setEnabled( true );

        m_currentUnitIndex = currentRow;

        SceneRoot::getInstance()->showUnit( m_currentUnitIndex );
    }
    else
    {
        m_ui->pushButtonUnitEdit->setEnabled( false );
        m_ui->pushButtonUnitRemove->setEnabled( false );

        m_currentUnitIndex = -1;

        SceneRoot::getInstance()->hideUnit();
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetUnits_doubleClicked( const QModelIndex & /*index*/ )
{
    editUnit();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonStageAdd_clicked()
{
    int tempIndex = m_currentStageIndex + 1;

    if ( tempIndex >= 0 && tempIndex < (int)Mission::getInstance()->getStages().size() )
    {
        Mission::getInstance()->getStages().insert( Mission::getInstance()->getStages().begin() + tempIndex, new Stage() );
    }
    else
    {
        Mission::getInstance()->getStages().push_back( new Stage() );
    }

    emit( changed() );

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonStageEdit_clicked()
{
    editStage();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonStageRemove_clicked()
{
    if ( m_currentStageIndex >= 0 && m_currentStageIndex < (int)Mission::getInstance()->getStages().size() )
    {
        Mission::Stages::iterator it = Mission::getInstance()->getStages().begin() + m_currentStageIndex;

        m_ui->listWidgetStages->setCurrentRow( -1 );

        if ( *it ) delete (*it);
        (*it) = 0;

        Mission::getInstance()->getStages().erase( it );

        updateGUI();

        emit( changed() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetStages_currentRowChanged(int currentRow)
{
    if ( currentRow >= 0 && currentRow < (int)Mission::getInstance()->getStages().size() )
    {
        m_ui->pushButtonStageEdit->setEnabled( true );
        m_ui->pushButtonStageRemove->setEnabled( true );

        m_currentStageIndex = currentRow;
    }
    else
    {
        m_ui->pushButtonStageEdit->setEnabled( false );
        m_ui->pushButtonStageRemove->setEnabled( false );

        m_currentStageIndex = -1;
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_listWidgetStages_doubleClicked(const QModelIndex &)
{
    editStage();
}
