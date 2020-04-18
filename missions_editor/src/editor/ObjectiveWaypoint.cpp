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

#include <editor/ObjectiveWaypoint.h>

////////////////////////////////////////////////////////////////////////////////

ObjectiveWaypoint* ObjectiveWaypoint::readObjective( QDomElement &xmlNode )
{
    ObjectiveWaypoint *objective = 0;

    if ( !xmlNode.isNull() )
    {
        unsigned int index = xmlNode.attributeNode( "index" ).value().toInt();

        objective = new ObjectiveWaypoint();

        objective->m_index = index;
    }

    return objective;
}

////////////////////////////////////////////////////////////////////////////////

Objective* ObjectiveWaypoint::clone()
{
    Objective *obj = new ObjectiveWaypoint();

    *obj = *this;

    return obj;
}

////////////////////////////////////////////////////////////////////////////////

void ObjectiveWaypoint::saveObjective( QDomDocument &doc, QDomElement &parentNode )
{
    QDomElement objectiveWaypointNode = doc.createElement( "objective_waypoint" );
    parentNode.appendChild( objectiveWaypointNode );

    QString index = QString::number( m_index );

    QDomAttr nodeWaypointIndex = doc.createAttribute( "index" );
    nodeWaypointIndex.setValue( index );
    objectiveWaypointNode.setAttributeNode( nodeWaypointIndex );
}
