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
#ifndef COMMON_H
#define COMMON_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <osg/Geode>
#include <osg/Material>
#include <osg/StateSet>

#include "Singleton.h"
#include "Types.h"

////////////////////////////////////////////////////////////////////////////////

/** Common. */
class Common : public Singleton< Common >
{
    friend class Singleton< Common >;

public:

    static const double horizon;        ///< [m] distance to the horizon

    static const double stepH;          ///< [m] step size (high details)
    static const double stepM;          ///< [m] step size (medium details)
    static const double stepL;          ///< [m] step size (low details)

    static const double texS;           ///< [m] small texture size
    static const double texL;           ///< [m] large texture size

    static const double tileH;          ///< [m] tile size (high details)
    static const double tileM;          ///< [m] tile size (medium details)
    static const double tileL;          ///< [m] tile size (low details)

    static const double block;          ///< [m] size of low details tiles blosk

    static const double radiusH;        ///< [m] tile radius (high details)
    static const double radiusM;        ///< [m] tile radius (medium details)
    static const double radiusL;        ///< [m] tile radius (low details)
    static const double radiusB;        ///< [m] block radius

    static const float maxAnisotropy;   ///<

    static const std::string texturesDir;

    static const std::string coverTextures[ TILE_MAX_COVER_TYPES ];

    static const std::string coastTexturesSquares[ TILE_MAX_COAST_SQUARES_TYPES ];
    static const std::string coastTexturesRows[ TILE_MAX_COAST_ROWS_TYPES ];
    static const std::string coastTexturesCols[ TILE_MAX_COAST_COLS_TYPES ];

    static const std::string treesTexturesSquares[ TILE_MAX_TREES_SQUARES_TYPES ];
    static const std::string treesTexturesRows[ TILE_MAX_TREES_ROWS_TYPES ];
    static const std::string treesTexturesCols[ TILE_MAX_TREES_COLS_TYPES ];

private:

    /**
     * You should use static function getInstance() due to get refernce
     * to Common class instance.
     */
    Common();

    /** Using this constructor is forbidden. */
    Common( const Common & ) {}

public:

    void init();

    inline osg::Material* getMaterial() { return m_material.get(); }

    inline osg::Geode* getOcean( Level level )
    {
        if ( level == LevelH )
            return m_oceanH.get();
        else if ( level == LevelM )
            return m_oceanM.get();

        return m_oceanL.get();
    }

    inline osg::StateSet* getCoverStateSet( CoverType i, Level j )
    {
        return m_stateCover[ i ][ j ].get();
    }

    inline osg::StateSet* getCoastStateSetSquares( unsigned int i, unsigned int j )
    {
        return m_stateCoastSquares[ i ][ j ].get();
    }

    inline osg::StateSet* getCoastStateSetRows( unsigned int i, unsigned int j )
    {
        return m_stateCoastRows[ i ][ j ].get();
    }

    inline osg::StateSet* getCoastStateSetCols( unsigned int i, unsigned int j )
    {
        return m_stateCoastCols[ i ][ j ].get();
    }

    inline osg::StateSet* getTreesStateSetSquares( unsigned int i, unsigned int j )
    {
        return m_stateTreesSquares[ i ][ j ].get();
    }

    inline osg::StateSet* getTreesStateSetRows( unsigned int i, unsigned int j )
    {
        return m_stateTreesRows[ i ][ j ].get();
    }

    inline osg::StateSet* getTreesStateSetCols( unsigned int i, unsigned int j )
    {
        return m_stateTreesCols[ i ][ j ].get();
    }

    inline bool isOceanValid( Level level )
    {
        if ( level == LevelH )
            return m_oceanH.valid();
        else if ( level == LevelM )
            return m_oceanM.valid();

        return m_oceanL.valid();
    }

    inline void setOcean( Level level, osg::Geode *ocean )
    {
        if ( level == LevelH )
        {
            m_oceanH = ocean;
            osg::notify(osg::ALWAYS) << "Ocean H set." << std::endl;
        }
        else if ( level == LevelM )
        {
            m_oceanM = ocean;
            osg::notify(osg::ALWAYS) << "Ocean M set." << std::endl;
        }
        else if ( level == LevelL )
        {
            m_oceanL = ocean;
            osg::notify(osg::ALWAYS) << "Ocean L set." << std::endl;
        }
    }

private:

    bool m_inited;

    osg::ref_ptr<osg::Geode> m_oceanH;       ///<
    osg::ref_ptr<osg::Geode> m_oceanM;       ///<
    osg::ref_ptr<osg::Geode> m_oceanL;       ///<

    osg::ref_ptr<osg::Material> m_material;  ///<

    osg::ref_ptr<osg::StateSet> m_stateCover[ TILE_MAX_COVER_TYPES ][ TILE_MAX_LEVELS ];

    osg::ref_ptr<osg::StateSet> m_stateCoastSquares[ TILE_MAX_COAST_SQUARES_TYPES ][ 64 ];
    osg::ref_ptr<osg::StateSet> m_stateCoastRows[ TILE_MAX_COAST_ROWS_TYPES ][ 8 ];
    osg::ref_ptr<osg::StateSet> m_stateCoastCols[ TILE_MAX_COAST_COLS_TYPES ][ 8 ];

    osg::ref_ptr<osg::StateSet> m_stateTreesSquares[ TILE_MAX_TREES_SQUARES_TYPES ][ 64 ];
    osg::ref_ptr<osg::StateSet> m_stateTreesRows[ TILE_MAX_TREES_ROWS_TYPES ][ 8 ];
    osg::ref_ptr<osg::StateSet> m_stateTreesCols[ TILE_MAX_TREES_COLS_TYPES ][ 8 ];

    std::string getCoverTexture( CoverType type, Level level );

    std::string getCoastTextureSquares( CoastTypeSquares type, int tile );
    std::string getCoastTextureRows( CoastTypeRows type, int tile );
    std::string getCoastTextureCols( CoastTypeCols type, int tile );

    std::string getTreesTextureSquares( TreesTypeSquares type, int tile );
    std::string getTreesTextureRows( TreesTypeRows type, int tile );
    std::string getTreesTextureCols( TreesTypeCols type, int tile );
};

////////////////////////////////////////////////////////////////////////////////

#endif // COMMON_H
