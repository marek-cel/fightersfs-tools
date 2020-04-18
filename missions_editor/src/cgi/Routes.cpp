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

#include <cgi/Routes.h>
#include <cgi/Colors.h>

#include <osg/Billboard>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/PositionAttitudeTransform>

#include <osgText/Text>

#include <editor/Mission.h>

////////////////////////////////////////////////////////////////////////////////

Routes::Routes() :
    m_highlightIndex ( -1 )
{
    m_root = new osg::Group();
}

////////////////////////////////////////////////////////////////////////////////

Routes::~Routes() {}

////////////////////////////////////////////////////////////////////////////////

void Routes::reloadRoutes()
{
    if ( m_root.valid() )
    {
        if ( m_root->getNumChildren() > 0 )
        {
            m_root->removeChildren( 0, m_root->getNumChildren() );
        }
    }

    for ( int i = 0; i < (int)Mission::getInstance()->getRoutes().size(); i++ )
    {
        Route *route = Mission::getInstance()->getRoutes().at( i );

        if ( route )
        {
            osg::ref_ptr<osg::Geode> geode = new osg::Geode();
            m_root->addChild( geode.get() );

            osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

            osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
            osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors
            osg::ref_ptr<osg::Vec3Array> p = new osg::Vec3Array();

            n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

            if ( i == m_highlightIndex )
            {
                c->push_back( Colors::Cyan );
            }
            else
            {
                c->push_back( Colors::Grey );
            }

            for ( int j = 0; j < (int)route->size(); j++ )
            {
                osg::Vec3d point = (*route)[ j ].first;

                p->push_back( point );

                if ( i == m_highlightIndex )
                {
                    // number
                    {
                        osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
                        m_root->addChild( pat.get() );

                        pat->setPosition( point + osg::Vec3( 0.0, 0.0, 100.0 ) );

                        osg::ref_ptr<osg::Billboard> billboard = new osg::Billboard();
                        pat->addChild( billboard.get() );

                        billboard->setMode( osg::Billboard::POINT_ROT_EYE );
                        billboard->setNormal( osg::Vec3f( 0.0f, 0.0f, 1.0f ) );

                        osg::ref_ptr<osgText::Text> text = new osgText::Text();
                        billboard->addDrawable( text );

                        text->setColor( Colors::Black );
                        text->setCharacterSize( 100.0f );
                        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
                        text->setPosition( osg::Vec3( 0.0f, 0.0f, 0.0f ) );
                        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
                        text->setAlignment( osgText::Text::CENTER_CENTER );
                        text->setText( QString::number( j ).toStdString() );
                    }

                    // line
                    {
                        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
                        m_root->addChild( geode.get() );

                        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

                        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
                        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors
                        osg::ref_ptr<osg::Vec3Array> p = new osg::Vec3Array();

                        n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
                        c->push_back( Colors::Black );

                        p->push_back( point );
                        p->push_back( point + osg::Vec3( 0.0f, 0.0f, 50.0f ) );

                        geometry->setVertexArray( p.get() );
                        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, p->size() ) );

                        geometry->setNormalArray( n.get() );
                        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

                        geometry->setColorArray( c.get() );
                        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

                        geode->addDrawable( geometry.get() );

                        osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();

                        osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
                        lineWidth->setWidth( 3.0f );

                        stateSet->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
                        stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
                    }
                }
            }


            geometry->setVertexArray( p.get() );
            geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, p->size() ) );

            geometry->setNormalArray( n.get() );
            geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );

            geometry->setColorArray( c.get() );
            geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

            geode->addDrawable( geometry.get() );

            ////////////////////

            osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();

            osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
            lineWidth->setWidth( 3.0f );

            stateSet->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
            stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
        }
    }
}
