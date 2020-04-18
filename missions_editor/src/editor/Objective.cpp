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

#include <editor/Objective.h>

#include <editor/ObjectiveAirspeed.h>
#include <editor/ObjectiveAltitude.h>
#include <editor/ObjectiveDefend.h>
#include <editor/ObjectiveDestroy.h>
#include <editor/ObjectiveDistance.h>
#include <editor/ObjectiveHeading.h>
#include <editor/ObjectiveTimeout.h>
#include <editor/ObjectiveWaypoint.h>

////////////////////////////////////////////////////////////////////////////////

Objective* Objective::readObjective( QDomElement &xmlNode )
{
    Objective *objective = 0;

    if ( !xmlNode.isNull() )
    {
        if ( xmlNode.tagName() == "objective_airspeed" )
        {
            objective = ObjectiveAirspeed::readObjective( xmlNode );
        }
        else if ( xmlNode.tagName() == "objective_altitude" )
        {
            objective = ObjectiveAltitude::readObjective( xmlNode );
        }
        else if ( xmlNode.tagName() == "objective_defend" )
        {
            objective = ObjectiveDefend::readObjective( xmlNode );
        }
        else if ( xmlNode.tagName() == "objective_destroy" )
        {
            objective = ObjectiveDestroy::readObjective( xmlNode );
        }
        else if ( xmlNode.tagName() == "objective_distance" )
        {
            objective = ObjectiveDistance::readObjective( xmlNode );
        }
        else if ( xmlNode.tagName() == "objective_heading" )
        {
            objective = ObjectiveHeading::readObjective( xmlNode );
        }
        else if ( xmlNode.tagName() == "objective_timeout" )
        {
            objective = ObjectiveTimeout::readObjective( xmlNode );
        }
        else if ( xmlNode.tagName() == "objective_waypoint" )
        {
            objective = ObjectiveWaypoint::readObjective( xmlNode );
        }
    }

    return objective;
}
