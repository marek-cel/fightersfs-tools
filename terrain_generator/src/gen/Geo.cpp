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

#include <osg/CoordinateSystemNode>

#include <gen/Geo.h>

////////////////////////////////////////////////////////////////////////////////

Geo::Geo() :
    m_scale ( 1.0 ),
    m_origLat ( 0.0 ),
    m_origLon ( 0.0 )
{
    update();
}

////////////////////////////////////////////////////////////////////////////////

Geo::Geo( const Geo &geo ) :
    m_scale ( geo.m_scale ),
    m_origLat ( geo.m_origLat ),
    m_origLon ( geo.m_origLon )
{
    update();
}

////////////////////////////////////////////////////////////////////////////////

Geo::Geo( double origLat, double origLon, double scale ) :
    m_scale ( scale ),
    m_origLat ( origLat ),
    m_origLon ( origLon )
{
    update();
}

////////////////////////////////////////////////////////////////////////////////

double Geo::lat_d2m( double d )
{
    return m_lat_d2m * d;
}

////////////////////////////////////////////////////////////////////////////////

double Geo::lat_m2d( double m )
{
    return m / m_lat_d2m;
}

////////////////////////////////////////////////////////////////////////////////

double Geo::lon_d2m( double d )
{
    return m_lon_d2m * d;
}

////////////////////////////////////////////////////////////////////////////////

double Geo::lon_m2d( double m )
{
    return m / m_lon_d2m;
}

////////////////////////////////////////////////////////////////////////////////

void Geo::cart2geo( double x, double y, double &lat, double &lon )
{
    lat = m_origLat + lat_m2r( y );
    lon = m_origLon + lon_m2r( x );
}

////////////////////////////////////////////////////////////////////////////////

void Geo::update()
{
    osg::ref_ptr<osg::EllipsoidModel> em = new osg::EllipsoidModel();

    double originX = 0.0;
    double originY = 0.0;
    double originZ = 0.0;

    em->convertLatLongHeightToXYZ( m_origLat, m_origLon, 0.0, originX, originY, originZ );

    double r_lon = sqrt( originX*originX + originY*originY );
    double r_lat = osg::Vec3d( originX, originY, originZ ).length();

    double circ_lon = 2.0 * M_PI * r_lon;
    double circ_lat = 2.0 * M_PI * r_lat;

    //osg::notify(osg::ALWAYS) << m_scale << std::endl;

    m_lat_d2m = m_scale * circ_lat / 360.0;
    m_lon_d2m = m_scale * circ_lon / 360.0;

//    m_lat_d2m = circ_lat / 360.0;
//    m_lon_d2m = circ_lon / 360.0;
}
