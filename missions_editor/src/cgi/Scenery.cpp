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

#include <cgi/Scenery.h>
#include <cgi/VisitorLOD.h>

#include <editor/Mission.h>

#include <osgDB/ReadFile>

////////////////////////////////////////////////////////////////////////////////

Scenery::Scenery()
{
    m_root = new osg::Group();
}

////////////////////////////////////////////////////////////////////////////////

Scenery::~Scenery() {}

////////////////////////////////////////////////////////////////////////////////

void Scenery::reloadScenery()
{
    if ( m_root.valid() )
    {
        if ( m_root->getNumChildren() > 0 )
        {
            m_root->removeChildren( 0, m_root->getNumChildren() );
        }
    }

    std::string terrainFile = "data/" + Mission::getInstance()->getTerrainFile();

    osg::ref_ptr<osg::Node> terrainNode = osgDB::readNodeFile( terrainFile );

    if ( terrainNode.valid() )
    {
        VisitorLOD visitorLOD;
        terrainNode->accept( visitorLOD );

        m_root->addChild( terrainNode.get() );

        // objects
        for ( Mission::ObjectFiles::iterator it = Mission::getInstance()->getObjects().begin();
              it != Mission::getInstance()->getObjects().end(); ++it )
        {
            std::string objectFile = "data/" + *it;

            osg::ref_ptr<osg::Node> object = osgDB::readNodeFile( objectFile );

            if ( object.valid() )
            {
                VisitorLOD visitorLOD( true );
                object->accept( visitorLOD );

                m_root->addChild( object.get() );
            }
            else
            {
                std::cerr << "Error reading file: " << objectFile << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error reading file: " << terrainFile << std::endl;
    }
}
