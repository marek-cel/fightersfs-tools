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

#include <editor/ObjectiveTimeout.h>

////////////////////////////////////////////////////////////////////////////////

ObjectiveTimeout* ObjectiveTimeout::readObjective( QDomElement &xmlNode )
{
    ObjectiveTimeout *objective = 0;

    if ( !xmlNode.isNull() )
    {
        float limit = xmlNode.attributeNode( "limit" ).value().toFloat();

        objective = new ObjectiveTimeout();

        objective->m_limit = limit;
    }

    return objective;
}

////////////////////////////////////////////////////////////////////////////////

ObjectiveTimeout::ObjectiveTimeout() :
    m_limit ( 0.0f )
{}

////////////////////////////////////////////////////////////////////////////////

Objective* ObjectiveTimeout::clone()
{
    Objective *obj = new ObjectiveTimeout();

    *obj = *this;

    return obj;
}

////////////////////////////////////////////////////////////////////////////////

void ObjectiveTimeout::saveObjective( QDomDocument &doc, QDomElement &parentNode )
{
    QDomElement objectiveTimeoutNode = doc.createElement( "objective_timeout" );
    parentNode.appendChild( objectiveTimeoutNode );

    QString limit = QString::number( m_limit );

    QDomAttr nodeLimit = doc.createAttribute( "limit" );

    nodeLimit.setValue( limit );

    objectiveTimeoutNode.setAttributeNode( nodeLimit );
}
