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

#include <gen/Tile.h>

#include <iostream>

#include <osg/PositionAttitudeTransform>
#include <osgUtil/Optimizer>

#include <gen/Markers.h>

////////////////////////////////////////////////////////////////////////////////

const bool Tile::m_zeroizeElevation = true;

////////////////////////////////////////////////////////////////////////////////

Tile::Tile( Level level ) :
    m_ocean ( true ),

    m_x0 ( 0.0 ),
    m_y0 ( 0.0 ),

    m_level ( level ),
    m_lod ( false ),

    m_terrain ( 0 ),
    m_parent ( 0 ),

    m_step ( 0.0 )
{
    init();
}

////////////////////////////////////////////////////////////////////////////////

Tile::Tile( Terrain *terrain, double x0, double y0, Level level, Tile *parent, bool lod ) :
    m_ocean ( false ),

    m_x0 ( x0 ),
    m_y0 ( y0 ),

    m_level ( level ),
    m_lod ( lod ),

    m_terrain ( terrain ),
    m_parent ( parent ),

    m_step ( 0.0 )
{
    init();

    if ( m_level == LevelM )
    {
        Tile tile( m_terrain, 0.0, 0.0, LevelH, this, false );

        if ( m_lod )
        {
            m_rootLOD->addChild( tile.getRoot(), m_range0, m_range1 );
        }
        else
        {
            m_root->addChild( tile.getRoot() );
        }
    }
    else if ( m_level > LevelM )
    {
        generateSubtiles();
    }
}

////////////////////////////////////////////////////////////////////////////////

double Tile::getAbsX0() const
{
    double x0 = m_x0;

    if ( m_parent )
    {
        x0 += m_parent->getAbsX0();
    }

    return x0;
}

////////////////////////////////////////////////////////////////////////////////

double Tile::getAbsY0() const
{
    double y0 = m_y0;

    if ( m_parent )
    {
        y0 += m_parent->getAbsY0();
    }

    return y0;
}

////////////////////////////////////////////////////////////////////////////////

void Tile::init()
{
    if ( m_level == LevelH )
    {
        m_step = Common::stepH;
        m_size = Common::tileM;

        m_radius = Common::radiusH;

        m_range0 = 0.0;
        m_range1 = 0.0;
        m_range2 = 1.4 * Common::radiusM + 1000.0;  // 1.4 * 5775 + 1000 = 9085
    }
    else if ( m_level == LevelM )
    {
        m_step = Common::stepM;
        m_size = Common::tileM;

        m_radius = Common::radiusM;

        m_range0 = 0.0;
        m_range1 = 1.4 * Common::radiusM + 1000.0;  // 1.4 * 5775 + 1000 = 9085
        m_range2 = 1.7 * Common::radiusL;           // 1.7 * 11551 = 19636
    }
    else
    {
        m_step = Common::stepL;
        m_size = Common::tileL;

        m_radius = Common::radiusL;

        m_range0 = 0.0;
        m_range1 = 1.2 * Common::radiusL;               // 1.2 * 11551 = 13861
        m_range2 = Common::horizon + Common::radiusL;   // 10000 + 11551 = 21551
    }

    if ( m_ocean ) m_step = Common::stepL;

    m_offset = m_size / 2.0;

    m_steps = floor( m_size / m_step + 0.5 );

    if ( m_lod )
    {
        m_rootLOD = new osg::LOD();
        m_root = m_rootLOD;

        m_rootLOD->setCenterMode( osg::LOD::USER_DEFINED_CENTER );
        m_rootLOD->setCenter( osg::Vec3d( 0.0, 0.0, 0.0 ) );
        m_rootLOD->setRadius( m_radius );
        m_rootLOD->setRangeMode( osg::LOD::DISTANCE_FROM_EYE_POINT );
    }
    else
    {
        m_root = new osg::Group();
    }

    createData();
    generate();
    createGeom();
    addToGeode();
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createData()
{
    for ( int i = 0; i < TILE_MAX_COVER_TYPES; i++ )
    {
        createData( m_cover[ i ], Common::getInstance()->getCoverStateSet( (CoverType)i, m_level ) );
    }

    for ( int i = 0; i < TILE_MAX_COAST_SQUARES_TYPES; i++ )
    {
        for ( int j = 0; j < 64; j++ )
        {
            createData( m_coastSquares[ i ][ j ], Common::getInstance()->getCoastStateSetSquares( i, j ) );
        }
    }

    for ( int i = 0; i < TILE_MAX_COAST_ROWS_TYPES; i++ )
    {
        for ( int j = 0; j < 8; j++ )
        {
            createData( m_coastRows[ i ][ j ], Common::getInstance()->getCoastStateSetRows( i, j ) );
        }
    }

    for ( int i = 0; i < TILE_MAX_COAST_COLS_TYPES; i++ )
    {
        for ( int j = 0; j < 8; j++ )
        {
            createData( m_coastCols[ i ][ j ], Common::getInstance()->getCoastStateSetCols( i, j ) );
        }
    }

    for ( int i = 0; i < TILE_MAX_TREES_SQUARES_TYPES; i++ )
    {
        for ( int j = 0; j < 64; j++ )
        {
            createData( m_treesSquares[ i ][ j ], Common::getInstance()->getTreesStateSetSquares( i, j ) );
        }
    }

    for ( int i = 0; i < TILE_MAX_TREES_ROWS_TYPES; i++ )
    {
        for ( int j = 0; j < 8; j++ )
        {
            createData( m_treesRows[ i ][ j ], Common::getInstance()->getTreesStateSetRows( i, j ) );
        }
    }

    for ( int i = 0; i < TILE_MAX_TREES_COLS_TYPES; i++ )
    {
        for ( int j = 0; j < 8; j++ )
        {
            createData( m_treesCols[ i ][ j ], Common::getInstance()->getTreesStateSetCols( i, j ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createData( Data &data, osg::StateSet *stateSet )
{
    data.vtn.v = new osg::Vec3Array();
    data.vtn.t = new osg::Vec2Array();
    data.vtn.n = new osg::Vec3Array();

    data.geometry = new osg::Geometry();
    data.geometry->setStateSet( stateSet );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::generate()
{
    m_ground = false;

    for ( int jy = 0; jy < m_steps; jy++ )
    {
        for ( int jx = 0; jx < m_steps; jx++ )
        {
            createSquare( jx, jy );
        }
    }

    if ( m_terrain != 0 && m_ground && m_level > LevelH )
    {
        for ( int jy = 0; jy < m_steps; jy++ )
        {
            for ( int jx = 0; jx < m_steps; jx++ )
            {
                if ( jy == 0 )
                {
                    createBorderS( jx, jy );
                }
                else if ( jy == m_steps - 1 )
                {
                    createBorderN( jx, jy );
                }

                if ( jx == 0 )
                {
                    createBorderW( jx, jy );
                }
                else if ( jx == m_steps - 1 )
                {
                    createBorderE( jx, jy );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createSquare( int jx, int jy )
{
    osg::Vec3d r1( 0.0, 0.0, 0.0 );
    osg::Vec3d r2;
    osg::Vec3d r3;
    osg::Vec3d r4;

    double lat1 = 0.0;
    double lat2 = 0.0;
    double lat3 = 0.0;
    double lat4 = 0.0;

    double lon1 = 0.0;
    double lon2 = 0.0;
    double lon3 = 0.0;
    double lon4 = 0.0;

    bool even = false;

    if ( ( jx % 2 == 0 && jy % 2 != 0 )
      || ( jx % 2 != 0 && jy % 2 == 0 ) )
    {
        even = true;
    }

    r1.x() = ( jx + 0 ) * m_step - m_offset;
    r1.y() = ( jy + 0 ) * m_step - m_offset;

    r2.x() = ( jx + 1 ) * m_step - m_offset;
    r2.y() = ( jy + 0 ) * m_step - m_offset;

    r3.x() = ( jx + 0 ) * m_step - m_offset;
    r3.y() = ( jy + 1 ) * m_step - m_offset;

    r4.x() = ( jx + 1 ) * m_step - m_offset;
    r4.y() = ( jy + 1 ) * m_step - m_offset;

    bool excluded = false;

    if ( m_terrain )
    {
        m_terrain->getGeo().cart2geo( getAbsX( r1.x() ), getAbsY( r1.y() ), lat1, lon1 );
        m_terrain->getGeo().cart2geo( getAbsX( r2.x() ), getAbsY( r2.y() ), lat2, lon2 );
        m_terrain->getGeo().cart2geo( getAbsX( r3.x() ), getAbsY( r3.y() ), lat3, lon3 );
        m_terrain->getGeo().cart2geo( getAbsX( r4.x() ), getAbsY( r4.y() ), lat4, lon4 );

        r1.z() = m_terrain->getElevation()->getElevation( lat1, lon1 );
        r2.z() = m_terrain->getElevation()->getElevation( lat2, lon2 );
        r3.z() = m_terrain->getElevation()->getElevation( lat3, lon3 );
        r4.z() = m_terrain->getElevation()->getElevation( lat4, lon4 );

        excluded = m_terrain->getExclusion()->isExcluded( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4 );
    }

    if ( m_level > LevelH || !excluded )
    {
        if ( m_terrain != 0
          && CoverWater != m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4, m_level ) )
        {
            if ( m_level == LevelH )
            {
                m_ground = true;

                if ( m_terrain->getLandCover()->isCoast( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4 ) )
                {
                    createCoast( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                                 r1, r2, r3, r4, jx, jy );
                }
                else if ( m_terrain->getLandCover()->isTrees( lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4 ) )
                {
                    createTrees( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                                 r1, r2, r3, r4, jx, jy );
                }
                else
                {
                    createCover( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                                 r1, r2, r3, r4 );
                }
            }
            else
            {
                createCover( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                             r1, r2, r3, r4 );
            }
        }
        else
        {
            if ( m_zeroizeElevation )
            {
                r1.z() = 0.0;
                r2.z() = 0.0;
                r3.z() = 0.0;
                r4.z() = 0.0;
            }

            createSquare( even, m_cover[ CoverWater ].vtn, m_cover[ CoverWater ].vtn,
                          r1, r2, r3, r4, Common::texL, Common::texL );
        }
    }
//    else
//    {
//        std::cerr << "Exclusion" << std::endl;
//    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createBorderS( int jx, int jy )
{
    osg::Vec3d r1;
    osg::Vec3d r2;
    osg::Vec3d r3;
    osg::Vec3d r4;

    double lat1 = 0.0;
    double lat2 = 0.0;

    double lon1 = 0.0;
    double lon2 = 0.0;

    r1.x() = ( jx + 0 ) * m_step - m_offset;
    r1.y() = ( jy + 0 ) * m_step - m_offset;

    r2.x() = ( jx + 1 ) * m_step - m_offset;
    r2.y() = ( jy + 0 ) * m_step - m_offset;

    r3.x() = r1.x();
    r3.y() = r1.y();

    r4.x() = r2.x();
    r4.y() = r2.y();

    m_terrain->getGeo().cart2geo( getAbsX( r1.x() ), getAbsY( r1.y() ), lat1, lon1 );
    m_terrain->getGeo().cart2geo( getAbsX( r2.x() ), getAbsY( r2.y() ), lat2, lon2 );

    r1.z() = m_terrain->getElevation()->getElevation( lat1, lon1 );
    r2.z() = m_terrain->getElevation()->getElevation( lat2, lon2 );
    r3.z() = 0.0;
    r4.z() = 0.0;

    if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 )
      && ( r1.z() > 0.0 || r2.z() > 0.0 ) )
    {
        CoverType type = m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 );

        createBorder( m_cover[ type ].vtn, r1, r2, r3, r4 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createBorderN( int jx, int jy )
{
    osg::Vec3d r1;
    osg::Vec3d r2;
    osg::Vec3d r3;
    osg::Vec3d r4;

    double lat1 = 0.0;
    double lat2 = 0.0;

    double lon1 = 0.0;
    double lon2 = 0.0;

    r1.x() = ( jx + 0 ) * m_step - m_offset;
    r1.y() = ( jy + 1 ) * m_step - m_offset;

    r2.x() = ( jx + 1 ) * m_step - m_offset;
    r2.y() = ( jy + 1 ) * m_step - m_offset;

    r3.x() = r1.x();
    r3.y() = r1.y();

    r4.x() = r2.x();
    r4.y() = r2.y();

    m_terrain->getGeo().cart2geo( getAbsX( r1.x() ), getAbsY( r1.y() ), lat1, lon1 );
    m_terrain->getGeo().cart2geo( getAbsX( r2.x() ), getAbsY( r2.y() ), lat2, lon2 );

    r1.z() = m_terrain->getElevation()->getElevation( lat1, lon1 );
    r2.z() = m_terrain->getElevation()->getElevation( lat2, lon2 );
    r3.z() = 0.0;
    r4.z() = 0.0;

    if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 )
      && ( r1.z() > 0.0 || r2.z() > 0.0 ) )
    {
        CoverType type = m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 );

        createBorder( m_cover[ type ].vtn, r1, r2, r3, r4 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createBorderW( int jx, int jy )
{
    osg::Vec3d r1;
    osg::Vec3d r2;
    osg::Vec3d r3;
    osg::Vec3d r4;

    double lat1 = 0.0;
    double lat2 = 0.0;

    double lon1 = 0.0;
    double lon2 = 0.0;

    r1.x() = ( jx + 0 ) * m_step - m_offset;
    r1.y() = ( jy + 0 ) * m_step - m_offset;

    r2.x() = ( jx + 0 ) * m_step - m_offset;
    r2.y() = ( jy + 1 ) * m_step - m_offset;

    r3.x() = r1.x();
    r3.y() = r1.y();

    r4.x() = r2.x();
    r4.y() = r2.y();

    m_terrain->getGeo().cart2geo( getAbsX( r1.x() ), getAbsY( r1.y() ), lat1, lon1 );
    m_terrain->getGeo().cart2geo( getAbsX( r2.x() ), getAbsY( r2.y() ), lat2, lon2 );

    r1.z() = m_terrain->getElevation()->getElevation( lat1, lon1 );
    r2.z() = m_terrain->getElevation()->getElevation( lat2, lon2 );
    r3.z() = 0.0;
    r4.z() = 0.0;

    if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 )
      && ( r1.z() > 0.0 || r2.z() > 0.0 ) )
    {
        CoverType type = m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 );

        createBorder( m_cover[ type ].vtn, r1, r2, r3, r4 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createBorderE( int jx, int jy )
{
    osg::Vec3d r1;
    osg::Vec3d r2;
    osg::Vec3d r3;
    osg::Vec3d r4;

    double lat1 = 0.0;
    double lat2 = 0.0;

    double lon1 = 0.0;
    double lon2 = 0.0;

    r1.x() = ( jx + 1 ) * m_step - m_offset;
    r1.y() = ( jy + 0 ) * m_step - m_offset;

    r2.x() = ( jx + 1 ) * m_step - m_offset;
    r2.y() = ( jy + 1 ) * m_step - m_offset;

    r3.x() = r1.x();
    r3.y() = r1.y();

    r4.x() = r2.x();
    r4.y() = r2.y();

    m_terrain->getGeo().cart2geo( getAbsX( r1.x() ), getAbsY( r1.y() ), lat1, lon1 );
    m_terrain->getGeo().cart2geo( getAbsX( r2.x() ), getAbsY( r2.y() ), lat2, lon2 );

    r1.z() = m_terrain->getElevation()->getElevation( lat1, lon1 );
    r2.z() = m_terrain->getElevation()->getElevation( lat2, lon2 );
    r3.z() = 0.0;
    r4.z() = 0.0;

    if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 )
      && ( r1.z() > 0.0 || r2.z() > 0.0 ) )
    {
        CoverType type = m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2 );

        createBorder( m_cover[ type ].vtn, r1, r2, r3, r4 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createBorder( VTN &vtn,
                         osg::Vec3d &r1,
                         osg::Vec3d &r2,
                         osg::Vec3d &r3,
                         osg::Vec3d &r4 )
{
    vtn.v->push_back( r1 );
    vtn.v->push_back( r2 );
    vtn.v->push_back( r3 );

    vtn.v->push_back( r2 );
    vtn.v->push_back( r3 );
    vtn.v->push_back( r4 );

    double tx1 = getTexCoord( r1.x(), Common::texL );
    double tx2 = getTexCoord( r2.x(), Common::texL );

    double ty1 = getTexCoord( r1.y(), Common::texL );
    double ty2 = getTexCoord( r2.y(), Common::texL );

    double dx = ty2 - ty1;
    double dy = tx2 - tx1;

    double tx3 = tx1 + dx;
    double tx4 = tx2 + dx;

    double ty3 = ty1 + dy;
    double ty4 = ty2 + dy;

    if      ( tx3 < 0.0f ) tx3 += 1.0f;
    else if ( tx3 > 1.0f ) tx3 -= 1.0f;

    if      ( tx4 < 0.0f ) tx4 += 1.0f;
    else if ( tx4 > 1.0f ) tx4 -= 1.0f;

    if      ( ty3 < 0.0f ) ty3 += 1.0f;
    else if ( ty3 > 1.0f ) ty3 -= 1.0f;

    if      ( ty4 < 0.0f ) ty4 += 1.0f;
    else if ( ty4 > 1.0f ) ty4 -= 1.0f;

    vtn.t->push_back( osg::Vec2(tx1,ty1) );
    vtn.t->push_back( osg::Vec2(tx2,ty2) );
    vtn.t->push_back( osg::Vec2(tx3,ty3) );

    vtn.t->push_back( osg::Vec2(tx2,ty2) );
    vtn.t->push_back( osg::Vec2(tx3,ty3) );
    vtn.t->push_back( osg::Vec2(tx4,ty4) );

    vtn.n->push_back( osg::Vec3( 0,0,1 ) );
    vtn.n->push_back( osg::Vec3( 0,0,1 ) );
    vtn.n->push_back( osg::Vec3( 0,0,1 ) );

    vtn.n->push_back( osg::Vec3( 0,0,1 ) );
    vtn.n->push_back( osg::Vec3( 0,0,1 ) );
    vtn.n->push_back( osg::Vec3( 0,0,1 ) );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createCover( bool even,
                        double lat1, double lon1,
                        double lat2, double lon2,
                        double lat3, double lon3,
                        double lat4, double lon4,
                        const osg::Vec3d &r1,
                        const osg::Vec3d &r2,
                        const osg::Vec3d &r3,
                        const osg::Vec3d &r4 )
{
    CoverType type1 = CoverGrass;
    CoverType type2 = CoverGrass;

    if ( even )
    {
        type1 = m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2, lat3, lon3 );
        type2 = m_terrain->getLandCover()->getType( lat2, lon2, lat3, lon3, lat4, lon4 );
    }
    else
    {
        type1 = m_terrain->getLandCover()->getType( lat1, lon1, lat2, lon2, lat4, lon4 );
        type2 = m_terrain->getLandCover()->getType( lat1, lon1, lat3, lon3, lat4, lon4 );
    }

    if ( type1 != CoverWater || type2 != CoverWater )
    {
        m_ground = true;
    }

    createSquare( even, m_cover[ type1 ].vtn, m_cover[ type2 ].vtn,
                  r1, r2, r3, r4, Common::texL, Common::texL );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createCoast( bool even,
                        double lat1, double lon1,
                        double lat2, double lon2,
                        double lat3, double lon3,
                        double lat4, double lon4,
                        const osg::Vec3d &r1,
                        const osg::Vec3d &r2,
                        const osg::Vec3d &r3,
                        const osg::Vec3d &r4,
                        int jx, int jy )
{
    osg::Vec3d rr1 = r1;
    osg::Vec3d rr2 = r2;
    osg::Vec3d rr3 = r3;
    osg::Vec3d rr4 = r4;

    int tileR = 7 - ( jy % 8 );
    int tileC = jx % 8;
    int tile = 8 * tileC + tileR;

    if      ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
            rr2.z() = 0.0;
            rr3.z() = 0.0;
            rr4.z() = 0.0;
        }

        createCover( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                     rr1, rr2, rr3, rr4 );
    }
    else if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr3.z() = 0.0;
            rr4.z() = 0.0;
        }

        createSquare( even,
                      m_coastRows[ CoastN ][ tileR ].vtn,
                      m_coastRows[ CoastN ][ tileR ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texL, Common::texS );
    }
    else if ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
            rr2.z() = 0.0;
        }

        createSquare( even,
                      m_coastRows[ CoastS ][ tileR ].vtn,
                      m_coastRows[ CoastS ][ tileR ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texL, Common::texS );
    }
    else if ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
            rr3.z() = 0.0;
        }

        createSquare( even,
                      m_coastCols[ CoastW ][ tileC ].vtn,
                      m_coastCols[ CoastW ][ tileC ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texL );
    }
    else if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr2.z() = 0.0;
            rr4.z() = 0.0;
        }

        createSquare( even,
                      m_coastCols[ CoastE ][ tileC ].vtn,
                      m_coastCols[ CoastE ][ tileC ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texL );
    }
    else if ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
            rr2.z() = 0.0;
            rr4.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast1NW ][ tile ].vtn,
                      m_coastSquares[ Coast1NW ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
            rr2.z() = 0.0;
            rr3.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast1NE ][ tile ].vtn,
                      m_coastSquares[ Coast1NE ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
            rr3.z() = 0.0;
            rr4.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast1SE ][ tile ].vtn,
                      m_coastSquares[ Coast1SE ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr2.z() = 0.0;
            rr3.z() = 0.0;
            rr4.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast1SW ][ tile ].vtn,
                      m_coastSquares[ Coast1SW ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr3.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast3NW ][ tile ].vtn,
                      m_coastSquares[ Coast3NW ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr4.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast3NE ][ tile ].vtn,
                      m_coastSquares[ Coast3NE ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr2.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast3SE ][ tile ].vtn,
                      m_coastSquares[ Coast3SE ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ Coast3SW ][ tile ].vtn,
                      m_coastSquares[ Coast3SW ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr1.z() = 0.0;
            rr4.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ CoastSENW ][ tile ].vtn,
                      m_coastSquares[ CoastSENW ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else if ( CoverWater != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverWater == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverWater == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverWater != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        if ( m_zeroizeElevation )
        {
            rr2.z() = 0.0;
            rr3.z() = 0.0;
        }

        createSquare( even,
                      m_coastSquares[ CoastNESW ][ tile ].vtn,
                      m_coastSquares[ CoastNESW ][ tile ].vtn,
                      rr1, rr2, rr3, rr4,
                      Common::texS, Common::texS );
    }
    else
    {
        createCover( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                     r1, r2, r3, r4 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createTrees( bool even,
                        double lat1, double lon1,
                        double lat2, double lon2,
                        double lat3, double lon3,
                        double lat4, double lon4,
                        const osg::Vec3d &r1,
                        const osg::Vec3d &r2,
                        const osg::Vec3d &r3,
                        const osg::Vec3d &r4,
                        int jx, int jy )
{
    int tileR = 7 - ( jy % 8 );
    int tileC = jx % 8;
    int tile = 8 * tileC + tileR;

    if      ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createCover( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                     r1, r2, r3, r4 );
    }
    else if ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesRows[ TreesN ][ tileR ].vtn,
                      m_treesRows[ TreesN ][ tileR ].vtn,
                      r1, r2, r3, r4,
                      Common::texL, Common::texS );
    }
    else if ( CoverTrees != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesRows[ TreesS ][ tileR ].vtn,
                      m_treesRows[ TreesS ][ tileR ].vtn,
                      r1, r2, r3, r4,
                      Common::texL, Common::texS );
    }
    else if ( CoverTrees != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesCols[ TreesW ][ tileC ].vtn,
                      m_treesCols[ TreesW ][ tileC ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texL );
    }
    else if ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesCols[ TreesE ][ tileC ].vtn,
                      m_treesCols[ TreesE ][ tileC ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texL );
    }
    else if ( CoverTrees != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees1NW ][ tile ].vtn,
                      m_treesSquares[ Trees1NW ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees1NE ][ tile ].vtn,
                      m_treesSquares[ Trees1NE ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees1SE ][ tile ].vtn,
                      m_treesSquares[ Trees1SE ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees1SW ][ tile ].vtn,
                      m_treesSquares[ Trees1SW ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees3NW ][ tile ].vtn,
                      m_treesSquares[ Trees3NW ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees3NE ][ tile ].vtn,
                      m_treesSquares[ Trees3NE ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees3SE ][ tile ].vtn,
                      m_treesSquares[ Trees3SE ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ Trees3SW ][ tile ].vtn,
                      m_treesSquares[ Trees3SW ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees != m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ TreesSENW ][ tile ].vtn,
                      m_treesSquares[ TreesSENW ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else if ( CoverTrees == m_terrain->getLandCover()->getType( lat1, lon1 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat2, lon2 )
           && CoverTrees != m_terrain->getLandCover()->getType( lat3, lon3 )
           && CoverTrees == m_terrain->getLandCover()->getType( lat4, lon4 ) )
    {
        createSquare( even,
                      m_treesSquares[ TreesNESW ][ tile ].vtn,
                      m_treesSquares[ TreesNESW ][ tile ].vtn,
                      r1, r2, r3, r4,
                      Common::texS, Common::texS );
    }
    else
    {
        createCover( even, lat1, lon1, lat2, lon2, lat3, lon3, lat4, lon4,
                     r1, r2, r3, r4 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createSquare( bool even,
                         VTN &vtn1, VTN &vtn2,
                         const osg::Vec3d &r1,
                         const osg::Vec3d &r2,
                         const osg::Vec3d &r3,
                         const osg::Vec3d &r4,
                         double texX, double texY )
{
    if ( even )
    {
        createTriangle1E( vtn1, r1, r2, r3, texX, texY );
        createTriangle2E( vtn2, r2, r3, r4, texX, texY );
    }
    else
    {
        createTriangle1O( vtn1, r1, r2, r4, texX, texY );
        createTriangle2O( vtn2, r1, r3, r4, texX, texY );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createTriangle1E( VTN &vtn,
                             const osg::Vec3d &r1,
                             const osg::Vec3d &r2,
                             const osg::Vec3d &r3,
                             double texX, double texY )
{
    vtn.v->push_back( r1 );
    vtn.v->push_back( r2 );
    vtn.v->push_back( r3 );

    double tx1 = getTexCoord( r1.x(), texX );
    double tx2 = getTexCoord( r2.x(), texX );
    double tx3 = getTexCoord( r3.x(), texX );

    if ( r2.x() > r1.x() && fabs( tx2 ) < 1.0e-3 ) tx2 = 1.0;

    double ty1 = getTexCoord( r1.y(), texY );
    double ty2 = getTexCoord( r2.y(), texY );
    double ty3 = getTexCoord( r3.y(), texY );

    if ( r3.y() > r1.y() && fabs( ty3 ) < 1.0e-3 ) ty3 = 1.0;

    //std::cout << tx2 << " ; " << ty2 << std::endl;

    vtn.t->push_back( osg::Vec2( tx1, ty1 ) );
    vtn.t->push_back( osg::Vec2( tx2, ty2 ) );
    vtn.t->push_back( osg::Vec2( tx3, ty3 ) );

    osg::Vec3d n1 = getNormal( r1 );
    osg::Vec3d n2 = getNormal( r2 );
    osg::Vec3d n3 = getNormal( r3 );

    vtn.n->push_back( n1 );
    vtn.n->push_back( n2 );
    vtn.n->push_back( n3 );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createTriangle1O( VTN &vtn,
                             const osg::Vec3d &r1,
                             const osg::Vec3d &r2,
                             const osg::Vec3d &r4,
                             double texX, double texY )
{
    vtn.v->push_back( r1 );
    vtn.v->push_back( r2 );
    vtn.v->push_back( r4 );

    double tx1 = getTexCoord( r1.x() , texX );
    double tx2 = getTexCoord( r2.x() , texX );
    double tx4 = getTexCoord( r4.x() , texX );

    if ( r2.x() > r1.x() && fabs( tx2 ) < 1.0e-3 ) tx2 = 1.0;
    if ( r4.x() > r1.x() && fabs( tx4 ) < 1.0e-3 ) tx4 = 1.0;

    double ty1 = getTexCoord( r1.y() , texY );
    double ty2 = getTexCoord( r2.y() , texY );
    double ty4 = getTexCoord( r4.y() , texY );

    if ( r4.y() > r1.y() && fabs( ty4 ) < 1.0e-3 ) ty4 = 1.0;

    //std::cout << "(" << tx1 << "," << ty1 << ") (" << tx2 << "," << ty2 << ") (" << tx4 << "," << ty4 << ")" << std::endl;

    vtn.t->push_back( osg::Vec2( tx1, ty1 ) );
    vtn.t->push_back( osg::Vec2( tx2, ty2 ) );
    vtn.t->push_back( osg::Vec2( tx4, ty4 ) );

    osg::Vec3d n1 = getNormal( r1 );
    osg::Vec3d n2 = getNormal( r2 );
    osg::Vec3d n4 = getNormal( r4 );

    vtn.n->push_back( n1 );
    vtn.n->push_back( n2 );
    vtn.n->push_back( n4 );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createTriangle2E( VTN &vtn,
                             const osg::Vec3d &r2,
                             const osg::Vec3d &r3,
                             const osg::Vec3d &r4,
                             double texX, double texY )
{
    vtn.v->push_back( r3 );
    vtn.v->push_back( r2 );
    vtn.v->push_back( r4 );

    double tx3 = getTexCoord( r3.x(), texX );
    double tx2 = getTexCoord( r2.x(), texX );
    double tx4 = getTexCoord( r4.x(), texX );

    if ( r2.x() > r3.x() && fabs( tx2 ) < 1.0e-3 ) tx2 = 1.0;
    if ( r4.x() > r3.x() && fabs( tx4 ) < 1.0e-3 ) tx4 = 1.0;

    double ty3 = getTexCoord( r3.y(), texY );
    double ty2 = getTexCoord( r2.y(), texY );
    double ty4 = getTexCoord( r4.y(), texY );

    if ( r3.y() > r2.y() && fabs( ty3 ) < 1.0e-3 ) ty3 = 1.0;
    if ( r4.y() > r2.y() && fabs( ty4 ) < 1.0e-3 ) ty4 = 1.0;

    //std::cout << "(" << tx3 << "," << ty3 << ") (" << tx2 << "," << ty2 << ") (" << tx4 << "," << ty4 << ")" << std::endl;

    vtn.t->push_back( osg::Vec2( tx3, ty3 ) );
    vtn.t->push_back( osg::Vec2( tx2, ty2 ) );
    vtn.t->push_back( osg::Vec2( tx4, ty4 ) );

    osg::Vec3d n2 = getNormal( r2 );
    osg::Vec3d n3 = getNormal( r3 );
    osg::Vec3d n4 = getNormal( r4 );

    vtn.n->push_back( n3 );
    vtn.n->push_back( n2 );
    vtn.n->push_back( n4 );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createTriangle2O( VTN &vtn,
                             const osg::Vec3d &r1,
                             const osg::Vec3d &r3,
                             const osg::Vec3d &r4,
                             double texX, double texY )
{
    vtn.v->push_back( r1 );
    vtn.v->push_back( r4 );
    vtn.v->push_back( r3 );

    double tx1 = getTexCoord( r1.x(), texX );
    double tx4 = getTexCoord( r4.x(), texX );
    double tx3 = getTexCoord( r3.x(), texX );

    if ( r4.x() > r1.x() && fabs( tx4 ) < 1.0e-3 ) tx4 = 1.0;

    double ty1 = getTexCoord( r1.y(), texY );
    double ty4 = getTexCoord( r4.y(), texY );
    double ty3 = getTexCoord( r3.y(), texY );

    if ( r3.y() > r1.y() && fabs( ty3 ) < 1.0e-3 ) ty3 = 1.0;
    if ( r4.y() > r1.y() && fabs( ty4 ) < 1.0e-3 ) ty4 = 1.0;

    //std::cout << "(" << tx1 << "," << ty1 << ") (" << tx4 << "," << ty4 << ") (" << tx3 << "," << ty3 << ")" << std::endl;

    vtn.t->push_back( osg::Vec2( tx1, ty1 ) );
    vtn.t->push_back( osg::Vec2( tx4, ty4 ) );
    vtn.t->push_back( osg::Vec2( tx3, ty3 ) );

    osg::Vec3d n1 = getNormal( r1 );
    osg::Vec3d n3 = getNormal( r3 );
    osg::Vec3d n4 = getNormal( r4 );

    vtn.n->push_back( n1 );
    vtn.n->push_back( n4 );
    vtn.n->push_back( n3 );
}

////////////////////////////////////////////////////////////////////////////////

osg::Vec3d Tile::getNormal( const osg::Vec3d &r )
{
    osg::Vec3d n( 0.0, 0.0, 1.0 );

    if ( m_terrain != 0 )
    {
        double lat0 = 0.0;
        double lon0 = 0.0;

        m_terrain->getGeo().cart2geo( getAbsX( r.x() ), getAbsY( r.y() ), lat0, lon0 );

        if ( CoverWater == m_terrain->getLandCover()->getType( lat0, lon0 ) )
        {
            return osg::Vec3d( 0.0, 0.0, 1.0 );
        }

        osg::Vec3d r1;
        osg::Vec3d r2;
        osg::Vec3d r3;
        osg::Vec3d r4;
        osg::Vec3d r5;
        osg::Vec3d r6;
        osg::Vec3d r7;
        osg::Vec3d r8;

        double lat1 = 0.0;
        double lat2 = 0.0;
        double lat3 = 0.0;
        double lat4 = 0.0;
        double lat5 = 0.0;
        double lat6 = 0.0;
        double lat7 = 0.0;
        double lat8 = 0.0;

        double lon1 = 0.0;
        double lon2 = 0.0;
        double lon3 = 0.0;
        double lon4 = 0.0;
        double lon5 = 0.0;
        double lon6 = 0.0;
        double lon7 = 0.0;
        double lon8 = 0.0;

        r1.x() = r.x() + m_step;
        r1.y() = r.y();

        r2.x() = r.x() + m_step;
        r2.y() = r.y() + m_step;

        r3.x() = r.x();
        r3.y() = r.y() + m_step;

        r4.x() = r.x() - m_step;
        r4.y() = r.y() + m_step;

        r5.x() = r.x() - m_step;
        r5.y() = r.y();

        r6.x() = r.x() - m_step;
        r6.y() = r.y() - m_step;

        r7.x() = r.x();
        r7.y() = r.y() - m_step;

        r8.x() = r.x() + m_step;
        r8.y() = r.y() - m_step;

        m_terrain->getGeo().cart2geo( getAbsX( r1.x() ), getAbsY( r1.y() ), lat1, lon1 );
        m_terrain->getGeo().cart2geo( getAbsX( r2.x() ), getAbsY( r2.y() ), lat2, lon2 );
        m_terrain->getGeo().cart2geo( getAbsX( r3.x() ), getAbsY( r3.y() ), lat3, lon3 );
        m_terrain->getGeo().cart2geo( getAbsX( r4.x() ), getAbsY( r4.y() ), lat4, lon4 );
        m_terrain->getGeo().cart2geo( getAbsX( r5.x() ), getAbsY( r5.y() ), lat5, lon5 );
        m_terrain->getGeo().cart2geo( getAbsX( r6.x() ), getAbsY( r6.y() ), lat6, lon6 );
        m_terrain->getGeo().cart2geo( getAbsX( r7.x() ), getAbsY( r7.y() ), lat7, lon7 );
        m_terrain->getGeo().cart2geo( getAbsX( r8.x() ), getAbsY( r8.y() ), lat8, lon8 );

        r1.z() = m_terrain->getElevation()->getElevation( lat1, lon1 );
        r2.z() = m_terrain->getElevation()->getElevation( lat2, lon2 );
        r3.z() = m_terrain->getElevation()->getElevation( lat3, lon3 );
        r4.z() = m_terrain->getElevation()->getElevation( lat4, lon4 );
        r5.z() = m_terrain->getElevation()->getElevation( lat5, lon5 );
        r6.z() = m_terrain->getElevation()->getElevation( lat6, lon6 );
        r7.z() = m_terrain->getElevation()->getElevation( lat7, lon7 );
        r8.z() = m_terrain->getElevation()->getElevation( lat8, lon8 );

        osg::Vec3d v1 = r1 - r;
        osg::Vec3d v2 = r2 - r;
        osg::Vec3d v3 = r3 - r;
        osg::Vec3d v4 = r4 - r;
        osg::Vec3d v5 = r5 - r;
        osg::Vec3d v6 = r6 - r;
        osg::Vec3d v7 = r7 - r;
        osg::Vec3d v8 = r8 - r;

        osg::Vec3d n1 = v1 ^ v2;
        osg::Vec3d n2 = v2 ^ v3;
        osg::Vec3d n3 = v3 ^ v4;
        osg::Vec3d n4 = v4 ^ v5;
        osg::Vec3d n5 = v5 ^ v6;
        osg::Vec3d n6 = v6 ^ v7;
        osg::Vec3d n7 = v7 ^ v8;
        osg::Vec3d n8 = v8 ^ v1;

        if ( n1.isNaN() ) n1.set( 0.0, 0.0, 1.0 );
        if ( n2.isNaN() ) n2.set( 0.0, 0.0, 1.0 );
        if ( n3.isNaN() ) n3.set( 0.0, 0.0, 1.0 );
        if ( n4.isNaN() ) n4.set( 0.0, 0.0, 1.0 );
        if ( n5.isNaN() ) n5.set( 0.0, 0.0, 1.0 );
        if ( n6.isNaN() ) n6.set( 0.0, 0.0, 1.0 );
        if ( n7.isNaN() ) n7.set( 0.0, 0.0, 1.0 );
        if ( n8.isNaN() ) n8.set( 0.0, 0.0, 1.0 );

        n1 *= 1.0/n1.length();
        n2 *= 1.0/n2.length();
        n3 *= 1.0/n3.length();
        n4 *= 1.0/n4.length();
        n5 *= 1.0/n5.length();
        n6 *= 1.0/n6.length();
        n7 *= 1.0/n7.length();
        n8 *= 1.0/n8.length();

        n = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8;
        n *= 1.0/n.length();

        if ( n.isNaN() ) n.set( 0.0, 0.0, 1.0 );
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////////

double Tile::getTexCoord( double val, double textureSize )
{
//    if ( m_ocean )
//    {
//        return ( val + m_offset ) / textureSize;
//    }

    return (double)( (int)( val + m_offset ) % (int)textureSize ) / textureSize;
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createGeom()
{
    for ( int i = 0; i < TILE_MAX_COVER_TYPES; i++ )
    {
        createGeom( m_cover[ i ] );
    }

    createGeom< TILE_MAX_COAST_SQUARES_TYPES , 64 >( m_coastSquares );
    createGeom< TILE_MAX_COAST_ROWS_TYPES    , 8  >( m_coastRows );
    createGeom< TILE_MAX_COAST_COLS_TYPES    , 8  >( m_coastCols );

    createGeom< TILE_MAX_TREES_SQUARES_TYPES , 64 >( m_treesSquares );
    createGeom< TILE_MAX_TREES_ROWS_TYPES    , 8  >( m_treesRows );
    createGeom< TILE_MAX_TREES_COLS_TYPES    , 8  >( m_treesCols );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::createGeom( Data &data )
{
    data.geometry->setNormalArray( data.vtn.n );
    data.geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
    data.geometry->setVertexArray( data.vtn.v );
    data.geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, data.vtn.v->size() ) );
    data.geometry->setTexCoordArray( 0, data.vtn.t );
}

////////////////////////////////////////////////////////////////////////////////

void Tile::addToGeode()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    if ( m_ground || !Common::getInstance()->isOceanValid( m_level ) )
    {
        for ( int i = 0; i < TILE_MAX_COVER_TYPES; i++ )
        {
            if ( m_cover[ i ].vtn.v->size() > 0 )
            {
                geode->addDrawable( m_cover[ i ].geometry.get() );
            }
        }

        addToGeode< TILE_MAX_COAST_SQUARES_TYPES , 64 >( geode.get(), m_coastSquares );
        addToGeode< TILE_MAX_COAST_ROWS_TYPES    , 8  >( geode.get(), m_coastRows );
        addToGeode< TILE_MAX_COAST_COLS_TYPES    , 8  >( geode.get(), m_coastCols );

        addToGeode< TILE_MAX_TREES_SQUARES_TYPES , 64 >( geode.get(), m_treesSquares );
        addToGeode< TILE_MAX_TREES_ROWS_TYPES    , 8  >( geode.get(), m_treesRows );
        addToGeode< TILE_MAX_TREES_COLS_TYPES    , 8  >( geode.get(), m_treesCols );

        // optimization
        int options =
                osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS |
                osgUtil::Optimizer::REMOVE_REDUNDANT_NODES |
                osgUtil::Optimizer::REMOVE_LOADED_PROXY_NODES |
                osgUtil::Optimizer::COMBINE_ADJACENT_LODS |
                osgUtil::Optimizer::SHARE_DUPLICATE_STATE |
                osgUtil::Optimizer::MERGE_GEOMETRY |
                osgUtil::Optimizer::CHECK_GEOMETRY |
                osgUtil::Optimizer::MAKE_FAST_GEOMETRY |
                osgUtil::Optimizer::SPATIALIZE_GROUPS |
                osgUtil::Optimizer::COPY_SHARED_NODES |
                osgUtil::Optimizer::TRISTRIP_GEOMETRY |
                osgUtil::Optimizer::TESSELLATE_GEOMETRY |
                osgUtil::Optimizer::MERGE_GEODES |
                osgUtil::Optimizer::FLATTEN_BILLBOARDS |
                osgUtil::Optimizer::STATIC_OBJECT_DETECTION |
                osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS_DUPLICATING_SHARED_SUBGRAPHS |
                osgUtil::Optimizer::INDEX_MESH |
                osgUtil::Optimizer::VERTEX_POSTTRANSFORM;

        osgUtil::Optimizer optimizer;
        optimizer.optimize( geode, options );

        if ( !m_ground )
        {
            Common::getInstance()->setOcean( m_level, geode.get() );
        }
    }
    else
    {
        geode = Common::getInstance()->getOcean( m_level );
    }

    if ( m_lod )
    {
        m_rootLOD->addChild( geode.get(), m_range1, m_range2 );
    }
    else
    {
        m_root->addChild( geode.get() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tile::generateSubtiles()
{
    osg::ref_ptr<osg::Group> subtiles;

    if ( m_lod )
    {
        subtiles = new osg::Group();
        m_rootLOD->addChild( subtiles.get(), m_range0, m_range1 );
    }
    else
    {
        subtiles = m_root;
    }

    for ( int ix = 0; ix < 2; ix++ )
    {
        for ( int iy = 0; iy < 2; iy++ )
        {
            double dx = -m_size / 4.0 + (double)ix * m_size / 2.0;
            double dy = -m_size / 4.0 + (double)iy * m_size / 2.0;

            osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
            subtiles->addChild( pat.get() );

            pat->setPosition( osg::Vec3d( dx, dy, 0.0 ) );

            Markers::createMarker( pat.get(), (Level)( m_level - 1 ) );

            Tile tile( m_terrain, dx, dy, (Level)( m_level - 1 ), this );
            pat->addChild( tile.getRoot() );
        }
    }
}
