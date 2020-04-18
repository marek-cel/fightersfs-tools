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
#ifndef TILE_H
#define TILE_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Array>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LOD>

#include "Common.h"

#include "Terrain.h"

////////////////////////////////////////////////////////////////////////////////

/** */
class Tile
{
public:

    struct VTN
    {
        osg::ref_ptr<osg::Vec3Array> v;
        osg::ref_ptr<osg::Vec2Array> t;
        osg::ref_ptr<osg::Vec3Array> n;
    };

    struct Data
    {
        VTN vtn;

        osg::ref_ptr<osg::Geometry> geometry;
    };

    static const bool m_zeroizeElevation;

    /** Constructor. */
    Tile( Level level );

    /** Constructor. */
    Tile( Terrain *terrain, double x0, double y0, Level level, Tile *parent = 0, bool lod = true );

    double getAbsX0() const;
    double getAbsY0() const;

    inline double getAbsX( double x ) const { return getAbsX0() + x; }
    inline double getAbsY( double y ) const { return getAbsY0() + y; }

    inline osg::Group* getRoot() { return m_root.get(); }

private:

    const bool m_ocean;

    const double m_x0;                  ///< [m] tile origin x-coordinate relative to the parent origin
    const double m_y0;                  ///< [m] tile origin y-coordinate relative to the parent origin

    const Level m_level;                ///< details level (0: highest details)

    const bool m_lod;                   ///<

    Terrain *m_terrain;                 ///<

    Tile *m_parent;                     ///< parent tile

    double m_step;                      ///< [m] step
    double m_size;                      ///< [m] size

    double m_radius;                    ///<

    double m_offset;                    ///< [m]

    double m_range0;                    ///< [m]
    double m_range1;                    ///< [m]
    double m_range2;                    ///< [m]

    unsigned short m_steps;             ///< number of steps per tile

    osg::ref_ptr<osg::Group> m_root;    ///<
    osg::ref_ptr<osg::LOD> m_rootLOD;

    bool m_ground;                      ///< specifies if tile is ground

    Data m_cover[ TILE_MAX_COVER_TYPES ];

    Data m_coastSquares[ TILE_MAX_COAST_SQUARES_TYPES ][ 64 ];
    Data m_coastRows[ TILE_MAX_COAST_ROWS_TYPES ][ 8 ];
    Data m_coastCols[ TILE_MAX_COAST_COLS_TYPES ][ 8 ];

    Data m_treesSquares[ TILE_MAX_TREES_SQUARES_TYPES ][ 64 ];
    Data m_treesRows[ TILE_MAX_TREES_ROWS_TYPES ][ 8 ];
    Data m_treesCols[ TILE_MAX_TREES_COLS_TYPES ][ 8 ];

    void init();

    void createData();
    void createData( Data &data, osg::StateSet *stateSet );

    void generate();

    void createSquare( int jx, int jy );

    void createBorderS( int jx, int jy );
    void createBorderN( int jx, int jy );
    void createBorderW( int jx, int jy );
    void createBorderE( int jx, int jy );

    void createBorder( VTN &vtn,
                       osg::Vec3d &r1,
                       osg::Vec3d &r2,
                       osg::Vec3d &r3,
                       osg::Vec3d &r4 );

    void createCover( bool even,
                      double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3,
                      double lat4, double lon4,
                      const osg::Vec3d &r1,
                      const osg::Vec3d &r2,
                      const osg::Vec3d &r3,
                      const osg::Vec3d &r4 );

    void createCoast( bool even,
                      double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3,
                      double lat4, double lon4,
                      const osg::Vec3d &r1,
                      const osg::Vec3d &r2,
                      const osg::Vec3d &r3,
                      const osg::Vec3d &r4,
                      int jx, int jy );

    void createTrees( bool even,
                      double lat1, double lon1,
                      double lat2, double lon2,
                      double lat3, double lon3,
                      double lat4, double lon4,
                      const osg::Vec3d &r1,
                      const osg::Vec3d &r2,
                      const osg::Vec3d &r3,
                      const osg::Vec3d &r4,
                      int jx, int jy );

    void createSquare( bool even,
                       VTN &vtn1, VTN &vtn2,
                       const osg::Vec3d &r1,
                       const osg::Vec3d &r2,
                       const osg::Vec3d &r3,
                       const osg::Vec3d &r4,
                       double texX, double texY );

    /** even */
    void createTriangle1E( VTN &vtn,
                           const osg::Vec3d &r1,
                           const osg::Vec3d &r2,
                           const osg::Vec3d &r3,
                           double texX, double texY );

    /** odd */
    void createTriangle1O( VTN &vtn,
                           const osg::Vec3d &r1,
                           const osg::Vec3d &r2,
                           const osg::Vec3d &r4,
                           double texX, double texY );

    /** even */
    void createTriangle2E( VTN &vtn,
                           const osg::Vec3d &r2,
                           const osg::Vec3d &r3,
                           const osg::Vec3d &r4,
                           double texX, double texY );

    /** odd */
    void createTriangle2O( VTN &vtn,
                           const osg::Vec3d &r1,
                           const osg::Vec3d &r3,
                           const osg::Vec3d &r4,
                           double texX, double texY );

    osg::Vec3d getNormal( const osg::Vec3d &r );

    double getTexCoord( double val, double textureSize );

    void createGeom();

    template <size_t i_max, size_t j_max>
    void createGeom( Data data[i_max][j_max] )
    {
        for ( size_t i = 0; i < i_max; i++ )
        {
            for ( size_t j = 0; j < j_max; j++ )
            {
                createGeom( data[ i ][ j ] );
            }
        }
    }

    void createGeom( Data &data );

    void addToGeode();

    template <size_t i_max, size_t j_max>
    void addToGeode( osg::Geode *geode, Data data[i_max][j_max] )
    {
        for ( size_t i = 0; i < i_max; i++ )
        {
            for ( size_t j = 0; j < j_max; j++ )
            {
                if ( data[ i ][ j ].vtn.v->size() > 0 )
                {
                    geode->addDrawable( data[ i ][ j ].geometry.get() );
                }
            }
        }
    }

    void generateSubtiles();
};

////////////////////////////////////////////////////////////////////////////////

#endif // TILE_H
