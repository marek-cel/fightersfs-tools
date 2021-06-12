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

#include <gen/Terrain.h>

#include <iostream>
#include <stdio.h>

#include <QTime>

#include <osg/LineSegment>
#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/IntersectVisitor>

#include <gen/Markers.h>
#include <gen/Tile.h>

////////////////////////////////////////////////////////////////////////////////

Terrain::Terrain() :
    m_root ( new osg::Group() ),

    m_tiles ( 1 ),

    m_elevation ( 0 ),
    m_exclusion ( 0 ),
    m_landCover ( 0 )
{
    Common::getInstance();

    newEmpty();
}

////////////////////////////////////////////////////////////////////////////////

Terrain::Terrain( QDomElement *xmlNode ) :
    m_root ( new osg::Group() ),

    m_tiles ( 1 ),

    m_elevation ( 0 ),
    m_exclusion ( 0 ),
    m_landCover ( 0 )
{
    newEmpty();

    m_geo.setOrigLat( osg::DegreesToRadians( xmlNode->attributeNode( "lat" ).value().toDouble() ) );
    m_geo.setOrigLon( osg::DegreesToRadians( xmlNode->attributeNode( "lon" ).value().toDouble() ) );

    m_tiles = xmlNode->attributeNode( "tiles" ).value().toInt();
    if ( m_tiles < 1 ) m_tiles = 1;

    m_geo.setScale( xmlNode->attributeNode( "scale" ).value().toDouble() );

    // elevation
    QDomElement elevationNode = xmlNode->firstChildElement( "elevation" );

    if ( elevationNode.tagName() == "elevation" )
    {
        ListGeoTIFF::ListNames list;
        readListNames( &elevationNode, list );
        m_elevation->setList( list );
        m_landCover->setElevation( m_elevation );
    }

    // exclusion
    QDomElement exclusionNode = xmlNode->firstChildElement( "exclusion" );

    if ( exclusionNode.tagName() == "exclusion" )
    {
        ListGeoTIFF::ListNames list;
        readListNames( &exclusionNode, list );
        m_exclusion->setList( list );
    }

    // land cover crops
    QDomElement landCropsNode = xmlNode->firstChildElement( "land_crops" );

    if ( landCropsNode.tagName() == "land_crops" )
    {
        ListGeoTIFF::ListNames list;
        readListNames( &landCropsNode, list );
        m_landCover->setListCrops( list );
    }

    // land cover trees
    QDomElement landTreesNode = xmlNode->firstChildElement( "land_trees" );

    if ( landTreesNode.tagName() == "land_trees" )
    {
        ListGeoTIFF::ListNames list;
        readListNames( &landTreesNode, list );
        m_landCover->setListTrees( list );
    }

    // land cover urban
    QDomElement landUrbanNode = xmlNode->firstChildElement( "land_urban" );

    if ( landUrbanNode.tagName() == "land_urban" )
    {
        ListGeoTIFF::ListNames list;
        readListNames( &landUrbanNode, list );
        m_landCover->setListUrban( list );
    }

    // land cover water
    QDomElement landWaterNode = xmlNode->firstChildElement( "land_water" );

    if ( landWaterNode.tagName() == "land_water" )
    {
        ListGeoTIFF::ListNames list;
        readListNames( &landWaterNode, list );
        m_landCover->setListWater( list );
    }

    // detailed
    QDomElement detailedNode = xmlNode->firstChildElement( "detailed" );

    if ( detailedNode.tagName() == "detailed" )
    {
        readListNames( &detailedNode, m_detailed );
    }

    generate();
}

////////////////////////////////////////////////////////////////////////////////

Terrain::~Terrain()
{
    if ( m_elevation ) delete m_elevation;
    m_elevation = 0;

    if ( m_exclusion ) delete m_exclusion;
    m_exclusion = 0;

    if ( m_landCover ) delete m_landCover;
    m_landCover = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Terrain::save( QDomDocument *doc, QDomElement *parentNode )
{
    saveParameters( doc, parentNode );
}

////////////////////////////////////////////////////////////////////////////////

void Terrain::newEmpty()
{
    if ( m_root.valid() )
    {
        m_root->removeChildren( 0, m_root->getNumChildren() );
    }

    if ( m_elevation ) delete m_elevation;
    m_elevation = new Elevation();

    if ( m_exclusion ) delete m_exclusion;
    m_exclusion = new Exclusion();

    if ( m_landCover ) delete m_landCover;
    m_landCover = new LandCover();

    m_detailed.clear();

    m_geo.setOrigLat( 0.0 );
    m_geo.setOrigLon( 0.0 );

    m_tiles = 1;
}

////////////////////////////////////////////////////////////////////////////////

void Terrain::generate()
{
    m_root->removeChildren( 0, m_root->getNumChildren() );

    int allTiles = 4 * m_tiles * m_tiles;
    QTime time;
    time.start();

    int tileIndex = 0;

    for ( int ix = -m_tiles; ix < m_tiles; ix++ )
    {
        for ( int iy = -m_tiles; iy < m_tiles; iy++)
        {
            osg::Vec3d pos = getPosition( ix, iy );

            osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
            m_root->addChild( pat.get() );
            pat->setPosition( pos );

            osg::ref_ptr<osg::LOD> lod = new osg::LOD();
            pat->addChild( lod.get() );

            lod->setCenterMode( osg::LOD::USER_DEFINED_CENTER );
            lod->setCenter( osg::Vec3d( 0.0, 0.0, 0.0 ) );
            lod->setRadius( 2.0 * Common::radiusL );
            lod->setRangeMode( osg::LOD::DISTANCE_FROM_EYE_POINT );

            osg::ref_ptr<osg::Group> subtiles = new osg::Group();
            lod->addChild( subtiles.get(), 0.0, Common::radiusB + Common::horizon );
            //lod->addChild( subtiles.get(), 0.0, 1000000000.0 );

            for ( int jx = 0; jx < 2; jx++ )
            {
                for ( int jy = 0; jy < 2; jy++)
                {
                    double dx = -Common::tileL / 2.0 + (double)jx * Common::tileL;
                    double dy = -Common::tileL / 2.0 + (double)jy * Common::tileL;

                    osg::ref_ptr<osg::PositionAttitudeTransform> pat_d = new osg::PositionAttitudeTransform();
                    subtiles->addChild( pat_d.get() );
                    pat_d->setPosition( osg::Vec3d( dx, dy, 0.0 ) );

                    Markers::createMarker( pat_d.get(), LevelL );

                    Tile tile( this, pos.x() + dx, pos.y() + dy, LevelL );
                    pat_d->addChild( tile.getRoot() );
                }
            }

            tileIndex++;
            int percentDone = 100 * tileIndex / allTiles;
            std::cout << "Done " << percentDone << " %" << std::endl;
        }
    }

    int sec = time.elapsed() / 1000;
    int min = sec / 60;
    if ( min > 0 ) { sec = sec % min; }
    time.setHMS( 0, min, sec );
    std::cout << "Terrain generation finished in " << time.toString( "HH:mm:ss" ).toStdString() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

bool Terrain::generateElevation( QString fileName )
{
    std::fstream file( fileName.toStdString().c_str(), std::ios_base::out );

    if ( file.is_open() )
    {
        std::vector< osg::ref_ptr<osg::Node> > detailedNodes;

        for ( unsigned int id = 0; id < m_detailed.size(); id++ )
        {
            osg::ref_ptr<osg::Node> detailedNode = osgDB::readNodeFile( m_detailed.at( id ) );

            if ( detailedNode.valid() )
            {
                detailedNodes.push_back( detailedNode.get() );
            }
        }

        const int nodes = (double)( 2*m_tiles ) * Common::block / Common::stepH + 1;
        const double scale = 10.0;
        const double coef = 1.0 / scale;

        file << nodes << "," << coef << "," << Common::stepH << std::endl;

        for ( int ix = 0; ix < nodes; ix++ )
        {
            double z_max = 0.0;

            if ( ix > 0 )
            {
                file << std::endl;
            }

            double x = ix * Common::stepH - m_tiles * Common::block;

            for ( int iy = 0; iy < nodes; iy++ )
            {
                //std::cout << ".";

                double y = iy * Common::stepH - m_tiles * Common::block;
                double z = 0.0;

                double lat = 0.0;
                double lon = 0.0;

                m_geo.cart2geo( x, y, lat, lon );

                bool intersect = false;

                for ( unsigned int id = 0; id < detailedNodes.size(); id++ )
                {
                    osg::ref_ptr<osg::Node> detailedNode = detailedNodes.at( id ).get();

                    osg::Vec3f b( x, y,  10000.0f );
                    osg::Vec3f e( x, y, -10000.0f );

                    osg::ref_ptr<osg::LineSegment> lineSegment = new osg::LineSegment( b, e );

                    osgUtil::IntersectVisitor intersectVisitor;
                    intersectVisitor.addLineSegment( lineSegment );
                    detailedNode->accept( intersectVisitor );

                    osgUtil::IntersectVisitor::HitList hitList = intersectVisitor.getHitList( lineSegment );

                    if ( !hitList.empty() )
                    {
                        osgUtil::Hit hit = hitList.front();

                        z = hit.getWorldIntersectPoint().z();

                        //osg::notify(osg::ALWAYS) << "HIT" << std::endl;

                        intersect = true;
                        break;
                    }
                }

                if ( !intersect )
                {
                    if ( CoverWater != m_landCover->getType( lat, lon ) )
                    {
                        z = m_elevation->getElevation( lat, lon );
                    }
                }

                if ( iy > 0 )
                {
                    file << ",";
                }

                if ( z < 0.0 ) z = 0.0;

                if ( z > z_max ) z_max = z;

                int z_int = floor( scale * z + 0.5 );

                file << z_int;
            }

            std::cout << "Row " << ix << " of " << nodes << " finished. z_max= " << z_max << std::endl;
        }

        file.close();

        std::cout << "GENERETING ELEVATION FINISHED" << std::endl;

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

void Terrain::exportTiles()
{
    const int tiles = 4 * m_tiles;

    int allTiles = 4 * tiles * tiles;
    int tileIndex = 0;

    for ( int ix = -tiles; ix < tiles; ix++ )
    {
        for ( int iy = -tiles; iy < tiles; iy++ )
        {
            double x = ( (double)ix + 0.5 ) * Common::tileM;
            double y = ( (double)iy + 0.5 ) * Common::tileM;

            Tile tile( this, x, y, LevelH, 0, false );

            QString name = "export/tile";
            name += "_x";
            name += QString::number( ix + tiles );
            name += "_y";
            name += QString::number( iy + tiles );
            name += ".osgb";

            osgDB::writeNodeFile( *( tile.getRoot() ), name.toStdString() );

            tileIndex++;
            int percentDone = 100 * tileIndex / allTiles;
            std::cout << "Done " << percentDone << " %" << std::endl;
        }
    }

    std::cout << "Export done." << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

osg::Vec3d Terrain::getPosition( int ix, int iy )
{
    return osg::Vec3d( ( (double)ix + 0.5 ) * Common::tileL * 2.0,
                       ( (double)iy + 0.5 ) * Common::tileL * 2.0,
                       0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void Terrain::readListNames( QDomElement *node, std::vector< std::string > &list )
{
    QDomElement fileNode = node->firstChildElement( "file" );

    while ( fileNode.tagName() == "file" )
    {
        list.push_back( fileNode.attributeNode( "path" ).value().toStdString() );
        fileNode = fileNode.nextSiblingElement( "file" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Terrain::saveListNames( const std::vector< std::string > &list,
                             QDomDocument *doc, QDomElement *parentNode )
{
    for ( unsigned int i = 0; i < list.size(); i++ )
    {
        QDomElement fileNode = doc->createElement( "file" );

        QDomAttr attrPath = doc->createAttribute( "path" );
        attrPath.setValue( QString( list[ i ].c_str() ) );
        fileNode.setAttributeNode( attrPath );

        parentNode->appendChild( fileNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Terrain::saveParameters( QDomDocument *doc, QDomElement *xmlNode )
{
    // origin
    QDomAttr attrOriginLat = doc->createAttribute( "lat" );
    QDomAttr attrOriginLon = doc->createAttribute( "lon" );

    attrOriginLat.setValue( QString::number( osg::RadiansToDegrees( m_geo.getOrigLat() ) ) );
    attrOriginLon.setValue( QString::number( osg::RadiansToDegrees( m_geo.getOrigLon() ) ) );

    xmlNode->setAttributeNode( attrOriginLat );
    xmlNode->setAttributeNode( attrOriginLon );

    // tiles
    QDomAttr attrTiles = doc->createAttribute( "tiles" );
    attrTiles.setValue( QString::number( m_tiles ) );
    xmlNode->setAttributeNode( attrTiles );

    // scale
    QDomAttr attrScale = doc->createAttribute( "scale" );
    attrScale.setValue( QString::number( m_geo.getScale() ) );
    xmlNode->setAttributeNode( attrScale );

    // elevation
    QDomElement elevationNode = doc->createElement( "elevation" );
    xmlNode->appendChild( elevationNode );
    saveListNames( m_elevation->getList(), doc, &elevationNode );

    // exclusion
    QDomElement exclusionNode = doc->createElement( "exclusion" );
    xmlNode->appendChild( exclusionNode );
    saveListNames( m_exclusion->getList(), doc, &exclusionNode );

    // land cover crops
    QDomElement landCropsNode = doc->createElement( "land_crops" );
    xmlNode->appendChild( landCropsNode );
    saveListNames( m_landCover->getListCrops(), doc, &landCropsNode );

    // land cover trees
    QDomElement landTreesNode = doc->createElement( "land_trees" );
    xmlNode->appendChild( landTreesNode );
    saveListNames( m_landCover->getListTrees(), doc, &landTreesNode );

    // land cover urban
    QDomElement landUrbanNode = doc->createElement( "land_urban" );
    xmlNode->appendChild( landUrbanNode );
    saveListNames( m_landCover->getListUrban(), doc, &landUrbanNode );

    // land cover water
    QDomElement landWaterNode = doc->createElement( "land_water" );
    xmlNode->appendChild( landWaterNode );
    saveListNames( m_landCover->getListWater(), doc, &landWaterNode );

    // detailed
    QDomElement detailedNode = doc->createElement( "detailed" );
    xmlNode->appendChild( detailedNode );
    saveListNames( m_detailed, doc, &detailedNode );
}

