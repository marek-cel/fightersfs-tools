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

#include <cgi/Units.h>
#include <cgi/VisitorLOD.h>

#include <cgi/Models.h>
#include <cgi/Textures.h>

#include <editor/Mission.h>

#include <osgDB/ReadFile>

////////////////////////////////////////////////////////////////////////////////

Units::Units()
{
    m_root = new osg::Group();

    m_pats.clear();
}

////////////////////////////////////////////////////////////////////////////////

Units::~Units() {}

////////////////////////////////////////////////////////////////////////////////

void Units::reloadUnits()
{
    m_pats.clear();

    if ( m_root.valid() )
    {
        if ( m_root->getNumChildren() > 0 )
        {
            m_root->removeChildren( 0, m_root->getNumChildren() );
        }
    }

    for ( int i = 0; i < (int)Mission::getInstance()->getUnits().size(); i++ )
    {
        Unit *unit = Mission::getInstance()->getUnits().at( i );

        if ( unit )
        {
            osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
            m_root->addChild( pat.get() );

            pat->setPosition( unit->getPosition() );
            pat->setAttitude( unit->getAttitude() );

            //std::cout << unit->getModel() << std::endl;

            osg::ref_ptr<osg::Node> unitNode = osgDB::readNodeFile( "data/" + unit->getModel() );

            if ( unitNode.valid() )
            {
                VisitorLOD visitorLOD( true );
                unitNode->accept( visitorLOD );

                pat->addChild( unitNode.get() );

                m_pats.push_back( pat.get() );

                if ( Unit::isAircraft( unit->getType() ) )
                {
                    std::string textureFile = "data/";
                    textureFile += unit->getLivery();

                    osg::ref_ptr<osg::Texture2D> texture = Textures::get( textureFile, 8.0f );

                    osg::ref_ptr<osg::StateSet> stateSet = unitNode->getOrCreateStateSet();

                    if ( texture.valid() )
                    {
                        stateSet->setTextureAttributeAndModes( 0, texture.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Units::setScale( double coef )
{
    osg::Vec3d scale( coef, coef, coef );

    for ( PATS::iterator it = m_pats.begin(); it != m_pats.end(); it++ )
    {
        (*it)->setScale( scale );
    }
}
