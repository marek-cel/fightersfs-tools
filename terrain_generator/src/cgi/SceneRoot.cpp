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

#include <osg/ShapeDrawable>
#include <osg/LightSource>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <cgi/SceneRoot.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

SceneRoot::SceneRoot()
{
    //std::cout << "SceneRoot::create()" << std::endl;

    m_rootNode = new osg::Group();
    m_rootNode->setName( "SceneRoot" );

    osg::ref_ptr<osg::StateSet> rootStateSet = m_rootNode->getOrCreateStateSet();

    rootStateSet->setMode( GL_RESCALE_NORMAL , osg::StateAttribute::ON );
    rootStateSet->setMode( GL_LIGHT0         , osg::StateAttribute::ON );
    rootStateSet->setMode( GL_LIGHT1         , osg::StateAttribute::ON );
    rootStateSet->setMode( GL_LIGHTING       , osg::StateAttribute::ON );
//    rootStateSet->setMode( GL_BLEND          , osg::StateAttribute::ON );
//    rootStateSet->setMode( GL_ALPHA_TEST     , osg::StateAttribute::ON );
//    rootStateSet->setMode( GL_DEPTH_TEST     , osg::StateAttribute::ON );
//    rootStateSet->setRenderBinDetails( 1, "DepthSortedBin" );
}

////////////////////////////////////////////////////////////////////////////////

SceneRoot::~SceneRoot() {}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::setRootNode( osg::Node *root )
{
    m_rootNode->removeChildren( 0, m_rootNode->getNumChildren() );
    if ( root ) m_rootNode->addChild( root );

    createSceneLight();
}

////////////////////////////////////////////////////////////////////////////////

void SceneRoot::createSceneLight()
{
    osg::ref_ptr<osg::PositionAttitudeTransform> patSun = new osg::PositionAttitudeTransform();
    m_rootNode->addChild( patSun.get() );

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
    lightSourceSun->setStateSetModes( *m_rootNode->getOrCreateStateSet(), osg::StateAttribute::ON );
}
