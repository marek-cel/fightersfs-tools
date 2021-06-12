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

#include <limits>

#include <gen/ListGeoTIFF.h>

////////////////////////////////////////////////////////////////////////////////

ListGeoTIFF::ListGeoTIFF()
{
    clearFiles();
}

////////////////////////////////////////////////////////////////////////////////

ListGeoTIFF::~ListGeoTIFF()
{
    clearFiles();
}

////////////////////////////////////////////////////////////////////////////////

short ListGeoTIFF::getValue( double lat, double lon, bool nonZero ) const
{
    for ( unsigned int i = 0; i < m_listFiles.size(); i++ )
    {
        if ( m_listFiles[ i ] )
        {
            if ( m_listFiles[ i ]->inRange( lat, lon ) )
            {
                short value = m_listFiles[ i ]->getValue( lat, lon );

                if ( nonZero )
                {
                    if ( value != 0 )
                    {
                        return value;
                    }
                }
                else
                {
                    return value;
                }
            }
        }
    }

    if ( nonZero )
    {
        return 0;
    }

    return std::numeric_limits< short >::min();
}

////////////////////////////////////////////////////////////////////////////////

void ListGeoTIFF::setList( const ListNames &list )
{
    clearFiles();

    m_listNames.clear();
    m_listNames = list;

    for ( unsigned int i = 0; i < m_listNames.size(); i++ )
    {
        m_listFiles.push_back( new GeoTIFF( m_listNames[ i ].c_str() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void ListGeoTIFF::clearFiles()
{
    for ( unsigned int i = 0; i < m_listFiles.size(); i++ )
    {
        if ( m_listFiles[ i ] ) delete m_listFiles[ i ];
        m_listFiles[ i ] = 0;
    }

    m_listFiles.clear();
}
