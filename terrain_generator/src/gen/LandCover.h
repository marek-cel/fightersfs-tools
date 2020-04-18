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
#ifndef LANDCOVER_H
#define LANDCOVER_H

////////////////////////////////////////////////////////////////////////////////

#include "Elevation.h"
#include "Types.h"

////////////////////////////////////////////////////////////////////////////////

/** Land cover. */
class LandCover
{
public:

    /** Constructor. */
    LandCover();

    virtual ~LandCover();

    CoverType getType( double lat1, double lon1 ) const;

    CoverType getType( double lat1, double lon1,
                       double lat2, double lon2 ) const;

    CoverType getType( double lat1, double lon1,
                       double lat2, double lon2,
                       double lat3, double lon3 ) const;

    CoverType getType( double lat1, double lon1,
                       double lat2, double lon2,
                       double lat3, double lon3,
                       double lat4, double lon4,
                       Level level ) const;

    bool isCoast( double lat1, double lon1,
                  double lat2, double lon2,
                  double lat3, double lon3,
                  double lat4, double lon4 ) const;

    bool isTrees( double lat1, double lon1,
                  double lat2, double lon2,
                  double lat3, double lon3,
                  double lat4, double lon4 ) const;

    inline ListGeoTIFF::ListNames getListCrops() const { return m_crops->getList(); }
    inline ListGeoTIFF::ListNames getListTrees() const { return m_trees->getList(); }
    inline ListGeoTIFF::ListNames getListUrban() const { return m_urban->getList(); }
    inline ListGeoTIFF::ListNames getListWater() const { return m_water->getList(); }

    inline void setElevation( Elevation *elevation )
    {
        m_water->setElevation( elevation );
    }

    inline void setListCrops( const ListGeoTIFF::ListNames &list ) { m_crops->setList( list ); }
    inline void setListTrees( const ListGeoTIFF::ListNames &list ) { m_trees->setList( list ); }
    inline void setListUrban( const ListGeoTIFF::ListNames &list ) { m_urban->setList( list ); }
    inline void setListWater( const ListGeoTIFF::ListNames &list ) { m_water->setList( list ); }

private:

    /** Crops. */
    class Crops : public ListGeoTIFF
    {
    public:

        /** Constructor. */
        Crops();

        bool isCrops( double lat1, double lon1 ) const;

        bool isCrops( double lat1, double lon1,
                      double lat2, double lon2 ) const;

        bool isCrops( double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3 ) const;

        bool isCrops( double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3,
                      double lat4, double lon4 ) const;
    };

    /** Trees. */
    class Trees : public ListGeoTIFF
    {
    public:

        /** Constructor. */
        Trees();

        bool isTrees( double lat1, double lon1 ) const;

        bool isTrees( double lat1, double lon1,
                      double lat2, double lon2 ) const;

        bool isTrees( double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3 ) const;

        bool isTrees( double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3,
                      double lat4, double lon4,
                      Level level ) const;
    };

    /** Urban. */
    class Urban : public ListGeoTIFF
    {
    public:

        /** Constructor. */
        Urban();

        bool isUrban( double lat1, double lon1 ) const;

        bool isUrban( double lat1, double lon1,
                      double lat2, double lon2 ) const;

        bool isUrban( double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3 ) const;

        bool isUrban( double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3,
                      double lat4, double lon4 ) const;
    };

    /** Water. */
    class Water : public ListGeoTIFF
    {
    public:

        static const double m_elevMax;  ///< [m] max water elevation

        /** Constructor. */
        Water();

        bool isLand( double lat1, double lon1 ) const;

        bool isLand( double lat1, double lon1,
                     double lat2, double lon2 ) const;

        bool isLand( double lat1, double lon1,
                     double lat2, double lon2,
                     double lat3, double lon3 ) const;

        bool isLand( double lat1, double lon1,
                     double lat2, double lon2,
                     double lat3, double lon3,
                     double lat4, double lon4 ) const;

        inline void setElevation( Elevation *elevation )
        {
            m_elevation = elevation;
        }

    private:

        Elevation *m_elevation;
    };

    Crops *m_crops;     ///<
    Trees *m_trees;     ///<
    Urban *m_urban;     ///<
    Water *m_water;     ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // LANDCOVER_H
