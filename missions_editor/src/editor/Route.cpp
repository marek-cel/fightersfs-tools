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

#include <editor/Mission.h>
#include <editor/ObjectiveWaypoint.h>
#include <editor/Route.h>

////////////////////////////////////////////////////////////////////////////////

Route::Route() :
    std::vector< Waypoint >()
{}

////////////////////////////////////////////////////////////////////////////////

Route::Route( const Route &route ) :
    std::vector< Waypoint >( (std::vector< Waypoint >)route ),
    name ( route.name )
{}

////////////////////////////////////////////////////////////////////////////////

void Route::eraseWaypoint( int waypointIndex )
{
    erase( begin() + waypointIndex );
}

////////////////////////////////////////////////////////////////////////////////

void Route::insertWaypoint( int waypointIndex, const Waypoint &waypoint )
{
    int tempIndex = waypointIndex + 1;

    if ( tempIndex >= 0 && tempIndex < (int)size() )
    {
        insert( begin() + tempIndex, waypoint );
    }
    else
    {
        push_back( waypoint );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Route::decreaseWaypointObjectivesIndeced( int waypointIndex )
{
    for ( int i = 0; i < (int)Mission::getInstance()->getStages().size(); i++ )
    {
        Stage::Objectives::iterator it = Mission::getInstance()->getStages()[ i ]->getObjectives().begin();

        while ( it != Mission::getInstance()->getStages()[ i ]->getObjectives().end() )
        {
            ObjectiveWaypoint *obj = dynamic_cast< ObjectiveWaypoint* >( *it );

            if ( obj )
            {
                if ( (int)obj->getIndex() >= waypointIndex && obj->getIndex() > 0 )
                {
                    obj->setIndex( obj->getIndex() - 1 );
                }
            }

            ++it;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Route::increaseWaypointObjectivesIndeced( int waypointIndex )
{
    for ( int i = 0; i < (int)Mission::getInstance()->getStages().size(); i++ )
    {
        Stage::Objectives::iterator it = Mission::getInstance()->getStages()[ i ]->getObjectives().begin();

        while ( it != Mission::getInstance()->getStages()[ i ]->getObjectives().end() )
        {
            ObjectiveWaypoint *obj = dynamic_cast< ObjectiveWaypoint* >( *it );

            if ( obj )
            {
                if ( (int)obj->getIndex() >= waypointIndex )
                {
                    obj->setIndex( obj->getIndex() + 1 );
                }
            }

            ++it;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Route::removeFromAllUnits()
{
    for ( int i = 0; i < (int)Mission::getInstance()->getUnits().size(); i++ )
    {
        Unit *unit = Mission::getInstance()->getUnits()[ i ];

        if ( unit )
        {
            if ( QString( name.c_str() ) == QString( unit->getRoute().c_str() ) )
            {
                unit->setRoute( "" );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Route::removeWaypointObjectives()
{
    for ( int i = 0; i < (int)Mission::getInstance()->getStages().size(); i++ )
    {
        Stage::Objectives::iterator it = Mission::getInstance()->getStages()[ i ]->getObjectives().begin();

        while ( it != Mission::getInstance()->getStages()[ i ]->getObjectives().end() )
        {
            ObjectiveWaypoint *obj = dynamic_cast< ObjectiveWaypoint* >( *it );

            if ( obj )
            {
                it = Mission::getInstance()->getStages()[ i ]->getObjectives().erase( it );
            }
            else
            {
                ++it;
            }
        }
    }
}
