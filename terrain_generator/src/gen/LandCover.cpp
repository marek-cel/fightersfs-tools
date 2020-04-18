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

#include <gen/LandCover.h>

////////////////////////////////////////////////////////////////////////////////

//const double LandCover::Water::m_elevMax = 1.0e-3;
const double LandCover::Water::m_elevMax = 1.0e4;

////////////////////////////////////////////////////////////////////////////////

LandCover::LandCover() :
    m_crops ( 0 ),
    m_trees ( 0 ),
    m_urban ( 0 ),
    m_water ( 0 )
{
    m_crops = new Crops();
    m_trees = new Trees();
    m_urban = new Urban();
    m_water = new Water();
}

////////////////////////////////////////////////////////////////////////////////

LandCover::~LandCover()
{
    if ( m_crops ) delete m_crops;
    m_crops = 0;

    if ( m_trees ) delete m_trees;
    m_trees = 0;

    if ( m_urban ) delete m_urban;
    m_urban = 0;
}

////////////////////////////////////////////////////////////////////////////////

CoverType LandCover::getType( double lat1, double lon1 ) const
{
    CoverType type = CoverGrass;

    if ( m_water->isLand( lat1, lon1 ) )
    {
        if ( m_urban->isUrban( lat1, lon1 ) )
        {
            type = CoverUrban;
        }
        else if ( m_crops->isCrops( lat1, lon1 ) )
        {
            type = CoverCrops;
        }
        else if ( m_trees->isTrees( lat1, lon1 ) )
        {
            type = CoverTrees;
        }
        else
        {
            type = CoverGrass;
        }
    }
    else
    {
        type = CoverWater;
    }

    return type;
}

////////////////////////////////////////////////////////////////////////////////

CoverType LandCover::getType( double lat1, double lon1,
                              double lat2, double lon2 ) const
{
    CoverType type = CoverGrass;

    if ( m_water->isLand( lat1, lon1, lat2, lon2 ) )
    {
        if ( m_urban->isUrban( lat1, lon1, lat2, lon2 ) )
        {
            type = CoverUrban;
        }
        else if ( m_crops->isCrops( lat1, lon1, lat2, lon2 ) )
        {
            type = CoverCrops;
        }
        else if ( m_trees->isTrees( lat1, lon1, lat2, lon2 ) )
        {
            type = CoverTrees;
        }
        else
        {
            type = CoverGrass;
        }
    }
    else
    {
        type = CoverWater;
    }

    return type;
}

////////////////////////////////////////////////////////////////////////////////

CoverType LandCover::getType( double lat1, double lon1,
                              double lat2, double lon2,
                              double lat3, double lon3 ) const
{
    CoverType type = CoverGrass;

    if ( m_water->isLand( lat1, lon1, lat2, lon2, lat3, lon3 ) )
    {
        if ( m_urban->isUrban( lat1, lon1, lat2, lon2, lat3, lon3 ) )
        {
            type = CoverUrban;
        }
        else if ( m_crops->isCrops( lat1, lon1, lat2, lon2, lat3, lon3 ) )
        {
            type = CoverCrops;
        }
        else if ( m_trees->isTrees( lat1, lon1, lat2, lon2, lat3, lon3 ) )
        {
            type = CoverTrees;
        }
        else
        {
            type = CoverGrass;
        }
    }
    else
    {
        type = CoverWater;
    }

    return type;
}

////////////////////////////////////////////////////////////////////////////////

CoverType LandCover::getType( double lat1, double lon1,
                              double lat2, double lon2,
                              double lat3, double lon3,
                              double lat4, double lon4,
                              Level level ) const
{
    CoverType type = CoverGrass;

    if ( m_water->isLand( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4 ) )
    {
        if ( m_urban->isUrban( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4 ) )
        {
            type = CoverUrban;
        }
        else if ( m_crops->isCrops( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4 ) )
        {
            type = CoverCrops;
        }
        else if ( m_trees->isTrees( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4, level ) )
        {
            type = CoverTrees;
        }
        else
        {
            type = CoverGrass;
        }
    }
    else
    {
        type = CoverWater;
    }

    return type;
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::isCoast( double lat1, double lon1,
                         double lat2, double lon2,
                         double lat3, double lon3,
                         double lat4, double lon4 ) const
{
    return !( m_water->isLand( lat1, lon1 )
           && m_water->isLand( lat2, lon2 )
           && m_water->isLand( lat3, lon3 )
           && m_water->isLand( lat4, lon4 ) );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::isTrees( double lat1, double lon1,
                         double lat2, double lon2,
                         double lat3, double lon3,
                         double lat4, double lon4 ) const
{
    return ( m_trees->isTrees( lat1, lon1 )
          || m_trees->isTrees( lat2, lon2 )
          || m_trees->isTrees( lat3, lon3 )
          || m_trees->isTrees( lat4, lon4 ) );
}

////////////////////////////////////////////////////////////////////////////////

LandCover::Crops::Crops() {}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Crops::isCrops( double lat1, double lon1 ) const
{
    return ( 255 == getValue( lat1, lon1 ) );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Crops::isCrops( double lat1, double lon1,
                                double lat2, double lon2 ) const
{
    return isCrops( lat1, lon1 )
        && isCrops( lat2, lon2 );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Crops::isCrops( double lat1, double lon1,
                                double lat2, double lon2,
                                double lat3, double lon3 ) const
{
    return isCrops( lat1, lon1 )
        && isCrops( lat2, lon2 )
        && isCrops( lat3, lon3 );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Crops::isCrops( double lat1, double lon1,
                                double lat2, double lon2,
                                double lat3, double lon3,
                                double lat4, double lon4 ) const
{
    return isCrops( lat1, lon1 )
        && isCrops( lat2, lon2 )
        && isCrops( lat3, lon3 )
        && isCrops( lat4, lon4 );
}

////////////////////////////////////////////////////////////////////////////////

LandCover::Trees::Trees() {}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Trees::isTrees( double lat1, double lon1 ) const
{
    return ( 255 == getValue( lat1, lon1 ) );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Trees::isTrees( double lat1, double lon1,
                                double lat2, double lon2 ) const
{
    return isTrees( lat1, lon1 )
        && isTrees( lat2, lon2 );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Trees::isTrees( double lat1, double lon1,
                                double lat2, double lon2,
                                double lat3, double lon3 ) const
{
    return isTrees( lat1, lon1 )
        && isTrees( lat2, lon2 )
        && isTrees( lat3, lon3 );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Trees::isTrees( double lat1, double lon1,
                                double lat2, double lon2,
                                double lat3, double lon3,
                                double lat4, double lon4,
                                Level level ) const
{
    if ( level == LevelH )
    {
        return isTrees( lat1, lon1 )
            && isTrees( lat2, lon2 )
            && isTrees( lat3, lon3 )
            && isTrees( lat4, lon4 );
    }
    else
    {
        return ( isTrees( lat1, lon1 ) && isTrees( lat2, lon2 ) && isTrees( lat3, lon3 ) )
            || ( isTrees( lat1, lon1 ) && isTrees( lat2, lon2 ) && isTrees( lat4, lon4 ) )
            || ( isTrees( lat1, lon1 ) && isTrees( lat3, lon3 ) && isTrees( lat4, lon4 ) )
            || ( isTrees( lat2, lon2 ) && isTrees( lat3, lon3 ) && isTrees( lat4, lon4 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

LandCover::Urban::Urban() {}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Urban::isUrban( double lat1, double lon1 ) const
{
    return ( 255 == getValue( lat1, lon1 ) );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Urban::isUrban( double lat1, double lon1,
                                double lat2, double lon2 ) const
{
    return isUrban( lat1, lon1 )
        && isUrban( lat2, lon2 );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Urban::isUrban( double lat1, double lon1,
                                double lat2, double lon2,
                                double lat3, double lon3 ) const
{
    return isUrban( lat1, lon1 )
        && isUrban( lat2, lon2 )
        && isUrban( lat3, lon3 );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Urban::isUrban( double lat1, double lon1,
                                double lat2, double lon2,
                                double lat3, double lon3,
                                double lat4, double lon4 ) const
{
    return isUrban( lat1, lon1 )
        && isUrban( lat2, lon2 )
        && isUrban( lat3, lon3 )
        && isUrban( lat4, lon4 );
}

////////////////////////////////////////////////////////////////////////////////

LandCover::Water::Water() :
    m_elevation ( 0 )
{}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Water::isLand( double lat1, double lon1 ) const
{
    if ( m_elevation )
    {
        if ( m_elevation->getElevation( lat1, lon1 ) > m_elevMax )
        {
            return true;
        }
    }

    return ( 0 == getValue( lat1, lon1 ) );
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Water::isLand( double lat1, double lon1,
                               double lat2, double lon2 ) const
{
    bool result = isLand( lat1, lon1 )
               || isLand( lat2, lon2 );

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Water::isLand( double lat1, double lon1,
                               double lat2, double lon2,
                               double lat3, double lon3 ) const
{
    bool result = isLand( lat1, lon1 )
               || isLand( lat2, lon2 )
               || isLand( lat3, lon3 );

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool LandCover::Water::isLand( double lat1, double lon1,
                               double lat2, double lon2,
                               double lat3, double lon3,
                               double lat4, double lon4 ) const
{
    bool result = isLand( lat1, lon1 )
               || isLand( lat2, lon2 )
               || isLand( lat3, lon3 )
               || isLand( lat4, lon4 );

    return result;
}
