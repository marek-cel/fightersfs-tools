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

#include <editor/Text.h>

#include <Languages.h>

////////////////////////////////////////////////////////////////////////////////

Text* Text::readText( QDomElement &xmlNode )
{
    Text *text = new Text();

    if ( !xmlNode.isNull() )
    {
        QDomElement textNode = xmlNode.firstChildElement( "text" );

        while ( !textNode.isNull() )
        {
            if ( textNode.hasAttribute( "lang" ) )
            {
                QString code = textNode.attribute( "lang", "en" );

                int index = Languages::instance()->getIndexByCode( code );

                if ( index >= 0 && index <  Languages::instance()->getCount() )
                {
                    text->set( index, textNode.text() );
                }
            }

            textNode = textNode.nextSiblingElement( "text" );
        }
    }

    return text;
}

////////////////////////////////////////////////////////////////////////////////

Text::Text()
{
    m_strings.clear();

    for ( int i = 0; i < Languages::instance()->getCount(); i++ )
    {
        m_strings.insert( i, QString() );
    }
}

////////////////////////////////////////////////////////////////////////////////

Text::Text( const Text &text )
{
    m_strings = text.m_strings;
}

////////////////////////////////////////////////////////////////////////////////

Text::~Text() {}

////////////////////////////////////////////////////////////////////////////////

Text& Text::operator= ( const Text &text )
{
    m_strings = text.m_strings;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

std::string Text::get() const
{
    Strings::const_iterator it = m_strings.find( Languages::instance()->getCurrent() );

    if ( it != m_strings.end() )
    {
        return (*it).toStdString();
    }

    return std::string();
}

////////////////////////////////////////////////////////////////////////////////

std::string Text::get( int index ) const
{
    Strings::const_iterator it = m_strings.find( index );

    if ( it != m_strings.end() )
    {
        return (*it).toStdString();
    }

    return std::string();
}

////////////////////////////////////////////////////////////////////////////////

void Text::set( const char *str )
{
    set( QString( str ) );
}

////////////////////////////////////////////////////////////////////////////////

void Text::set( const std::string &str )
{
    set( QString( str.c_str() ) );
}

////////////////////////////////////////////////////////////////////////////////

void Text::set( const QString &str )
{
    set( Languages::instance()->getCurrent(), str );
}

////////////////////////////////////////////////////////////////////////////////

void Text::set( int index, const char *str )
{
    set( index, QString( str ) );
}

////////////////////////////////////////////////////////////////////////////////

void Text::set( int index, const std::string &str )
{
    set( index, QString( str.c_str() ) );
}

////////////////////////////////////////////////////////////////////////////////

void Text::set( int index, const QString &str )
{
    Strings::iterator it = m_strings.find( index );

    if ( it != m_strings.end() )
    {
        (*it) = str;
    }
    else
    {
        m_strings.insert( index, str );
    }
}
