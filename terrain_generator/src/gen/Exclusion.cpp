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

#include <gen/Exclusion.h>

////////////////////////////////////////////////////////////////////////////////

Exclusion::Exclusion()
{}

////////////////////////////////////////////////////////////////////////////////

Exclusion::~Exclusion() {}

////////////////////////////////////////////////////////////////////////////////

bool Exclusion::isExcluded( double lat1, double lon1,
                            double lat2, double lon2,
                            double lat3, double lon3 )
{
    bool result = isExcluded( lat1, lon1 )
               || isExcluded( lat2, lon2 )
               || isExcluded( lat3, lon3 );

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Exclusion::isExcluded( double lat1, double lon1,
                            double lat2, double lon2,
                            double lat3, double lon3,
                            double lat4, double lon4 )
{
    bool result = isExcluded( lat1, lon1 )
               || isExcluded( lat2, lon2 )
               || isExcluded( lat3, lon3 )
               || isExcluded( lat4, lon4 );

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Exclusion::isExcluded( double lat, double lon )
{
    return ( 255 == getValue( lat, lon ) );
}
