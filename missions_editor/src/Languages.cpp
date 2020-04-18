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

#include <Languages.h>

#include <QDomElement>
#include <QDir>
#include <QFile>

////////////////////////////////////////////////////////////////////////////////

Languages* Languages::m_instance = 0;

////////////////////////////////////////////////////////////////////////////////

Languages::Languages() :
    m_current ( 0 ),
    m_default ( 0 )
{
    readLanguages();
}

////////////////////////////////////////////////////////////////////////////////

Languages::~Languages() {}

////////////////////////////////////////////////////////////////////////////////

QString Languages::getCodeByIndex( int index ) const
{
    if ( index >= 0 && index < m_list.size() )
    {
        return m_list.at( index );
    }

    return QString();
}

////////////////////////////////////////////////////////////////////////////////

int Languages::getIndexByCode( const QString &code ) const
{
    QString code1 = code.toLower();

    for ( int i = 0; i < m_list.size(); i++ )
    {
        QString code2 = m_list[ i ].toLower();

        if ( code1 == code2 )
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////

void Languages::setCurrent( int current )
{
    if ( current >= 0 && current < m_list.size() )
    {
        m_current = current;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Languages::readLanguages()
{
    QFile file( "data/languages.xml" );

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QDomDocument doc;

        doc.setContent( &file, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "languages" )
        {
            QDomElement languageNode = rootNode.firstChildElement( "language" );

            while ( !languageNode.isNull() )
            {
                m_list.push_back( languageNode.attributeNode( "code" ).value() );

                if ( 1 == languageNode.attribute( "default", "0" ).toInt() )
                {
                    m_default = m_list.size() - 1;
                }

                languageNode = languageNode.nextSiblingElement( "language" );
            }
        }
    }

    m_current = m_default;
}
