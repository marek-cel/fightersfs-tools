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

#include <iostream>

#include <QFileDialog>

#include "WidgetEditUnit.h"
#include "ui_WidgetEditUnit.h"

#include "Converter.h"

////////////////////////////////////////////////////////////////////////////////

WidgetEditUnit::WidgetEditUnit( QWidget *parent ) :
    QWidget ( parent ),
    m_ui( new Ui::WidgetEditUnit ),

    m_unitIndex ( -1 ),
    m_unit ( 0 )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

WidgetEditUnit::~WidgetEditUnit()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::edit( int unitIndex )
{
    m_inited = false;

    m_ui->comboBoxLeader->clear();
    m_ui->comboBoxUnitRoute->clear();

    for ( int i = 0; i < (int)Mission::getInstance()->getUnits().size(); i++ )
    {
        m_ui->comboBoxLeader->addItem( Mission::getInstance()->getUnits().at( i )->getName().c_str() );
    }

    m_ui->comboBoxLeader->setCurrentIndex( -1 );

    if ( unitIndex >= 0 && unitIndex < (int)Mission::getInstance()->getUnits().size() )
    {
        m_unitIndex = unitIndex;
        m_unit = Mission::getInstance()->getUnits()[ m_unitIndex ];

        for ( int i = 0; i < (int)Mission::getInstance()->getRoutes().size(); i++ )
        {
            m_ui->comboBoxUnitRoute->addItem( Mission::getInstance()->getRoutes()[ i ]->name.c_str() );
        }
    }
    else
    {
        m_unitIndex = -1;
        m_unit = 0;
    }

    updateGUI();

    SceneRoot::getInstance()->showUnit( m_unitIndex );

    m_inited = true;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::updateGUI()
{
    if ( m_unit )
    {
        if ( m_unit->isOwnship() )
        {
            m_ui->labelUnitName->setEnabled( false );
            m_ui->lineEditUnitName->setEnabled( false );

            m_ui->lineEditUnitName->setText( "ownship" );

            m_ui->labelUnitAffiliation->setEnabled( false );
            m_ui->radioButtonFriend->setEnabled( false );
            m_ui->radioButtonHostile->setEnabled( false );
            m_ui->radioButtonNeutral->setEnabled( false );

            m_ui->radioButtonFriend->setChecked( true );

            m_ui->checkBoxUnique->setEnabled( true );
            m_ui->checkBoxUnique->setChecked( true );
        }
        else
        {
            m_ui->labelUnitName->setEnabled( true );
            m_ui->lineEditUnitName->setEnabled( true );

            m_ui->lineEditUnitName->setText( m_unit->getName().c_str() );

            m_ui->labelUnitAffiliation->setEnabled( true );
            m_ui->radioButtonFriend->setEnabled( true );
            m_ui->radioButtonHostile->setEnabled( true );
            m_ui->radioButtonNeutral->setEnabled( true );

            if ( m_unit->getAffiliation() == Unit::Friend  ) m_ui->radioButtonFriend->setChecked( true );
            if ( m_unit->getAffiliation() == Unit::Hostile ) m_ui->radioButtonHostile->setChecked( true );
            if ( m_unit->getAffiliation() == Unit::Neutral ) m_ui->radioButtonNeutral->setChecked( true );

            m_ui->checkBoxUnique->setEnabled( false );
            m_ui->checkBoxUnique->setChecked( false );
        }

        int typeIndex = m_unit->getType();

        if ( typeIndex >= m_ui->comboBoxUnitType->count() ) typeIndex = -1;

        m_ui->comboBoxUnitType->setCurrentIndex( typeIndex );

        m_ui->lineEditUnitFile->setText( m_unit->getFile().c_str() );

        m_ui->spinBoxUnitHP->setValue( m_unit->getHP() );

        if ( Unit::isAircraft( m_unit->getType() ) )
        {
            m_ui->labelUnitLivery->setEnabled( true );
            m_ui->lineEditUnitLivery->setEnabled( true );
            m_ui->pushButtonUnitLivery->setEnabled( true );

            m_ui->lineEditUnitLivery->setText( m_unit->getLivery().c_str() );

            m_ui->checkBoxUnique->setEnabled( true );
            m_ui->checkBoxUnique->setChecked( m_unit->getUnique() );

            m_ui->checkBoxWingman->setEnabled( true );
            m_ui->checkBoxWingman->setChecked( m_unit->getWingman() );

            if ( m_unit->getWingman() )
            {
                int leaderIndex = Mission::getInstance()->getUnitIndex( m_unit->getLeader() );

                if ( leaderIndex >= 0 )
                {
                    m_ui->labelLeader->setEnabled( true );
                    m_ui->labelOffsetX->setEnabled( true );
                    m_ui->labelOffsetY->setEnabled( true );
                    m_ui->labelOffsetZ->setEnabled( true );

                    m_ui->comboBoxLeader->setEnabled( true );

                    m_ui->comboBoxLeader->setCurrentIndex( leaderIndex );

                    m_ui->spinBoxLeaderX->setEnabled( true );
                    m_ui->spinBoxLeaderY->setEnabled( true );
                    m_ui->spinBoxLeaderZ->setEnabled( true );

                    m_ui->spinBoxLeaderX->setValue( m_unit->getOffset().x() );
                    m_ui->spinBoxLeaderY->setValue( m_unit->getOffset().y() );
                    m_ui->spinBoxLeaderZ->setValue( m_unit->getOffset().z() );
                }
            }
            else
            {
                m_ui->labelLeader->setEnabled( false );
                m_ui->labelOffsetX->setEnabled( false );
                m_ui->labelOffsetY->setEnabled( false );
                m_ui->labelOffsetZ->setEnabled( false );

                m_ui->comboBoxLeader->setEnabled( false );

                m_ui->spinBoxLeaderX->setEnabled( false );
                m_ui->spinBoxLeaderY->setEnabled( false );
                m_ui->spinBoxLeaderZ->setEnabled( false );

                m_ui->spinBoxLeaderX->setValue( 0.0 );
                m_ui->spinBoxLeaderY->setValue( 0.0 );
                m_ui->spinBoxLeaderZ->setValue( 0.0 );
            }

            m_ui->checkBoxUnitRoute->setEnabled( true );

            int routeIndex = -1;

            if ( m_unit->getRoute().length() > 0 )
            {
                m_ui->checkBoxUnitRoute->setChecked( true );

                m_ui->labelUnitRoute->setEnabled( true );
                m_ui->comboBoxUnitRoute->setEnabled( true );

                routeIndex = Mission::getInstance()->getRouteIndex( m_unit->getRoute() );
            }
            else
            {
                m_ui->checkBoxUnitRoute->setChecked( false );

                m_ui->labelUnitRoute->setEnabled( false );
                m_ui->comboBoxUnitRoute->setEnabled( false );
            }

            if ( routeIndex >= 0 && routeIndex < (int)Mission::getInstance()->getRoutes().size() )
            {
                m_ui->comboBoxUnitRoute->setCurrentIndex( routeIndex );
            }
            else
            {
                m_ui->comboBoxUnitRoute->setCurrentIndex( -1 );
            }
        }
        else
        {
            m_ui->checkBoxWingman->setEnabled( false );
            m_ui->checkBoxWingman->setChecked( false );

            m_ui->comboBoxUnitRoute->setCurrentIndex( -1 );

            m_ui->labelUnitLivery->setEnabled( false);
            m_ui->lineEditUnitLivery->setEnabled( false );
            m_ui->pushButtonUnitLivery->setEnabled( false );

            m_ui->lineEditUnitLivery->setText( "" );

            m_ui->checkBoxWingman->setEnabled( false );
            m_ui->checkBoxWingman->setChecked( false );

            m_ui->labelLeader->setEnabled( false );
            m_ui->labelOffsetX->setEnabled( false );
            m_ui->labelOffsetY->setEnabled( false );
            m_ui->labelOffsetZ->setEnabled( false );

            m_ui->comboBoxLeader->setEnabled( false );

            m_ui->spinBoxLeaderX->setEnabled( false );
            m_ui->spinBoxLeaderY->setEnabled( false );
            m_ui->spinBoxLeaderZ->setEnabled( false );

            m_ui->checkBoxUnitRoute->setChecked( false );
            m_ui->checkBoxUnitRoute->setEnabled( false );
            m_ui->labelUnitRoute->setEnabled( false );
            m_ui->comboBoxUnitRoute->setEnabled( false );
        }

        m_ui->spinBoxUnitPositionX->setValue( m_unit->getPosition().x() );
        m_ui->spinBoxUnitPositionY->setValue( m_unit->getPosition().y() );
        m_ui->spinBoxUnitPositionZ->setValue( m_unit->getPosition().z() );

        m_ui->spinBoxUnitSpeed->setValue( m_unit->getVelocity() );
        m_ui->spinBoxUnitHeading->setValue( m_unit->getHeading() );

        if ( m_unit->isOwnship() )
        {
            m_ui->checkBoxOwnship->setChecked( true );
        }
        else
        {
            m_ui->checkBoxOwnship->setChecked( false );
        }

        SceneRoot::getInstance()->showUnit( m_unitIndex );
    }
    else
    {
        m_ui->lineEditUnitName->setText( "" );

        m_ui->labelUnitAffiliation->setEnabled( false );
        m_ui->radioButtonFriend->setEnabled( false );
        m_ui->radioButtonHostile->setEnabled( false );
        m_ui->radioButtonNeutral->setEnabled( false );

        m_ui->radioButtonFriend->setChecked( true );

        m_ui->comboBoxUnitType->setCurrentIndex( 0 );

        m_ui->labelUnitLivery->setEnabled( false);
        m_ui->lineEditUnitLivery->setEnabled( false );
        m_ui->pushButtonUnitLivery->setEnabled( false );

        m_ui->lineEditUnitLivery->setText( "" );

        m_ui->checkBoxWingman->setEnabled( false );
        m_ui->checkBoxWingman->setChecked( false );

        m_ui->labelLeader->setEnabled( false );
        m_ui->labelOffsetX->setEnabled( false );
        m_ui->labelOffsetY->setEnabled( false );
        m_ui->labelOffsetZ->setEnabled( false );

        m_ui->comboBoxLeader->setEnabled( false );

        m_ui->spinBoxLeaderX->setEnabled( false );
        m_ui->spinBoxLeaderY->setEnabled( false );
        m_ui->spinBoxLeaderZ->setEnabled( false );

        m_ui->checkBoxUnique->setChecked( false );
        m_ui->checkBoxUnique->setEnabled( false );

        m_ui->checkBoxUnitRoute->setChecked( false );
        m_ui->checkBoxUnitRoute->setEnabled( false );

        m_ui->comboBoxUnitRoute->setCurrentIndex( -1 );

        m_ui->spinBoxUnitPositionX->setValue( 0.0 );
        m_ui->spinBoxUnitPositionY->setValue( 0.0 );
        m_ui->spinBoxUnitPositionZ->setValue( 0.0 );

        m_ui->spinBoxUnitSpeed->setValue( 0.0 );
        m_ui->spinBoxUnitHeading->setValue( 0.0 );

        m_ui->checkBoxOwnship->setChecked( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_lineEditUnitName_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            m_unit->setName( m_ui->lineEditUnitName->text().toStdString() );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_radioButtonFriend_toggled(bool checked)
{
    if ( m_inited )
    {
        if ( m_unit && checked )
        {
            m_unit->setAffiliation( Unit::Friend );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_radioButtonHostile_toggled(bool checked)
{
    if ( m_inited )
    {
        if ( m_unit && checked )
        {
            m_unit->setAffiliation( Unit::Hostile );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_radioButtonNeutral_toggled(bool checked)
{
    if ( m_inited )
    {
        if ( m_unit && checked )
        {
            m_unit->setAffiliation( Unit::Neutral );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_comboBoxUnitType_currentIndexChanged(int index)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            Unit::Type type = Unit::Unknown;

            switch ( index )
            {
                case  1: type = Unit::Aircraft;      break;
                case  2: type = Unit::Balloon;       break;
                case  3: type = Unit::BomberDive;    break;
                case  4: type = Unit::BomberLevel;   break;
                case  5: type = Unit::BomberTorpedo; break;
                case  6: type = Unit::Building;      break;
                case  7: type = Unit::Fighter;       break;
                case  8: type = Unit::Kamikaze;      break;
                case  9: type = Unit::Warship;       break;
            }

#           warning UNIT_TYPE

            if ( type != m_unit->getType() )
            {
                m_unit->setType( type );

                emit( changed() );
                updateGUI();
                SceneRoot::getInstance()->reloadUnits();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_lineEditUnitFile_textChanged(const QString &arg1)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            m_unit->setFile( arg1.toStdString() );
            emit( changed() );
            updateGUI();
            SceneRoot::getInstance()->reloadUnits();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_pushButtonUnitFile_clicked()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            QString file = "data/" + m_ui->lineEditUnitFile->text();

            QString caption = "Browse";
            QString dir = ( file.length() > 0 ) ? QFileInfo( file ).path() : ".";
            QString filter;
            QString selectedFilter;

            filter += selectedFilter = "XML (*.xml)";

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
                m_ui->lineEditUnitFile->setText( newFile );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitHP_valueChanged(int arg1)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            m_unit->setHP( arg1 );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_lineEditUnitLivery_textChanged(const QString &arg1)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            m_unit->setLivery( arg1.toStdString() );

            emit( changed() );
            updateGUI();
            SceneRoot::getInstance()->reloadUnits();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_pushButtonUnitLivery_clicked()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            QString file = "data/" + m_ui->lineEditUnitLivery->text();

            QString caption = "Browse";
            QString dir = ( file.length() > 0 ) ? QFileInfo( file ).path() : ".";
            QString filter;
            QString selectedFilter;

            filter += selectedFilter = "RGB (*.rgb)";

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
                m_ui->lineEditUnitLivery->setText( newFile );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_checkBoxUnique_toggled(bool checked)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            m_unit->setUnique( checked );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_checkBoxWingman_toggled(bool checked)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            if ( checked )
            {
                m_ui->labelLeader->setEnabled( true );
                m_ui->labelOffsetX->setEnabled( true );
                m_ui->labelOffsetY->setEnabled( true );
                m_ui->labelOffsetZ->setEnabled( true );

                m_ui->comboBoxLeader->setEnabled( true );

                m_unit->setWingman( true );

                int leaderIndex = Mission::getInstance()->getUnitIndex( m_unit->getLeader() );

                if ( leaderIndex < 0 )
                {
                    if ( m_ui->comboBoxLeader->count() > 0 )
                    {
                        m_ui->comboBoxLeader->setCurrentIndex( 0 );
                    }
                    else
                    {
                        m_ui->comboBoxLeader->setCurrentIndex( -1 );
                    }
                }
                else
                {
                    m_ui->comboBoxLeader->setCurrentIndex( leaderIndex );
                }

                m_ui->spinBoxLeaderX->setEnabled( true );
                m_ui->spinBoxLeaderY->setEnabled( true );
                m_ui->spinBoxLeaderZ->setEnabled( true );
            }
            else
            {
                m_ui->labelLeader->setEnabled( false );
                m_ui->labelOffsetX->setEnabled( false );
                m_ui->labelOffsetY->setEnabled( false );
                m_ui->labelOffsetZ->setEnabled( false );

                m_ui->comboBoxLeader->setEnabled( false );

                m_ui->comboBoxLeader->setCurrentIndex( -1 );

                m_unit->setWingman( false );
                m_unit->setLeader( "" );

                m_ui->spinBoxLeaderX->setEnabled( false );
                m_ui->spinBoxLeaderY->setEnabled( false );
                m_ui->spinBoxLeaderZ->setEnabled( false );
            }

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_comboBoxLeader_currentIndexChanged(int index)
{
    if ( m_inited )
    {
        if ( m_unit && m_ui->checkBoxWingman->isChecked() )
        {
            if ( index >=0 && index < (int)Mission::getInstance()->getUnits().size() )
            {
                m_unit->setLeader( Mission::getInstance()->getUnitName( index ) );
            }

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxLeaderX_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            osg::Vec3d offset = m_unit->getOffset();

            offset.x() = m_ui->spinBoxLeaderX->value();

            m_unit->setOffset( offset );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxLeaderY_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            osg::Vec3d offset = m_unit->getOffset();

            offset.y() = m_ui->spinBoxLeaderY->value();

            m_unit->setOffset( offset );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxLeaderZ_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            osg::Vec3d offset = m_unit->getOffset();

            offset.z() = m_ui->spinBoxLeaderZ->value();

            m_unit->setOffset( offset );

            emit( changed() );
            updateGUI();
        }
    }
}


////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_checkBoxUnitRoute_toggled(bool checked)
{
    if ( m_inited )
    {
        if ( checked )
        {
            m_ui->labelUnitRoute->setEnabled( true );
            m_ui->comboBoxUnitRoute->setEnabled( true );

            m_ui->comboBoxUnitRoute->setCurrentIndex( 0 );

            if ( m_unit )
            {
                std::string routeName = Mission::getInstance()->getRouteName( m_ui->comboBoxUnitRoute->currentIndex() );
                m_unit->setRoute( routeName );

                emit( changed() );
            }
        }
        else
        {
            m_ui->labelUnitRoute->setEnabled( false );
            m_ui->comboBoxUnitRoute->setEnabled( false );

            m_ui->comboBoxUnitRoute->setCurrentIndex( -1 );

            if ( m_unit )
            {
                m_unit->setRoute( "" );

                emit( changed() );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_comboBoxUnitRoute_currentIndexChanged(int index)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            std::string routeName = Mission::getInstance()->getRouteName( index );

            if ( routeName.length() >  0 )
            {
                if ( m_unit->isOwnship()
                  && QString( m_unit->getRoute().c_str() ) != QString( routeName.c_str() ) )
                {
                    Route *route = Mission::getInstance()->getRouteByName( routeName );

                    if ( route )
                    {
                        //route->removeWaypointObjectives(); // ???????????????????
                        std::cout << "route->removeWaypointObjectives()" << std::endl;
                    }
                }

                m_unit->setRoute( routeName );

                emit( changed() );
                updateGUI();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitPositionX_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            osg::Vec3d position = m_unit->getPosition();

            position.x() = m_ui->spinBoxUnitPositionX->value();

            m_unit->setPosition( position );

            emit( changed() );
            updateGUI();
            SceneRoot::getInstance()->reloadUnits();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitPositionY_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            osg::Vec3d position = m_unit->getPosition();

            position.y() = m_ui->spinBoxUnitPositionY->value();

            m_unit->setPosition( position );

            emit( changed() );
            updateGUI();
            SceneRoot::getInstance()->reloadUnits();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitPositionZ_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            osg::Vec3d position = m_unit->getPosition();

            position.z() = m_ui->spinBoxUnitPositionZ->value();

            m_unit->setPosition( position );

            emit( changed() );
            updateGUI();
            SceneRoot::getInstance()->reloadUnits();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitPositionZ_valueChanged(double arg1)
{
    m_ui->spinBoxUnitPositionZFt->setValue( floor( Converter::m2ft( arg1 ) + 0.5 ) );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitSpeed_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            m_unit->setVelocity( m_ui->spinBoxUnitSpeed->value() );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitSpeed_valueChanged(double arg1)
{
    m_ui->spinBoxUnitSpeedKts->setValue( floor( Converter::mps2kts( arg1 ) + 0.5 ) );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_spinBoxUnitHeading_editingFinished()
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            m_unit->setHeading( m_ui->spinBoxUnitHeading->value() );

            emit( changed() );
            updateGUI();
            SceneRoot::getInstance()->reloadUnits();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_checkBoxOwnship_toggled(bool checked)
{
    if ( m_inited )
    {
        if ( m_unit )
        {
            if ( checked )
            {
                m_ui->radioButtonFriend->setChecked( true );

                m_ui->labelUnitName->setEnabled( false );
                m_ui->lineEditUnitName->setEnabled( false );

                m_unit->setName( "ownship" );
            }
            else
            {
                m_ui->labelUnitName->setEnabled( true );
                m_ui->lineEditUnitName->setEnabled( true );

                m_unit->setName( m_ui->lineEditUnitName->text().toStdString() );
            }

            m_unit->setOwnship( checked );

            emit( changed() );
            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditUnit::on_pushButtonDone_clicked()
{
    emit( done() );
}
