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
#ifndef TERRAIN_H
#define TERRAIN_H

////////////////////////////////////////////////////////////////////////////////

#include <QDomDocument>
#include <QDomElement>

#include <osg/Group>

#include "Geo.h"

#include "Elevation.h"
#include "Exclusion.h"
#include "LandCover.h"

////////////////////////////////////////////////////////////////////////////////

/** */
class Terrain
{
public:

    /** */
    Terrain();

    /** */
    Terrain( QDomElement *xmlNode );

    /** */
    virtual ~Terrain();

    /** */
    void save( QDomDocument *doc, QDomElement *parentNode );

    /** */
    void newEmpty();

    /** */
    void generate();

    /** */
    bool generateElevation( QString fileName );

    /** */
    void exportTiles();

    inline osg::Group* getRoot() { return m_root.get(); }

    inline Geo getGeo() const { return m_geo; }

    inline double getOrigLat() const { return m_geo.getOrigLat(); }
    inline double getOrigLon() const { return m_geo.getOrigLon(); }

    inline unsigned short getTiles() const { return m_tiles; }

    inline double getScale() const { return m_geo.getScale(); }

    Elevation* getElevation() const { return m_elevation; }
    Exclusion* getExclusion() const { return m_exclusion; }
    LandCover* getLandCover() const { return m_landCover; }

    ListGeoTIFF::ListNames getListElevation() const { return m_elevation->getList(); }
    ListGeoTIFF::ListNames getListExclusion() const { return m_exclusion->getList(); }
    ListGeoTIFF::ListNames getListLandCrops() const { return m_landCover->getListCrops(); }
    ListGeoTIFF::ListNames getListLandTrees() const { return m_landCover->getListTrees(); }
    ListGeoTIFF::ListNames getListLandUrban() const { return m_landCover->getListUrban(); }
    ListGeoTIFF::ListNames getListLandWater() const { return m_landCover->getListWater(); }

    std::vector< std::string > getListDetailed() const { return m_detailed; }

    inline void setOrigLat( double origLat ) { m_geo.setOrigLat( origLat ); }
    inline void setOrigLon( double origLon ) { m_geo.setOrigLon( origLon ); }

    inline void setTiles( unsigned short tiles ) { m_tiles = tiles; }

    inline void setScale( double scale ) { m_geo.setScale( scale ); }

    inline void setListElevation( const ListGeoTIFF::ListNames &list ) { m_elevation->setList( list ); }
    inline void setListExclusion( const ListGeoTIFF::ListNames &list ) { m_exclusion->setList( list ); }
    inline void setListLandCrops( const ListGeoTIFF::ListNames &list ) { m_landCover->setListCrops( list ); }
    inline void setListLandTrees( const ListGeoTIFF::ListNames &list ) { m_landCover->setListTrees( list ); }
    inline void setListLandUrban( const ListGeoTIFF::ListNames &list ) { m_landCover->setListUrban( list ); }
    inline void setListLandWater( const ListGeoTIFF::ListNames &list ) { m_landCover->setListWater( list ); }

    inline void setListDetailed( const std::vector< std::string > &list ) { m_detailed = list; }

private:

    osg::ref_ptr<osg::Group> m_root;        ///< terrain root node

    Geo m_geo;                              ///<

    unsigned short m_tiles;                 ///<

    Elevation *m_elevation;                 ///<
    Exclusion *m_exclusion;                 ///<
    LandCover *m_landCover;                 ///<

    std::vector< std::string > m_detailed;  ///< detailed terrain

    osg::Vec3d getPosition( int ix, int iy );

    void readListNames( QDomElement *node, std::vector< std::string > &list );

    void saveListNames( const std::vector< std::string > &list,
                        QDomDocument *doc, QDomElement *parentNode );

    virtual void saveParameters( QDomDocument *doc, QDomElement *xmlNode );
};

////////////////////////////////////////////////////////////////////////////////

#endif // TERRAIN_H
