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
#ifndef GEO_H
#define GEO_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Math>

////////////////////////////////////////////////////////////////////////////////

class Geo
{
public:

    Geo();

    Geo( const Geo &geo );

    Geo( double origLat, double origLon, double scale );

    inline double d2r( double d ) { return osg::DegreesToRadians( d ); }
    inline double r2d( double r ) { return osg::RadiansToDegrees( r ); }

    double lat_d2m( double d );
    double lat_m2d( double m );

    double lon_d2m( double d );
    double lon_m2d( double m );

    inline double lat_m2r( double m ) { return d2r( lat_m2d( m ) ); }
    inline double lat_r2m( double r ) { return lat_d2m( r2d( r ) ); }

    inline double lon_m2r( double m ) { return d2r( lon_m2d( m ) ); }
    inline double lon_r2m( double r ) { return lon_d2m( r2d( r ) ); }

    void cart2geo( double x, double y, double &lat, double &lon );

    inline double getOrigLat() const { return m_origLat; }
    inline double getOrigLon() const { return m_origLon; }

    inline double getScale() const { return m_scale; }

    inline void setOrigLat( double origLat ) { m_origLat = origLat; update(); }
    inline void setOrigLon( double origLon ) { m_origLon = origLon; update(); }

    inline void setScale( double scale ) { m_scale = scale; update(); }

private:

    double m_scale;

    double m_origLat;
    double m_origLon;

    double m_lat_d2m;
    double m_lon_d2m;

    void update();
};

////////////////////////////////////////////////////////////////////////////////

#endif // GEO_H
