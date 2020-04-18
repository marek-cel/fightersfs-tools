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

#include <gen/Markers.h>

#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

#include <gen/Common.h>

////////////////////////////////////////////////////////////////////////////////

const bool Markers::m_enabled = false;

const float Markers::m_radius = 100.0f;

////////////////////////////////////////////////////////////////////////////////

void Markers::createMarker( osg::Group *parent, Level level )
{
    if ( m_enabled )
    {
        double size = Common::tileL;

        if ( level == LevelM )
            size = Common::tileM;
        else if ( level == LevelH )
            size = Common::tileM;

        double d = size / 2.0;

//        createMarker( parent, 0.0, 0.0 );

        createMarker( parent, -d, -d );
        createMarker( parent,  d, -d );
        createMarker( parent,  d,  d );
        createMarker( parent, -d,  d );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Markers::createMarker( osg::Group *parent, double x, double y )
{
    if ( m_enabled )
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        parent->addChild( geode.get() );

        osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable();
        shape->setShape( new osg::Sphere(osg::Vec3( x, y, 1000.0 ), m_radius ) );

        geode->addDrawable( shape.get() );
    }
}
