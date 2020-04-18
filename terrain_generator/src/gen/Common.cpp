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

#include <math.h>

#include <QString>

#include <gen/Common.h>
#include <gen/Textures.h>
#include <gen/Tile.h>

////////////////////////////////////////////////////////////////////////////////

const double Common::horizon = 10000.0;                             // 10 km

const double Common::stepH = 256.0;                                 // 256
const double Common::stepM = 2.0 * Common::stepH;                   // 2 * 256 = 512
const double Common::stepL = 2.0 * Common::stepM;                   // 2 * 512 = 1024

const double Common::texS = Common::stepH;                          // 256
const double Common::texL = 8.0 * Common::texS;                     // 8 * 256 = 2048

const double Common::tileH = 2.0 * Common::texL;                    // 2 *  2048 =  4096 (not used)
const double Common::tileM = 2.0 * Common::tileH;                   // 2 *  4096 =  8192
const double Common::tileL = 2.0 * Common::tileM;                   // 2 *  8192 = 16384
const double Common::block = 2.0 * Common::tileL;                   // 2 * 16384 = 32768

const double Common::radiusH = 0.5 * M_SQRT2 * Common::tileH;       // 0.5 * sqrt(2) *  4096 =  2889
const double Common::radiusM = 0.5 * M_SQRT2 * Common::tileM;       // 0.5 * sqrt(2) *  8192 =  5775
const double Common::radiusL = 0.5 * M_SQRT2 * Common::tileL;       // 0.5 * sqrt(2) * 16384 = 11551
const double Common::radiusB = 0.5 * M_SQRT2 * Common::block;       // 0.5 * sqrt(2) * 32768 = 23101

////////////////////////////////////////////////////////////////////////////////

const float Common::maxAnisotropy = 1.0f;

////////////////////////////////////////////////////////////////////////////////

const std::string Common::texturesDir = "data/textures/";

////////////////////////////////////////////////////////////////////////////////

const std::string Common::coverTextures[] =
{
    "water",
    "grass",
    "crops",
    "trees",
    "urban",
};

////////////////////////////////////////////////////////////////////////////////

const std::string Common::coastTexturesSquares[] =
{
    "coast_1nw",
    "coast_1ne",
    "coast_1se",
    "coast_1sw",

    "coast_3nw",
    "coast_3ne",
    "coast_3se",
    "coast_3sw",

    "coast_nesw",
    "coast_senw"
};

////////////////////////////////////////////////////////////////////////////////

const std::string Common::coastTexturesRows[] =
{
    "coast_2n",
    "coast_2s"
};

////////////////////////////////////////////////////////////////////////////////

const std::string Common::coastTexturesCols[] =
{
    "coast_2e",
    "coast_2w"
};

////////////////////////////////////////////////////////////////////////////////

const std::string Common::treesTexturesSquares[] =
{
    "trees_1nw",
    "trees_1ne",
    "trees_1se",
    "trees_1sw",

    "trees_3nw",
    "trees_3ne",
    "trees_3se",
    "trees_3sw",

    "trees_nesw",
    "trees_senw"
};

////////////////////////////////////////////////////////////////////////////////

const std::string Common::treesTexturesRows[] =
{
    "trees_2n",
    "trees_2s"
};

////////////////////////////////////////////////////////////////////////////////

const std::string Common::treesTexturesCols[] =
{
    "trees_2e",
    "trees_2w"
};

////////////////////////////////////////////////////////////////////////////////

Common::Common() :
    m_inited ( false )
{}

////////////////////////////////////////////////////////////////////////////////

void Common::init()
{
    if ( !m_inited )
    {
        m_inited = true;

        m_material = new osg::Material();

        //m_material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
        m_material->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
        m_material->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );

        m_material->setShininess( osg::Material::FRONT_AND_BACK, 50.0f );

        for ( int i = 0; i < TILE_MAX_COVER_TYPES; i++ )
        {
            for ( int j = 0; j < TILE_MAX_LEVELS; j++ )
            {
                m_stateCover[ i ][ j ] = new osg::StateSet();

                std::string textureFile = getCoverTexture( (CoverType)i, (Level)j );

                m_stateCover[ i ][ j ]->setAttribute( m_material.get() );
                m_stateCover[ i ][ j ]->setTextureAttributeAndModes( 0, Textures::get( textureFile, true ), osg::StateAttribute::ON );
            }
        }

        for ( int i = 0; i < TILE_MAX_COAST_SQUARES_TYPES; i++ )
        {
            for ( int j = 0; j < 64; j++ )
            {
                m_stateCoastSquares[ i ][ j ] = new osg::StateSet();

                std::string textureFile = getCoastTextureSquares( (CoastTypeSquares)i, j );

                m_stateCoastSquares[ i ][ j ]->setAttribute( m_material.get() );
                m_stateCoastSquares[ i ][ j ]->setTextureAttributeAndModes( 0, Textures::get( textureFile ), osg::StateAttribute::ON );
            }
        }

        for ( int i = 0; i < TILE_MAX_COAST_ROWS_TYPES; i++ )
        {
            for ( int j = 0; j < 8; j++ )
            {
                m_stateCoastRows[ i ][ j ] = new osg::StateSet();

                std::string textureFile = getCoastTextureRows( (CoastTypeRows)i, j );

                m_stateCoastRows[ i ][ j ]->setAttribute( m_material.get() );
                m_stateCoastRows[ i ][ j ]->setTextureAttributeAndModes( 0, Textures::get( textureFile ), osg::StateAttribute::ON );
            }
        }

        for ( int i = 0; i < TILE_MAX_COAST_COLS_TYPES; i++ )
        {
            for ( int j = 0; j < 8; j++ )
            {
                m_stateCoastCols[ i ][ j ] = new osg::StateSet();

                std::string textureFile = getCoastTextureCols( (CoastTypeCols)i, j );

                m_stateCoastCols[ i ][ j ]->setAttribute( m_material.get() );
                m_stateCoastCols[ i ][ j ]->setTextureAttributeAndModes( 0, Textures::get( textureFile ), osg::StateAttribute::ON );
            }
        }

        for ( int i = 0; i < TILE_MAX_TREES_SQUARES_TYPES; i++ )
        {
            for ( int j = 0; j < 64; j++ )
            {
                m_stateTreesSquares[ i ][ j ] = new osg::StateSet();

                std::string textureFile = getTreesTextureSquares( (TreesTypeSquares)i, j );

                m_stateTreesSquares[ i ][ j ]->setAttribute( m_material.get() );
                m_stateTreesSquares[ i ][ j ]->setTextureAttributeAndModes( 0, Textures::get( textureFile ), osg::StateAttribute::ON );
            }
        }

        for ( int i = 0; i < TILE_MAX_TREES_ROWS_TYPES; i++ )
        {
            for ( int j = 0; j < 8; j++ )
            {
                m_stateTreesRows[ i ][ j ] = new osg::StateSet();

                std::string textureFile = getTreesTextureRows( (TreesTypeRows)i, j );

                m_stateTreesRows[ i ][ j ]->setAttribute( m_material.get() );
                m_stateTreesRows[ i ][ j ]->setTextureAttributeAndModes( 0, Textures::get( textureFile ), osg::StateAttribute::ON );
            }
        }

        for ( int i = 0; i < TILE_MAX_TREES_COLS_TYPES; i++ )
        {
            for ( int j = 0; j < 8; j++ )
            {
                m_stateTreesCols[ i ][ j ] = new osg::StateSet();

                std::string textureFile = getTreesTextureCols( (TreesTypeCols)i, j );

                m_stateTreesCols[ i ][ j ]->setAttribute( m_material.get() );
                m_stateTreesCols[ i ][ j ]->setTextureAttributeAndModes( 0, Textures::get( textureFile ), osg::StateAttribute::ON );
            }
        }

        Tile tileL( LevelL );
        Tile tileM( LevelM );
        Tile tileH( LevelH );

//        setOcean( LevelL, tileL.getOcean() );
//        setOcean( LevelM, tileM.getOcean() );
//        setOcean( LevelH, tileH.getOcean() );
    }
}

////////////////////////////////////////////////////////////////////////////////

std::string Common::getCoverTexture( CoverType type, Level level )
{
    QString coastTexture = QString( texturesDir.c_str() )
            + QString( coverTextures[ type ].c_str() );

    switch ( level )
    {
        case LevelH: coastTexture += "_h"; break;
        case LevelM: coastTexture += "_m"; break;
        case LevelL: coastTexture += "_l"; break;
    }

    coastTexture += ".rgb";

    return coastTexture.toStdString();
}

////////////////////////////////////////////////////////////////////////////////

std::string Common::getCoastTextureSquares( CoastTypeSquares type, int tile )
{
    QString coastTexture = QString( texturesDir.c_str() )
            + QString( coastTexturesSquares[ type ].c_str() )
            + "_"
            + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
            + ".bmp";

    return coastTexture.toStdString();
}

////////////////////////////////////////////////////////////////////////////////

std::string Common::getCoastTextureRows( CoastTypeRows type, int tile )
{
    QString coastTexture = QString( texturesDir.c_str() )
            + QString( coastTexturesRows[ type ].c_str() )
            + "_"
            + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
            + ".bmp";

    return coastTexture.toStdString();
}

////////////////////////////////////////////////////////////////////////////////

std::string Common::getCoastTextureCols( CoastTypeCols type, int tile )
{
    QString coastTexture = QString( texturesDir.c_str() )
            + QString( coastTexturesCols[ type ].c_str() )
            + "_"
            + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
            + ".bmp";

    return coastTexture.toStdString();
}

////////////////////////////////////////////////////////////////////////////////

std::string Common::getTreesTextureSquares( TreesTypeSquares type, int tile )
{
    QString coastTexture = QString( texturesDir.c_str() )
            + QString( treesTexturesSquares[ type ].c_str() )
            + "_"
            + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
            + ".bmp";

    return coastTexture.toStdString();
}

////////////////////////////////////////////////////////////////////////////////

std::string Common::getTreesTextureRows( TreesTypeRows type, int tile )
{
    QString coastTexture = QString( texturesDir.c_str() )
            + QString( treesTexturesRows[ type ].c_str() )
            + "_"
            + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
            + ".bmp";

    return coastTexture.toStdString();
}

////////////////////////////////////////////////////////////////////////////////

std::string Common::getTreesTextureCols( TreesTypeCols type, int tile )
{
    QString coastTexture = QString( texturesDir.c_str() )
            + QString( treesTexturesCols[ type ].c_str() )
            + "_"
            + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
            + ".bmp";

    return coastTexture.toStdString();
}
