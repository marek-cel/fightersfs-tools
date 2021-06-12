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
#ifndef WIDGETCGI_H
#define WIDGETCGI_H

////////////////////////////////////////////////////////////////////////////////

#include <gui/GraphicsWindowQt.h>
#include <osgViewer/Viewer>

#include <QDateTime>
#include <QGridLayout>
#include <QWidget>

#include <cgi/ManipulatorOrbit.h>
#include <cgi/ManipulatorWorld.h>
#include <cgi/SceneRoot.h>

////////////////////////////////////////////////////////////////////////////////

/** This is widget wrapper for CGI. */
class WidgetCGI : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:

    /** Constructor. */
    WidgetCGI( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~WidgetCGI();

    /** */
    void setRootNode( osg::Node* root );

    /** */
    void update();

    void setManipulatorOrbit();
    void setManipulatorWorld();

protected:

    /** */
    void paintEvent( QPaintEvent *event );

private:

    SceneRoot   *m_sceneRoot;
    QGridLayout *m_gridLayout;

    osg::ref_ptr<ManipulatorOrbit> m_manipulatorOrbit;
    osg::ref_ptr<ManipulatorWorld> m_manipulatorWorld;

    osg::ref_ptr<GraphicsWindowQt> m_graphicsWindow;

    /** */
    QWidget* addViewWidget( GraphicsWindowQt *graphicsWindow, osg::Node *scene );

    /** */
    osg::ref_ptr<GraphicsWindowQt> createGraphicsWindow( int x, int y, int w, int h,
                                                         const std::string &name = "",
                                                         bool windowDecoration = false );
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETCGI_H
