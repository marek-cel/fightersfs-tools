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

#include <osgViewer/ViewerEventHandlers>

#include <gui/WidgetCGI.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::WidgetCGI( QWidget *parent ) :
    QWidget ( parent ),

    m_sceneRoot  ( 0 ),
    m_gridLayout ( 0 )
{
#   ifdef SIM_OSGDEBUGINFO
    osg::setNotifyLevel( osg::DEBUG_INFO );
#   else
    osg::setNotifyLevel( osg::WARN );
#   endif

    //std::cout << "WidgetCGI::WidgetCGI( QWidget* )" << std::endl;

    m_sceneRoot = new SceneRoot();

    m_manipulatorOrbit = new ManipulatorOrbit();
    m_manipulatorWorld = new ManipulatorWorld();

    setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
    //setThreadingModel( osgViewer::ViewerBase::CullDrawThreadPerContext );

    m_graphicsWindow = createGraphicsWindow( x(), y(), width(), height() );

    QWidget *widget = addViewWidget( m_graphicsWindow, m_sceneRoot->getRootNode().get() );

    m_gridLayout = new QGridLayout( this );
    m_gridLayout->setContentsMargins( 1, 1, 1, 1 );

    m_gridLayout->addWidget( widget, 0, 0 );

    setLayout( m_gridLayout );
}

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::~WidgetCGI()
{
    if ( m_sceneRoot ) delete m_sceneRoot; m_sceneRoot = 0;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setRootNode( osg::Node *root )
{
    m_sceneRoot->setRootNode( root );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::update()
{
    //////////////////
    QWidget::update();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setManipulatorOrbit()
{
    setCameraManipulator( m_manipulatorOrbit.get() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setManipulatorWorld()
{
    setCameraManipulator( m_manipulatorWorld.get() );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::paintEvent( QPaintEvent *event )
{
    /////////////////////////////
    QWidget::paintEvent( event );
    /////////////////////////////

    frame();
}

////////////////////////////////////////////////////////////////////////////////

QWidget* WidgetCGI::addViewWidget( osgQt::GraphicsWindowQt *graphicsWindow, osg::Node *scene )
{
    osg::ref_ptr<osg::Camera> camera = getCamera();

    camera->setGraphicsContext( graphicsWindow );

    const osg::GraphicsContext::Traits *traits = graphicsWindow->getTraits();

    camera->setClearColor( osg::Vec4( 0.47, 0.71, 1.0, 1.0 ) );
    camera->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );

    camera->setComputeNearFarMode( osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES );
    camera->setProjectionMatrixAsPerspective( 30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.00, 1000.0 );

    setSceneData( scene );
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( m_manipulatorOrbit.get() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return graphicsWindow->getGLWidget();
}

////////////////////////////////////////////////////////////////////////////////

osg::ref_ptr<osgQt::GraphicsWindowQt> WidgetCGI::createGraphicsWindow( int x, int y, int w, int h, const std::string &name, bool windowDecoration )
{
    osg::DisplaySettings *displaySettings = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();

    traits->windowName       = name;
    traits->windowDecoration = windowDecoration;
    traits->x                = x;
    traits->y                = y;
    traits->width            = w;
    traits->height           = h;
    traits->doubleBuffer     = true;
    traits->alpha            = displaySettings->getMinimumNumAlphaBits();
    traits->stencil          = displaySettings->getMinimumNumStencilBits();
    traits->sampleBuffers    = displaySettings->getMultiSamples();
    traits->samples          = displaySettings->getNumMultiSamples();

    osg::ref_ptr<osgQt::GraphicsWindowQt> graphicsWindow =  new osgQt::GraphicsWindowQt( traits.get() );

    return graphicsWindow;
}
