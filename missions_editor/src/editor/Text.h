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
#ifndef TEXT_H
#define TEXT_H

////////////////////////////////////////////////////////////////////////////////

#include <QMap>
#include <QDomElement>

////////////////////////////////////////////////////////////////////////////////

class Text
{
public:

    typedef QMap< int, QString > Strings;

    /* */
    static Text* readText( QDomElement &xmlNode );

    /** Default constructor. */
    Text();

    /** Cppy constructor. */
    Text( const Text &text );

    /** Destructor. */
    virtual ~Text();

    Text& operator= ( const Text &text );

    /** */
    std::string get() const;

    /** */
    std::string get( int index ) const;

    /** */
    void set( const char *str );

    /** */
    void set( const std::string &str );

    /** */
    void set( const QString &str );

    /** */
    void set( int index, const char *str );

    /** */
    void set( int index, const std::string &str );

    /** */
    void set( int index, const QString &str );

private:

    Strings m_strings;
};

////////////////////////////////////////////////////////////////////////////////

#endif // TEXT_H
