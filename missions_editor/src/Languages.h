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
#ifndef LANGUAGES_H
#define LANGUAGES_H

////////////////////////////////////////////////////////////////////////////////

#include <QString>
#include <QVector>

////////////////////////////////////////////////////////////////////////////////

/** */
class Languages
{
public:

    typedef QVector< QString > List;

    inline static Languages* instance()
    {
        if ( !m_instance )
        {
            m_instance = new Languages();
        }

        return m_instance;
    }

private:

    /** Default constructor. */
    Languages();

    /** Copy constructor. */
    Languages( const Languages & ) {}

public:

    /** Destructor. */
    virtual ~Languages();

    /** */
    inline int getCurrent() const { return m_current; }

    /** */
    inline int getDefault() const { return m_default; }

    /** */
    QString getCodeByIndex( int index ) const;

    /** */
    int getIndexByCode( const QString &code ) const;

    /** */
    inline int getCount() const { return m_list.size(); }

    /** */
    void setCurrent( int current );

private:

    static Languages *m_instance;

    List m_list;        ///< supported languages list

    int m_current;    ///< current language index
    int m_default;    ///< default language index

    void readLanguages();
};

////////////////////////////////////////////////////////////////////////////////

#endif // LANGUAGES_H
