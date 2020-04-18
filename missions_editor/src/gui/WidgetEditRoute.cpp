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

#include "WidgetEditRoute.h"
#include "ui_WidgetEditRoute.h"

////////////////////////////////////////////////////////////////////////////////

WidgetEditRoute::WidgetEditRoute( QWidget *parent ) :
    QWidget ( parent ),
    m_ui( new Ui::WidgetEditRoute ),

    m_dialogEditWaypoint ( 0 ),

    m_routeIndex ( -1 ),
    m_currentWaypointIndex ( -1 ),
    m_route ( 0 )
{
    m_ui->setupUi( this );

    m_dialogEditWaypoint = new DialogEditWaypoint( this );
}

////////////////////////////////////////////////////////////////////////////////

WidgetEditRoute::~WidgetEditRoute()
{
    if ( m_dialogEditWaypoint ) delete m_dialogEditWaypoint;
    m_dialogEditWaypoint = 0;

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::edit( int routeIndex )
{
    m_inited = false;

    if ( routeIndex >= 0 && routeIndex < (int)Mission::getInstance()->getRoutes().size() )
    {
        m_routeIndex = routeIndex;
        m_route = Mission::getInstance()->getRoutes()[ routeIndex ];
    }
    else
    {
        m_routeIndex = -1;
        m_route = 0;
    }

    m_ui->listWidgetWaypoints->setCurrentRow( -1 );

    updateGUI();

    SceneRoot::getInstance()->showRoute( m_routeIndex );

    m_inited = true;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::editWaypoint()
{
    if ( m_inited )
    {
        if ( m_route )
        {
            if ( m_currentWaypointIndex >= 0 && m_currentWaypointIndex < (int)m_route->size() )
            {
                m_dialogEditWaypoint->setWaypoint( &( (*m_route)[ m_currentWaypointIndex ] ) );

                if ( m_dialogEditWaypoint->exec() == QDialog::Accepted )
                {
                    SceneRoot::getInstance()->reloadRoutes();
                    emit( changed() );
                    updateGUI();
                }

            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::updateGUI()
{
    int currentWaypointIndex = m_currentWaypointIndex;

    m_ui->listWidgetWaypoints->clear();

    if ( m_route )
    {
        m_ui->lineEditRouteName->setText( m_route->name.c_str() );

        for ( int i = 0; i < (int)m_route->size(); i++ )
        {
            QString name = "Waypoint #" + QString::number( i ) + ": ";

            name += QString::number( (*m_route)[ i ].first.x() ) + " ; "
                  + QString::number( (*m_route)[ i ].first.y() ) + " ; "
                  + QString::number( (*m_route)[ i ].first.z() ) + " m ";
            name += " - " + QString::number( (*m_route)[ i ].second ) + " m/s";

            m_ui->listWidgetWaypoints->addItem( new QListWidgetItem( name, m_ui->listWidgetWaypoints ) );
        }

        if ( currentWaypointIndex >= 0 && currentWaypointIndex < (int)m_route->size() )
        {
            m_currentWaypointIndex = currentWaypointIndex;
        }

        SceneRoot::getInstance()->showRoute( m_routeIndex );
    }

    m_ui->listWidgetWaypoints->setCurrentRow( m_currentWaypointIndex );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::on_lineEditRouteName_editingFinished()
{
    if ( m_inited )
    {
        if ( m_route )
        {
            m_route->name = m_ui->lineEditRouteName->text().toStdString();

            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::on_pushButtonAdd_clicked()
{
    if ( m_inited )
    {
        if ( m_route )
        {
            Waypoint waypoint;

            if ( (unsigned int)m_currentWaypointIndex + 1 < m_route->size() )
            {
                Waypoint waypointPrev = m_route->at( m_currentWaypointIndex );
                Waypoint waypointNext = m_route->at( m_currentWaypointIndex + 1 );

                waypoint.first.x() = 0.5 * ( waypointPrev.first.x() + waypointNext.first.x() );
                waypoint.first.y() = 0.5 * ( waypointPrev.first.y() + waypointNext.first.y() );
                waypoint.first.z() = 0.5 * ( waypointPrev.first.z() + waypointNext.first.z() );

                waypoint.second = 0.5 * ( waypointPrev.second + waypointNext.second );
            }

            m_route->insertWaypoint( m_currentWaypointIndex, waypoint );

            Unit *unit = Mission::getInstance()->getOwnship();

            if ( unit )
            {
                if ( QString( m_route->name.c_str() ) == QString( unit->getRoute().c_str() ) )
                {
                    m_route->increaseWaypointObjectivesIndeced( m_currentWaypointIndex );
                }
            }


            SceneRoot::getInstance()->reloadRoutes();
            emit( changed() );

            updateGUI();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::on_pushButtonEdit_clicked()
{
    editWaypoint();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::on_pushButtonRemove_clicked()
{
    if ( m_inited )
    {
        if ( m_route )
        {
            if ( m_currentWaypointIndex >= 0 && m_currentWaypointIndex < (int)m_route->size() )
            {
                m_route->eraseWaypoint( m_currentWaypointIndex );

                Unit *unit = Mission::getInstance()->getOwnship();

                if ( unit )
                {
                    if ( QString( m_route->name.c_str() ) == QString( unit->getRoute().c_str() ) )
                    {
                        m_route->decreaseWaypointObjectivesIndeced( m_currentWaypointIndex );
                    }
                }
            }


            SceneRoot::getInstance()->reloadRoutes();
            emit( changed() );

            updateGUI();
        }
    }
}


////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::on_listWidgetWaypoints_currentRowChanged(int currentRow)
{
    if ( m_inited )
    {
        if ( m_route && currentRow >= 0 && currentRow < (int)m_route->size() )
        {
            m_currentWaypointIndex = currentRow;

            m_ui->pushButtonEdit->setEnabled( true );
            m_ui->pushButtonRemove->setEnabled( true );

            SceneRoot::getInstance()->showRoute( m_routeIndex );

            if ( m_route )
            {
                SceneRoot::getInstance()->showMarker( (*m_route)[ m_currentWaypointIndex ].first );
            }
        }
        else
        {
            m_currentWaypointIndex = -1;

            m_ui->pushButtonEdit->setEnabled( false );
            m_ui->pushButtonRemove->setEnabled( false );

            SceneRoot::getInstance()->showRoute( m_routeIndex );
            SceneRoot::getInstance()->hideMarker();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::on_listWidgetWaypoints_doubleClicked(const QModelIndex &)
{
    editWaypoint();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetEditRoute::on_pushButtonDone_clicked()
{
    SceneRoot::getInstance()->hideMarker();

    emit( done() );
}
