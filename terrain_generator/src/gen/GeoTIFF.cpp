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
#include <iostream>
#include <stdio.h>
#include <vector>

#include <gdal/gdal.h>

#include <osg/Math>

#include <gen/GeoTIFF.h>

////////////////////////////////////////////////////////////////////////////////

GeoTIFF::GeoTIFF( const char *file ) :
    m_hDataset ( 0 ),

    m_raster_w ( 0 ),
    m_raster_h ( 0 ),

    m_ul_lat ( 0.0 ),
    m_ul_lon ( 0.0 ),
    m_lr_lat ( 0.0 ),
    m_lr_lon ( 0.0 ),

    m_pt_rad_x ( 0.0 ),
    m_pt_rad_y ( 0.0 )
{
    GDALAllRegister();

    if ( file )
    {
        m_hDataset = GDALOpen( file, GA_ReadOnly );

        if ( m_hDataset )
        {
            m_raster_w = GDALGetRasterXSize( m_hDataset );
            m_raster_h = GDALGetRasterYSize( m_hDataset );

            getGeo(          0,          0, m_ul_lat, m_ul_lon );
            getGeo( m_raster_w, m_raster_h, m_lr_lat, m_lr_lon );

            printf( "Size is %d, %d\n", m_raster_w, m_raster_h );
            printf( "Upper Left   %12.7f, %12.7f \n", m_ul_lon, m_ul_lat );
            printf( "Lower Right  %12.7f, %12.7f \n", m_lr_lon, m_lr_lat );

            m_ul_lat *= M_PI / 180.0;
            m_ul_lon *= M_PI / 180.0;
            m_lr_lat *= M_PI / 180.0;
            m_lr_lon *= M_PI / 180.0;

            double raster_w = (double)m_raster_w;
            double raster_h = (double)m_raster_h;

            m_pt_rad_x = raster_w / ( m_lr_lon - m_ul_lon );
            m_pt_rad_y = raster_h / ( m_lr_lat - m_ul_lat );

            double pt_deg_x = 1.0 / osg::DegreesToRadians( m_pt_rad_x );
            double pt_deg_y = 1.0 / osg::DegreesToRadians( m_pt_rad_y );

            printf( "Pixel Size = (%.15f,%.15f)\n", pt_deg_x, pt_deg_y );

            fflush( stdout );
        }
        else
        {
            std::cerr << "ERROR! Cannot open file: " << file << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

GeoTIFF::~GeoTIFF()
{
    if ( m_hDataset ) GDALClose( m_hDataset );
}

////////////////////////////////////////////////////////////////////////////////

void GeoTIFF::getGeo( int x, int y, double &lat, double &lon ) const
{
    double temp[ 6 ];

    // Transform the point into georeferenced coordinates.
    if( GDALGetGeoTransform( m_hDataset, temp ) == CE_None )
    {
        lon = temp[ 0 ] + temp[ 1 ] * (double)x + temp[ 2 ] * (double)y;
        lat = temp[ 3 ] + temp[ 4 ] * (double)x + temp[ 5 ] * (double)y;
    }
}

////////////////////////////////////////////////////////////////////////////////

void GeoTIFF::getImg( double lat, double lon, int &x, int &y ) const
{
    y = (int)floor( m_pt_rad_y * ( lat - m_ul_lat ) + 0.5 );
    x = (int)floor( m_pt_rad_x * ( lon - m_ul_lon ) + 0.5 );
}

////////////////////////////////////////////////////////////////////////////////

bool GeoTIFF::inRange( double lat, double lon ) const
{
    if ( m_lr_lat < lat && lat < m_ul_lat
      && m_ul_lon < lon && lon < m_lr_lon )
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

short GeoTIFF::getValue( double lat, double lon ) const
{
    if ( inRange( lat, lon ) )
    {
        int x = 0;
        int y = 0;

        getImg( lat, lon, x, y );

        if ( x >= 0 && y >= 0 && x < m_raster_w && y < m_raster_h )
        {
            double adfPixel[ 2 ];

            GDALRasterBandH hBand = GDALGetRasterBand( m_hDataset, 1 );

            if ( CE_None == GDALRasterIO( hBand, GF_Read, x, y, 1, 1,
                                          adfPixel, 1, 1, GDT_CFloat64, 0, 0 ) )
            {
                return adfPixel[ 0 ];
            }
        }
    }

    return std::numeric_limits< short >::min();
}

