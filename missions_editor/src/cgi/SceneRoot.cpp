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

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LightSource>
#include <osg/LineWidth>

#include <cgi/Colors.h>
#include <cgi/SceneRoot.h>
#include <cgi/VisitorLOD.h>

#include <editor/Mission.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

SceneRoot::SceneRoot() :
    m_camera ( 0 ),
    m_routes ( 0 ),
    m_scenery ( 0 ),
    m_units ( 0 )
{
    m_root = new osg::Group();

    osg::ref_ptr<osg::StateSet> stateSet = m_root->getOrCreateStateSet();

    stateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON );
    stateSet->setMode( GL_LIGHTING       , osg::StateAttribute::ON );
    stateSet->setMode( GL_LIGHT0         , osg::StateAttribute::ON );
    stateSet->setMode( GL_BLEND          , osg::StateAttribute::ON );
    stateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

    m_camera = new Camera();
    m_routes = new Routes();
    m_scenery = new Scenery();
    m_units = new Units();

    m_groupRoute = new osg::Group();

    m_root->addChild( m_routes->getNode() );
    m_root->addChild( m_scenery->getNode() );
    m_root->addChild( m_units->getNode() );

    m_root->addChild( m_groupRoute.get() );

    createMarker();
    createSceneLight();
}

////////////////////////////////////////////////////////////////////////////////

SceneRoot::~SceneRoot()
{
    if ( m_camera ) delete m_camera;
    m_camera = 0;

    if ( m_scenery ) delete m_scenery;
    m_scenery = 0;

    if ( m_units ) delete m_units;
    m_units = 0;
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::reloadRoutes()
{
    m_routes->reloadRoutes();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::reloadScenery()
{
    m_scenery->reloadScenery();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::reloadUnits()
{
    m_units->reloadUnits();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::hideAll()
{
    hideMarker();
    hideRoute();
    hideUnit();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::hideMarker()
{
    m_switchMarker->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::showMarker( osg::Vec3d position )
{
    m_switchMarker->setAllChildrenOn();
    m_patMarker->setPosition( position );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::hideRoute()
{
    m_routes->setHighlightIndex( -1 );
    reloadRoutes();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::showRoute( int routeIndex )
{
    m_routes->setHighlightIndex( routeIndex );
    reloadRoutes();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::hideUnit()
{
    hideMarker();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::showUnit( int unitIndex )
{
    hideUnit();

    if ( unitIndex >= 0 && unitIndex < (int)Mission::getInstance()->getUnits().size() )
    {
        Unit *unit = Mission::getInstance()->getUnits()[ unitIndex ];

        if ( unit )
        {
            showRoute( Mission::getInstance()->getRouteIndex( unit->getRoute() ) );
            showMarker( unit->getPosition() );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::update()
{
    m_camera->update();

    if ( m_switchMarker->getValue( 0 ) )
    {
        float dist = ( m_patMarker->getPosition() - m_camera->getPosition() ).length();

        if ( dist > 5000.0f )
        {
            float coef = dist / 5000.0f;

            m_patMarker->setScale( osg::Vec3( coef, coef, coef ) );

            //m_units->setScale( 10.0 * coef );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::setViewOrbit()
{
    m_camera->setViewOrbit( m_switchMarker.get() );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::setViewWorld()
{
    m_camera->setViewWorld( m_scenery->getNode() );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::createMarker()
{
    m_patMarker = new osg::PositionAttitudeTransform();
    m_root->addChild( m_patMarker.get() );

    m_switchMarker = new osg::Switch();
    m_patMarker->addChild( m_switchMarker.get() );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_switchMarker->addChild( geode.get() );

    m_switchMarker->setAllChildrenOff();

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors
    osg::ref_ptr<osg::Vec3Array> p = new osg::Vec3Array();

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( Colors::Green );

    const float a = 50.0f;
    const float l = 25.0f;

    p->push_back( osg::Vec3(  a,      a,      a     ) );
    p->push_back( osg::Vec3(  a,      a - l,  a     ) );
    p->push_back( osg::Vec3(  a,      a,      a     ) );
    p->push_back( osg::Vec3(  a - l,  a,      a     ) );
    p->push_back( osg::Vec3(  a,      a,      a     ) );
    p->push_back( osg::Vec3(  a,      a,      a - l ) );
    p->push_back( osg::Vec3( -a,      a,      a     ) );
    p->push_back( osg::Vec3( -a,      a - l,  a     ) );
    p->push_back( osg::Vec3( -a,      a,      a     ) );
    p->push_back( osg::Vec3( -a + l,  a,      a     ) );
    p->push_back( osg::Vec3( -a,      a,      a     ) );
    p->push_back( osg::Vec3( -a,      a,      a - l ) );
    p->push_back( osg::Vec3(  a,     -a,      a     ) );
    p->push_back( osg::Vec3(  a,     -a + l,  a     ) );
    p->push_back( osg::Vec3(  a,     -a,      a     ) );
    p->push_back( osg::Vec3(  a - l, -a,      a     ) );
    p->push_back( osg::Vec3(  a,     -a,      a     ) );
    p->push_back( osg::Vec3(  a,     -a,      a - l ) );
    p->push_back( osg::Vec3( -a,     -a,      a     ) );
    p->push_back( osg::Vec3( -a,     -a + l,  a     ) );
    p->push_back( osg::Vec3( -a,     -a,      a     ) );
    p->push_back( osg::Vec3( -a + l, -a,      a     ) );
    p->push_back( osg::Vec3( -a,     -a,      a     ) );
    p->push_back( osg::Vec3( -a,     -a,      a - l ) );
    p->push_back( osg::Vec3(  a,      a,     -a     ) );
    p->push_back( osg::Vec3(  a,      a - l, -a     ) );
    p->push_back( osg::Vec3(  a,      a,     -a     ) );
    p->push_back( osg::Vec3(  a - l,  a,     -a     ) );
    p->push_back( osg::Vec3(  a,      a,     -a     ) );
    p->push_back( osg::Vec3(  a,      a,     -a + l ) );
    p->push_back( osg::Vec3( -a,      a,     -a     ) );
    p->push_back( osg::Vec3( -a,      a - l, -a     ) );
    p->push_back( osg::Vec3( -a,      a,     -a     ) );
    p->push_back( osg::Vec3( -a + l,  a,     -a     ) );
    p->push_back( osg::Vec3( -a,      a,     -a     ) );
    p->push_back( osg::Vec3( -a,      a,     -a + l ) );
    p->push_back( osg::Vec3(  a,     -a,     -a     ) );
    p->push_back( osg::Vec3(  a,     -a + l, -a     ) );
    p->push_back( osg::Vec3(  a,     -a,     -a     ) );
    p->push_back( osg::Vec3(  a - l, -a,     -a     ) );
    p->push_back( osg::Vec3(  a,     -a,     -a     ) );
    p->push_back( osg::Vec3(  a,     -a,     -a + l ) );
    p->push_back( osg::Vec3( -a,     -a,     -a     ) );
    p->push_back( osg::Vec3( -a,     -a + l, -a     ) );
    p->push_back( osg::Vec3( -a,     -a,     -a     ) );
    p->push_back( osg::Vec3( -a + l, -a,     -a     ) );
    p->push_back( osg::Vec3( -a,     -a,     -a     ) );
    p->push_back( osg::Vec3( -a,     -a,     -a + l ) );

//    p->push_back( osg::Vec3( -l, 0.0f, 0.0f ) );
//    p->push_back( osg::Vec3(  l, 0.0f, 0.0f ) );
//    p->push_back( osg::Vec3( 0.0f, -l, 0.0f ) );
//    p->push_back( osg::Vec3( 0.0f,  l, 0.0f ) );
//    p->push_back( osg::Vec3( 0.0f, 0.0f, -l ) );
//    p->push_back( osg::Vec3( 0.0f, 0.0f,  l ) );

    geometry->setVertexArray( p.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, p->size() ) );

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
    stateSet->setRenderBinDetails( 10, "RenderBin" );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE  );
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::createSceneLight()
{
    osg::ref_ptr<osg::PositionAttitudeTransform> patSun = new osg::PositionAttitudeTransform();
    m_root->addChild( patSun.get() );

    patSun->setAttitude( osg::Quat( -M_PI_4, osg::Z_AXIS, -M_PI_4, osg::Y_AXIS, 0.0, osg::X_AXIS ) );

    osg::ref_ptr<osg::LightSource> lightSourceSun = new osg::LightSource();
    patSun->addChild( lightSourceSun.get() );

    osg::ref_ptr<osg::Light> lightSun = new osg::Light();

    lightSun->setLightNum( 0 );

    lightSun->setPosition( osg::Vec4d( 10000.0f, 0.0f, 0.0f, 0.0f ) );

    osg::Vec4 lightColor( 255.0f/255.0f, 253.0f/255.0f, 250.0f/255.0f, 1.0f );

    lightSun->setAmbient( lightColor );
    lightSun->setDiffuse( lightColor );
    lightSun->setSpecular( lightColor );

    lightSun->setConstantAttenuation( 1.0 );

    lightSourceSun->setLight( lightSun.get() );

    lightSourceSun->setLocalStateSetModes( osg::StateAttribute::ON );
    lightSourceSun->setStateSetModes( *m_root->getOrCreateStateSet(), osg::StateAttribute::ON );
}


