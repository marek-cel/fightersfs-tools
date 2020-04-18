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

#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

#include <QFileInfo>
#include <QTextStream>

#include <gen/Document.h>

////////////////////////////////////////////////////////////////////////////////

Document::Document() :
    m_terrain ( 0 )
{
    newEmpty();
}

////////////////////////////////////////////////////////////////////////////////

Document::Document( QString fileName ) :
    m_terrain ( 0 )
{
    readFile( fileName );
}

////////////////////////////////////////////////////////////////////////////////

Document::~Document()
{
    if ( m_terrain ) delete m_terrain;
    m_terrain = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Document::newEmpty()
{
    if ( m_terrain ) delete m_terrain;
    m_terrain = 0;

    m_terrain = new Terrain();
}

////////////////////////////////////////////////////////////////////////////////

bool Document::exportAs( QString fileName )
{
    osgUtil::Optimizer optimizer;

    int options = osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS |
                  osgUtil::Optimizer::REMOVE_REDUNDANT_NODES |
                  osgUtil::Optimizer::REMOVE_LOADED_PROXY_NODES |
                  osgUtil::Optimizer::COMBINE_ADJACENT_LODS |
                  osgUtil::Optimizer::SHARE_DUPLICATE_STATE |
                  osgUtil::Optimizer::MERGE_GEOMETRY |
                  osgUtil::Optimizer::MAKE_FAST_GEOMETRY |
                  osgUtil::Optimizer::CHECK_GEOMETRY |
                  osgUtil::Optimizer::OPTIMIZE_TEXTURE_SETTINGS |
                  osgUtil::Optimizer::STATIC_OBJECT_DETECTION;

    optimizer.optimize( m_terrain->getRoot(), options );

    osgDB::writeNodeFile( *( m_terrain->getRoot() ), fileName.toStdString() );

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Document::generateElevation( QString fileName )
{
    return m_terrain->generateElevation( fileName );
}

////////////////////////////////////////////////////////////////////////////////

bool Document::readFile( QString fileName )
{
    newEmpty();

    QFile devFile( fileName );

    if ( devFile.open( QFile::ReadOnly | QFile::Text ) )
    {
        QDomDocument doc;

        doc.setContent( &devFile, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "terrain" )
        {
            if ( m_terrain ) delete m_terrain;
            m_terrain = 0;

            m_terrain = new Terrain( &rootNode );

            return true;
        }

        devFile.close();
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Document::saveFile( QString fileName )
{
    QString fileTemp = fileName;

    if ( QFileInfo( fileTemp ).suffix() != QString( "xml" ) )
    {
        fileTemp += ".xml";
    }

    QFile devFile( fileTemp );

    if ( devFile.open( QFile::WriteOnly | QFile::Truncate | QFile::Text ) )
    {
        QTextStream out;
        out.setDevice( &devFile );
        out.setCodec("UTF-8");
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

        QDomDocument doc( "terrain" );

        doc.setContent( &devFile, false );

        QDomElement dataNode = doc.createElement( "terrain" );
        doc.appendChild( dataNode );

        m_terrain->save( &doc, &dataNode );

        out << doc.toString();

        devFile.close();

        return true;
    }

    return false;
}
