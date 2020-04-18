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

#include "WidgetEditStage.h"
#include "ui_WidgetEditStage.h"

#include <gui/DialogAddObjective.h>
#include <gui/DialogAddUnit.h>

////////////////////////////////////////////////////////////////////////////////

WidgetEditStage::WidgetEditStage( QWidget *parent ) :
    QWidget ( parent ),
    m_ui( new Ui::WidgetEditStage ),

    m_dialogEditMessage ( 0 ),

    m_dialogEditObjectiveAirspeed ( 0 ),
    m_dialogEditObjectiveAltitude ( 0 ),
    m_dialogEditObjectiveDefend   ( 0 ),
    m_dialogEditObjectiveDestroy  ( 0 ),
    m_dialogEditObjectiveDistance ( 0 ),
    m_dialogEditObjectiveHeading  ( 0 ),
    m_dialogEditObjectiveTimeout  ( 0 ),
    m_dialogEditObjectiveWaypoint ( 0 ),

    m_currentMessageIndex   ( -1 ),
    m_currentInitUnitIndex  ( -1 ),
    m_currentObjectiveIndex ( -1 ),

    m_stageIndex ( -1 ),
    m_stage ( 0 )
{
    m_ui->setupUi( this );

    m_dialogEditMessage = new DialogEditMessage( this );

    m_dialogEditObjectiveAirspeed = new DialogEditObjectiveAirspeed( this );
    m_dialogEditObjectiveAltitude = new DialogEditObjectiveAltitude( this );
    m_dialogEditObjectiveDefend   = new DialogEditObjectiveDefend( this );
    m_dialogEditObjectiveDestroy  = new DialogEditObjectiveDestroy( this );
    m_dialogEditObjectiveDistance = new DialogEditObjectiveDistance( this );
    m_dialogEditObjectiveHeading  = new DialogEditObjectiveHeading( this );
    m_dialogEditObjectiveTimeout  = new DialogEditObjectiveTimeout( this );
    m_dialogEditObjectiveWaypoint = new DialogEditObjectiveWaypoint( this );
}

////////////////////////////////////////////////////////////////////////////////

WidgetEditStage::~WidgetEditStage()
{
    if ( m_dialogEditMessage ) delete m_dialogEditMessage;
    m_dialogEditMessage = 0;

    if ( m_dialogEditObjectiveAirspeed ) delete m_dialogEditObjectiveAirspeed;
    m_dialogEditObjectiveAirspeed = 0;

    if ( m_dialogEditObjectiveAltitude ) delete m_dialogEditObjectiveAltitude;
    m_dialogEditObjectiveAltitude = 0;

    if ( m_dialogEditObjectiveDefend ) delete m_dialogEditObjectiveDefend;
    m_dialogEditObjectiveDefend = 0;

    if ( m_dialogEditObjectiveDestroy ) delete m_dialogEditObjectiveDestroy;
    m_dialogEditObjectiveDestroy = 0;

    if ( m_dialogEditObjectiveDistance ) delete m_dialogEditObjectiveDistance;
    m_dialogEditObjectiveDistance = 0;

    if ( m_dialogEditObjectiveHeading ) delete m_dialogEditObjectiveHeading;
    m_dialogEditObjectiveHeading = 0;

    if ( m_dialogEditObjectiveTimeout ) delete m_dialogEditObjectiveTimeout;
    m_dialogEditObjectiveTimeout = 0;

    if ( m_dialogEditObjectiveWaypoint ) delete m_dialogEditObjectiveWaypoint;
    m_dialogEditObjectiveWaypoint = 0;

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::edit( int stageIndex )
{
    m_inited = false;

    m_ui->listWidgetMessages->clear();
    m_ui->listWidgetInitUnits->clear();
    m_ui->listWidgetObjectives->clear();

    if ( stageIndex >= 0 && stageIndex < (int)Mission::getInstance()->getStages().size() )
    {
        m_stageIndex = stageIndex;
        m_stage = Mission::getInstance()->getStages()[ stageIndex ];
    }
    else
    {
        m_stageIndex = -1;
        m_stage = 0;
    }

    m_ui->listWidgetMessages->setCurrentRow( -1 );
    m_ui->listWidgetInitUnits->setCurrentRow( -1 );
    m_ui->listWidgetObjectives->setCurrentRow( -1 );

    updateGUI();

    m_inited = true;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::editMessage()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            if ( m_currentMessageIndex >= 0 && m_currentMessageIndex < (int)m_stage->getMessages().size() )
            {
                Stage::Messages::iterator it = m_stage->getMessages().begin() + m_currentMessageIndex;

                Message *msg = *it;

                if ( msg )
                {
                    m_dialogEditMessage->setMessage( msg );

                    if ( m_dialogEditMessage->exec() == QDialog::Accepted )
                    {
                        emit( changed() );

                        updateGUI();
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::editObjective()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            if ( m_currentObjectiveIndex >= 0 && m_currentObjectiveIndex < (int)m_stage->getObjectives().size() )
            {
                Stage::Objectives::iterator it = m_stage->getObjectives().begin() + m_currentObjectiveIndex;

                Objective *obj = *it;

                if ( obj )
                {
                    ObjectiveAirspeed *objAirspeed = dynamic_cast< ObjectiveAirspeed* >( obj );
                    ObjectiveAltitude *objAltitude = dynamic_cast< ObjectiveAltitude* >( obj );
                    ObjectiveDefend   *objDefend   = dynamic_cast< ObjectiveDefend* >( obj );
                    ObjectiveDestroy  *objDestroy  = dynamic_cast< ObjectiveDestroy* >( obj );
                    ObjectiveDistance *objDistance = dynamic_cast< ObjectiveDistance* >( obj );
                    ObjectiveHeading  *objHeading  = dynamic_cast< ObjectiveHeading* >( obj );
                    ObjectiveTimeout  *objTimeout  = dynamic_cast< ObjectiveTimeout* >( obj );
                    ObjectiveWaypoint *objWaypoint = dynamic_cast< ObjectiveWaypoint* >( obj );

                    if ( objAirspeed )
                    {
                        m_dialogEditObjectiveAirspeed->setObj( objAirspeed );

                        if ( m_dialogEditObjectiveAirspeed->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                    else if ( objAltitude )
                    {
                        m_dialogEditObjectiveAltitude->setObj( objAltitude );

                        if ( m_dialogEditObjectiveAltitude->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                    else if ( objDefend )
                    {
                        m_dialogEditObjectiveDefend->setObj( objDefend );

                        if ( m_dialogEditObjectiveDefend->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                    else if ( objDestroy )
                    {
                        m_dialogEditObjectiveDestroy->setObj( objDestroy );

                        if ( m_dialogEditObjectiveDestroy->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                    else if ( objDistance )
                    {
                        m_dialogEditObjectiveDistance->setObj( objDistance );

                        if ( m_dialogEditObjectiveDistance->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                    else if ( objHeading )
                    {
                        m_dialogEditObjectiveHeading->setObj( objHeading );

                        if ( m_dialogEditObjectiveHeading->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                    else if ( objTimeout )
                    {
                        m_dialogEditObjectiveTimeout->setObj( objTimeout );

                        if ( m_dialogEditObjectiveTimeout->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                    else if ( objWaypoint )
                    {
                        m_dialogEditObjectiveWaypoint->setObj( objWaypoint );

                        if ( m_dialogEditObjectiveWaypoint->exec() == QDialog::Accepted )
                        {
                            emit( changed() );

                            updateGUI();
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::updateGUI()
{
    m_ui->listWidgetInitUnits->clear();
    m_ui->listWidgetObjectives->clear();

    if ( m_stage )
    {
        m_ui->lineEditStageName->setText( m_stage->getName().c_str() );

        m_ui->spinBoxTimeLimit->setValue( m_stage->getTimeLimit() );

        // messages
        m_ui->listWidgetMessages->clear();

        for ( int i = 0; i < (int)m_stage->getMessages().size(); i++ )
        {
            QString name = "Message #" + QString::number( i );

            m_ui->listWidgetMessages->addItem( new QListWidgetItem( name, m_ui->listWidgetMessages ) );
        }

        // init units
        m_ui->listWidgetInitUnits->clear();

        for ( int i = 0; i < (int)m_stage->getInitUnits().size(); i++ )
        {
            m_ui->listWidgetInitUnits->addItem( new QListWidgetItem( m_stage->getInitUnits().at( i ).c_str(), m_ui->listWidgetInitUnits ) );
        }

        // objectives
        m_ui->listWidgetObjectives->clear();

        for ( int i = 0; i < (int)m_stage->getObjectives().size(); i++ )
        {
            QString name = "Objective #" + QString::number( i ) + ": ";
            name += m_stage->getObjectives().at( i )->getTypeName();

            m_ui->listWidgetObjectives->addItem( new QListWidgetItem( name, m_ui->listWidgetObjectives ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_lineEditStageName_editingFinished()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            m_stage->setName( m_ui->lineEditStageName->text().toStdString() );

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_spinBoxTimeLimit_valueChanged(double arg1)
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            m_stage->setTimeLimit( arg1 );

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonMessagesAdd_clicked()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            Message *msg = new Message();

            if ( msg )
            {
                m_stage->getMessages().push_back( msg );
            }

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonMessagesEdit_clicked()
{
    editMessage();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonMessagesRemove_clicked()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            if ( m_currentMessageIndex >= 0 && m_currentMessageIndex < (int)m_stage->getMessages().size() )
            {
                m_stage->getMessages().erase( m_stage->getMessages().begin() + m_currentMessageIndex );
            }

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_listWidgetMessages_currentRowChanged(int currentRow)
{
    if ( m_inited )
    {
        if ( m_stage && currentRow >= 0 && currentRow < (int)m_stage->getMessages().size() )
        {
            m_currentMessageIndex = currentRow;

            m_ui->pushButtonMessagesEdit->setEnabled( true );
            m_ui->pushButtonMessagesRemove->setEnabled( true );
        }
        else
        {
            m_currentMessageIndex = -1;

            m_ui->pushButtonMessagesEdit->setEnabled( false );
            m_ui->pushButtonMessagesRemove->setEnabled( false );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_listWidgetMessages_doubleClicked(const QModelIndex &)
{
    editMessage();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonInitUnitAdd_clicked()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            std::string unit = DialogAddUnit::getUnit( this ).toStdString();

            if ( unit.length() > 0 )
            {
                m_stage->getInitUnits().push_back( unit );
            }

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonInitUnitRemove_clicked()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            if ( m_currentInitUnitIndex >= 0 && m_currentInitUnitIndex < (int)m_stage->getInitUnits().size() )
            {
                m_stage->getInitUnits().erase( m_stage->getInitUnits().begin() + m_currentInitUnitIndex );
            }

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_listWidgetInitUnits_currentRowChanged(int currentRow)
{
    if ( m_inited )
    {
        if ( m_stage && currentRow >= 0 && currentRow < (int)m_stage->getInitUnits().size() )
        {
            m_currentInitUnitIndex = currentRow;

            m_ui->pushButtonInitUnitRemove->setEnabled( true );
        }
        else
        {
            m_currentInitUnitIndex = -1;

            m_ui->pushButtonInitUnitRemove->setEnabled( false );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonObjAdd_clicked()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            Objective *obj = DialogAddObjective::getObjective( this );

            if ( obj )
            {
                m_stage->getObjectives().push_back( obj );
            }

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonObjEdit_clicked()
{
    editObjective();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonObjRemove_clicked()
{
    if ( m_inited )
    {
        if ( m_stage )
        {
            if ( m_currentObjectiveIndex >= 0 && m_currentObjectiveIndex < (int)m_stage->getObjectives().size() )
            {
                Stage::Objectives::iterator it = m_stage->getObjectives().begin() + m_currentObjectiveIndex;

                if ( *it ) delete (*it);
                (*it) = 0;

                m_stage->getObjectives().erase( it );
            }

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_listWidgetObjectives_currentRowChanged(int currentRow)
{
    if ( m_inited )
    {
        if ( m_stage && currentRow >= 0 && currentRow < (int)m_stage->getObjectives().size() )
        {
            m_currentObjectiveIndex = currentRow;

            m_ui->pushButtonObjEdit->setEnabled( true );
            m_ui->pushButtonObjRemove->setEnabled( true );
        }
        else
        {
            m_currentObjectiveIndex = -1;

            m_ui->pushButtonObjEdit->setEnabled( false );
            m_ui->pushButtonObjRemove->setEnabled( false );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_listWidgetObjectives_doubleClicked(const QModelIndex &)
{
    editObjective();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditStage::on_pushButtonDone_clicked()
{
    emit( done() );
}
