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

#include <iostream>

#include <QFileInfo>
#include <QPixmap>

#include <TexturesGenerator.h>

////////////////////////////////////////////////////////////////////////////////

TexturesGenerator::TexturesGenerator()
{
    m_texturesSquares.push_back( "data/textures/coast_1nw.rgb" );
    m_texturesSquares.push_back( "data/textures/coast_1ne.rgb" );
    m_texturesSquares.push_back( "data/textures/coast_1se.rgb" );
    m_texturesSquares.push_back( "data/textures/coast_1sw.rgb" );

    m_texturesSquares.push_back( "data/textures/coast_3nw.rgb" );
    m_texturesSquares.push_back( "data/textures/coast_3ne.rgb" );
    m_texturesSquares.push_back( "data/textures/coast_3se.rgb" );
    m_texturesSquares.push_back( "data/textures/coast_3sw.rgb" );

    m_texturesSquares.push_back( "data/textures/coast_nesw.rgb" );
    m_texturesSquares.push_back( "data/textures/coast_senw.rgb" );

    m_texturesRows.push_back( "data/textures/coast_2n.rgb" );
    m_texturesRows.push_back( "data/textures/coast_2s.rgb" );

    m_texturesCols.push_back( "data/textures/coast_2e.rgb" );
    m_texturesCols.push_back( "data/textures/coast_2w.rgb" );

    //

    m_texturesSquares.push_back( "data/textures/trees_1nw.rgb" );
    m_texturesSquares.push_back( "data/textures/trees_1ne.rgb" );
    m_texturesSquares.push_back( "data/textures/trees_1se.rgb" );
    m_texturesSquares.push_back( "data/textures/trees_1sw.rgb" );

    m_texturesSquares.push_back( "data/textures/trees_3nw.rgb" );
    m_texturesSquares.push_back( "data/textures/trees_3ne.rgb" );
    m_texturesSquares.push_back( "data/textures/trees_3se.rgb" );
    m_texturesSquares.push_back( "data/textures/trees_3sw.rgb" );

    m_texturesSquares.push_back( "data/textures/trees_nesw.rgb" );
    m_texturesSquares.push_back( "data/textures/trees_senw.rgb" );

    m_texturesRows.push_back( "data/textures/trees_2n.rgb" );
    m_texturesRows.push_back( "data/textures/trees_2s.rgb" );

    m_texturesCols.push_back( "data/textures/trees_2e.rgb" );
    m_texturesCols.push_back( "data/textures/trees_2w.rgb" );
}

////////////////////////////////////////////////////////////////////////////////

void TexturesGenerator::generateTextures()
{
    generateTexturesSquares();
    generateTexturesRows();
    generateTexturesCols();
}

////////////////////////////////////////////////////////////////////////////////

void TexturesGenerator::generateTexturesSquares()
{
    int step = 512 / 8;

    for ( int i = 0; i < m_texturesSquares.size(); i++ )
    {
        QFileInfo fileInfo( m_texturesSquares.at( i ) );

        QPixmap img( m_texturesSquares.at( i ) );

        for ( int ix = 0; ix < 8; ix++ )
        {
            for ( int iy = 0; iy < 8; iy++ )
            {
                int offset_x = ix * step;
                int offset_y = iy * step;

                int tile = 8 * ix + iy;

                QPixmap img_new = img.copy( offset_x, offset_y, step, step );

                QString name = fileInfo.absolutePath()
                        + "/temp/"
                        + fileInfo.baseName()
                        + "_"
                        + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
                        + ".bmp";

                if ( !img_new.save( name, "BMP" ) )
                {
                    std::cerr << "Error writing file: " + name.toStdString() << std::endl;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void TexturesGenerator::generateTexturesRows()
{
    int step = 512 / 8;

    for ( int i = 0; i < m_texturesRows.size(); i++ )
    {
        QFileInfo fileInfo( m_texturesRows.at( i ) );

        QPixmap img( m_texturesRows.at( i ) );

        for ( int i = 0; i < 8; i++ )
        {
            int offset_y = i * step;

            int tile = i;

            QPixmap img_new = img.copy( 0, offset_y, 512, step );

            QString name = fileInfo.absolutePath()
                    + "/temp/"
                    + fileInfo.baseName()
                    + "_"
                    + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
                    + ".bmp";

            if ( !img_new.save( name, "BMP" ) )
            {
                std::cerr << "Error writing file: " + name.toStdString() << std::endl;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void TexturesGenerator::generateTexturesCols()
{
    int step = 512 / 8;

    for ( int i = 0; i < m_texturesCols.size(); i++ )
    {
        QFileInfo fileInfo( m_texturesCols.at( i ) );

        QPixmap img( m_texturesCols.at( i ) );

        for ( int i = 0; i < 8; i++ )
        {
            int offset_x = i * step;

            int tile = i;

            QPixmap img_new = img.copy( offset_x, 0, step, 512 );

            QString name = fileInfo.absolutePath()
                    + "/temp/"
                    + fileInfo.baseName()
                    + "_"
                    + QString("%1").arg(tile, 2, 'd', 0, QChar('0'))
                    + ".bmp";

            if ( !img_new.save( name, "BMP" ) )
            {
                std::cerr << "Error writing file: " + name.toStdString() << std::endl;
            }
        }
    }
}
