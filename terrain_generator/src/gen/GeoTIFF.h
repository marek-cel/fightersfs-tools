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
#ifndef GEOTIFF_H
#define GEOTIFF_H

////////////////////////////////////////////////////////////////////////////////

#include <gdal/gdal.h>

////////////////////////////////////////////////////////////////////////////////

class GeoTIFF
{
public:

    GeoTIFF( const char *file = 0 );

    virtual ~GeoTIFF();

    void getGeo( int x, int y, double &lat, double &lon ) const;
    void getImg( double lat, double lon, int &x, int &y ) const;

    bool inRange( double lat, double lon ) const;

    short getValue( double lat, double lon ) const;

protected:

    GDALDatasetH m_hDataset;    ///<

    int m_raster_w;             ///< [pt]
    int m_raster_h;             ///< [pt]

    double m_ul_lat;            ///< [rad]
    double m_ul_lon;            ///< [rad]
    double m_lr_lat;            ///< [rad]
    double m_lr_lon;            ///< [rad]

    double m_pt_rad_x;          ///< [pt/rad]
    double m_pt_rad_y;          ///< [pt/rad]
};

////////////////////////////////////////////////////////////////////////////////

#endif // GEOTIFF_H
