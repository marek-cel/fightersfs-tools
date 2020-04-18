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

#include <editor/ObjectiveHeading.h>

////////////////////////////////////////////////////////////////////////////////

ObjectiveHeading* ObjectiveHeading::readObjective( QDomElement &xmlNode )
{
    ObjectiveHeading *objective = 0;

    if ( !xmlNode.isNull() )
    {
        float above = xmlNode.attributeNode( "above" ).value().toFloat();
        float below = xmlNode.attributeNode( "below" ).value().toFloat();

        objective = new ObjectiveHeading();

        objective->m_above = above;
        objective->m_below = below;
    }

    return objective;
}

////////////////////////////////////////////////////////////////////////////////

ObjectiveHeading::ObjectiveHeading() :
    m_above ( 50.0f ),
    m_below ( 400.0f )
{}

////////////////////////////////////////////////////////////////////////////////

Objective* ObjectiveHeading::clone()
{
    Objective *obj = new ObjectiveHeading();

    *obj = *this;

    return obj;
}

////////////////////////////////////////////////////////////////////////////////

void ObjectiveHeading::saveObjective( QDomDocument &doc, QDomElement &parentNode )
{
    QDomElement objectiveHeadingNode = doc.createElement( "objective_heading" );
    parentNode.appendChild( objectiveHeadingNode );

    QString above = QString::number( m_above );
    QString below = QString::number( m_below );

    QDomAttr nodeAbove = doc.createAttribute( "above" );
    QDomAttr nodeBelow = doc.createAttribute( "below" );

    nodeAbove.setValue( above );
    nodeBelow.setValue( below );

    objectiveHeadingNode.setAttributeNode( nodeAbove );
    objectiveHeadingNode.setAttributeNode( nodeBelow );
}
