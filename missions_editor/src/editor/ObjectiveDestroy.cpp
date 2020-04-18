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

#include <editor/ObjectiveDestroy.h>

////////////////////////////////////////////////////////////////////////////////

ObjectiveDestroy* ObjectiveDestroy::readObjective( QDomElement &xmlNode )
{
    ObjectiveDestroy *objective = 0;

    if ( !xmlNode.isNull() )
    {
        objective = new ObjectiveDestroy();

        QDomElement unitNode = xmlNode.firstChildElement( "unit" );

        while ( !unitNode.isNull() )
        {
            QString unitName = unitNode.attributeNode( "name" ).value();

            objective->m_units.push_back( unitName.toStdString() );

            unitNode = unitNode.nextSiblingElement( "unit" );
        }
    }

    return objective;
}

////////////////////////////////////////////////////////////////////////////////

Objective* ObjectiveDestroy::clone()
{
    Objective *obj = new ObjectiveDestroy();

    *obj = *this;

    return obj;
}

////////////////////////////////////////////////////////////////////////////////

void ObjectiveDestroy::saveObjective( QDomDocument &doc, QDomElement &parentNode )
{
    QDomElement objectiveDestroyNode = doc.createElement( "objective_destroy" );
    parentNode.appendChild( objectiveDestroyNode );

    for ( Units::iterator it = m_units.begin(); it != m_units.end(); it++ )
    {
        QDomElement unitNode = doc.createElement( "unit" );
        objectiveDestroyNode.appendChild( unitNode );

        QDomAttr nodeUnitName = doc.createAttribute( "name" );
        nodeUnitName.setValue( (*it).c_str() );
        unitNode.setAttributeNode( nodeUnitName );
    }
}
