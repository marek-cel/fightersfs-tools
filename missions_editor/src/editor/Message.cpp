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

#include <editor/Message.h>
#include <editor/Mission.h>

////////////////////////////////////////////////////////////////////////////////

Message* Message::readMessage( QDomElement &xmlNode )
{
    Message *message = new Message();

    if ( !xmlNode.isNull() )
    {
        QDomElement captionNode = xmlNode.firstChildElement( "caption" );

        if ( !captionNode.isNull() )
        {
            QDomElement lineNode = captionNode.firstChildElement( "line" );

            Lines lines;

            while ( !lineNode.isNull() )
            {
                Text *line = Text::readText( lineNode );

                if ( line->get().length() > 0 )
                {
                    lines.push_back( *line );
                }
                else
                {
                    break;
                }

                if ( line ) delete line;
                line = 0;

                lineNode = lineNode.nextSiblingElement( "line" );
            }

            if ( lines.size() > 0 )
            {
                bool overlay = xmlNode.attributeNode( "overlay" ).value().toInt() == 1 ? true : false;

                QDomElement durationNode = xmlNode.firstChildElement( "duration" );

                if ( !durationNode.isNull() )
                {
                    double duration = durationNode.text().toDouble();

                    if ( duration > 0.0 )
                    {
                        QDomElement delayNode = xmlNode.firstChildElement( "delay" );

                        double delay = 0.0;

                        if ( !delayNode.isNull() )
                        {
                            delay = delayNode.text().toDouble();
                        }

                        message->m_overlay    = overlay;
                        message->m_duration   = duration;
                        message->m_delay      = delay;
                        message->m_lines      = lines;

                        // pointers
                        if ( Mission::getInstance()->getTutorial() )
                        {
                            message->m_pointer_custom  = xmlNode.attributeNode( "pointer_custom"  ).value().toInt() == 1 ? true : false;
                            message->m_pointer_target  = xmlNode.attributeNode( "pointer_target"  ).value().toInt() == 1 ? true : false;
                            message->m_pointer_rpm_dec = xmlNode.attributeNode( "pointer_rpm_dec" ).value().toInt() == 1 ? true : false;
                            message->m_pointer_rpm_inc = xmlNode.attributeNode( "pointer_rpm_inc" ).value().toInt() == 1 ? true : false;
                            message->m_pointer_trigger = xmlNode.attributeNode( "pointer_trigger" ).value().toInt() == 1 ? true : false;

                            message->m_pointer_x   = 0.0;
                            message->m_pointer_y   = 0.0;
                            message->m_pointer_phi = 0.0;

                            if ( message->m_pointer_custom )
                            {
                                QDomElement pointerNode = xmlNode.firstChildElement( "pointer_custom" );

                                if ( !pointerNode.isNull() )
                                {
                                    QDomElement xNode   = pointerNode.firstChildElement( "x" );
                                    QDomElement yNode   = pointerNode.firstChildElement( "y" );
                                    QDomElement phiNode = pointerNode.firstChildElement( "phi" );

                                    if ( !xNode.isNull()   ) message->m_pointer_x   =  xNode.text().toDouble();
                                    if ( !yNode.isNull()   ) message->m_pointer_y   =  yNode.text().toDouble();
                                    if ( !phiNode.isNull() ) message->m_pointer_phi = -phiNode.text().toDouble();
                                }
                            }

                            message->m_tutorial_tip = xmlNode.attributeNode( "tutorial_tip" ).value().toInt();
                        }

                    }
                }
            }
        }
    }

    return message;
}

////////////////////////////////////////////////////////////////////////////////

Message::Message() :
    m_overlay ( false ),
    m_duration ( 0.0 ),
    m_delay ( 0.0 ),

    m_pointer_custom  ( false ),
    m_pointer_target  ( false ),
    m_pointer_rpm_dec ( false ),
    m_pointer_rpm_inc ( false ),
    m_pointer_trigger ( false ),

    m_pointer_x ( 0.0 ),
    m_pointer_y ( 0.0 ),
    m_pointer_phi ( 0.0 )
{
    m_lines.clear();
}

////////////////////////////////////////////////////////////////////////////////

Message::~Message() {}

////////////////////////////////////////////////////////////////////////////////

void Message::saveMessage( QDomDocument &doc, QDomElement &parentNode )
{
    QDomElement messageNode = doc.createElement( "message" );
    parentNode.appendChild( messageNode );

    QDomAttr nodeOverlay = doc.createAttribute( "overlay" );
    nodeOverlay.setValue( m_overlay ? "1" : "0" );
    messageNode.setAttributeNode( nodeOverlay );

    QString duration = QString::number( m_duration );
    QString delay    = QString::number( m_delay );

    Mission::saveTextNode( doc, messageNode, "duration", duration.toStdString() );
    Mission::saveTextNode( doc, messageNode, "delay", delay.toStdString() );

    QDomElement captionNode = doc.createElement( "caption" );
    messageNode.appendChild( captionNode );

    for ( Lines::iterator it = m_lines.begin(); it != m_lines.end(); it++ )
    {
        Mission::saveTextNode( doc, captionNode, "line", (*it) );
    }

    // tutorial
    if ( Mission::getInstance()->getTutorial() )
    {
        // pointer_custom
        if ( m_pointer_custom )
        {
            QDomAttr nodePointer = doc.createAttribute( "pointer_custom" );
            nodePointer.setValue( m_pointer_custom ? "1" : "0" );
            messageNode.setAttributeNode( nodePointer );

            QDomElement pointerNode = doc.createElement( "pointer_custom" );
            messageNode.appendChild( pointerNode );

            QString x   = QString::number( m_pointer_x );
            QString y   = QString::number( m_pointer_y );
            QString phi = QString::number( -m_pointer_phi );

            Mission::saveTextNode( doc, pointerNode, "x"   , x.toStdString() );
            Mission::saveTextNode( doc, pointerNode, "y"   , y.toStdString() );
            Mission::saveTextNode( doc, pointerNode, "phi" , phi.toStdString() );
        }

        // pointer_target
        if ( m_pointer_target )
        {
            QDomAttr nodePointer = doc.createAttribute( "pointer_target" );
            nodePointer.setValue( m_pointer_target ? "1" : "0" );
            messageNode.setAttributeNode( nodePointer );
        }

        // pointer_rpm_decrease
        if ( m_pointer_rpm_dec )
        {
            QDomAttr nodePointer = doc.createAttribute( "pointer_rpm_dec" );
            nodePointer.setValue( m_pointer_rpm_dec ? "1" : "0" );
            messageNode.setAttributeNode( nodePointer );
        }

        // pointer_rpm_increase
        if ( m_pointer_rpm_inc )
        {
            QDomAttr nodePointer = doc.createAttribute( "pointer_rpm_inc" );
            nodePointer.setValue( m_pointer_rpm_inc ? "1" : "0" );
            messageNode.setAttributeNode( nodePointer );
        }

        // pointer_trigger
        if ( m_pointer_trigger )
        {
            QDomAttr nodePointer = doc.createAttribute( "pointer_trigger" );
            nodePointer.setValue( m_pointer_trigger ? "1" : "0" );
            messageNode.setAttributeNode( nodePointer );
        }

        // tutorial tip
        if ( m_tutorial_tip != 0 )
        {
            QDomAttr nodePointer = doc.createAttribute( "tutorial_tip" );
            nodePointer.setValue( QString::number( m_tutorial_tip ) );
            messageNode.setAttributeNode( nodePointer );
        }
    }
}
