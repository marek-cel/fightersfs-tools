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

#include <editor/Stage.h>
#include <editor/Mission.h>

////////////////////////////////////////////////////////////////////////////////

Stage* Stage::readStage( QDomElement &xmlNode )
{
    if ( !xmlNode.isNull() )
    {
        std::string name = xmlNode.attributeNode( "name" ).value().toStdString();

        // time limit
        QDomElement timeLimitNode = xmlNode.firstChildElement( "time_limit" );

        float timeLimit = 0.0f;

        if ( !timeLimitNode.isNull() )
        {
            timeLimit = timeLimitNode.text().toFloat();
        }

        // init units
        QDomElement initUnitsNode = xmlNode.firstChildElement( "init_units" );

        InitUnits initUnits;

        if ( !initUnitsNode.isNull() )
        {
            QDomElement unitNode = initUnitsNode.firstChildElement( "unit" );

            while ( !unitNode.isNull() )
            {
                std::string unitName = unitNode.attributeNode( "name" ).value().toStdString();

                if ( unitName.length() > 0 )
                {
                    initUnits.push_back( unitName );
                }

                unitNode = unitNode.nextSiblingElement( "unit" );
            }
        }

        // message
        QDomElement messageNode = xmlNode.firstChildElement( "message" );

        Messages messages;

        while ( !messageNode.isNull() )
        {
            Message *message = Message::readMessage( messageNode );

            if ( message )
            {
                messages.push_back( message );
            }

            messageNode = messageNode.nextSiblingElement( "message" );
        }

        // objectives
        QDomElement objectivesNode = xmlNode.firstChildElement( "objectives" );

        Objectives objectives;

        QDomElement objectiveNode = objectivesNode.firstChildElement();

        while ( !objectiveNode.isNull() )
        {
            Objective *objective = Objective::readObjective( objectiveNode );

            if ( objective )
            {
                objectives.push_back( objective );
            }
            else
            {
                return 0;
            }

            objectiveNode = objectiveNode.nextSiblingElement();
        }

        Stage *stage = new Stage();

        stage->m_name      = name;
        stage->m_timeLimit = timeLimit;
        stage->m_initUnits = initUnits;
        stage->m_messages = messages;
        stage->m_objectives = objectives;

        return stage;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

Stage::Stage() :
    m_timeLimit ( 0.0f )
{}

////////////////////////////////////////////////////////////////////////////////

Stage::~Stage()
{
    for ( Objectives::iterator it = m_objectives.begin(); it != m_objectives.end(); it++ )
    {
        if ( *it ) delete (*it);
        (*it) = 0;
    }

    m_objectives.clear();
}

////////////////////////////////////////////////////////////////////////////////

void Stage::saveStage( QDomDocument &doc, QDomElement &parentNode )
{
    QDomElement stageNode = doc.createElement( "stage" );
    parentNode.appendChild( stageNode );

    if ( m_name.length() > 0 )
    {
        QDomAttr nodeStageName = doc.createAttribute( "name" );
        nodeStageName.setValue( m_name.c_str() );
        stageNode.setAttributeNode( nodeStageName );
    }

    if ( m_timeLimit > 0.0f )
    {
        Mission::saveTextNode( doc, stageNode, "time_limit", QString::number( m_timeLimit ).toStdString() );
    }

    QDomElement initUnitsNode = doc.createElement( "init_units" );
    stageNode.appendChild( initUnitsNode );

    for ( InitUnits::iterator it = m_initUnits.begin(); it != m_initUnits.end(); it++ )
    {
        QDomElement unitNode = doc.createElement( "unit" );
        initUnitsNode.appendChild( unitNode );

        QDomAttr nodeUnitName = doc.createAttribute( "name" );
        nodeUnitName.setValue( (*it).c_str() );
        unitNode.setAttributeNode( nodeUnitName );
    }

    for ( Messages::iterator it = m_messages.begin(); it != m_messages.end(); it++ )
    {
        (*it)->saveMessage( doc, stageNode );
    }

    QDomElement objectivesNode = doc.createElement( "objectives" );
    stageNode.appendChild( objectivesNode );

    for ( Objectives::iterator it = m_objectives.begin(); it != m_objectives.end(); it++ )
    {
        (*it)->saveObjective( doc, objectivesNode );
    }
}
