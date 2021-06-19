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
#ifndef CAMERA_H
#define CAMERA_H

////////////////////////////////////////////////////////////////////////////////

#include <cgi/ManipulatorOrbit.h>
#include <cgi/ManipulatorWorld.h>

////////////////////////////////////////////////////////////////////////////////

class SceneRoot;

/** Camera control class. */
class Camera
{
public:

    /** Constructor. */
    Camera( SceneRoot *sceneRoot );

    /** Destructor. */
    virtual ~Camera();

    /** Updates camera position and attitude. */
    void update();

    /** */
    inline osgGA::CameraManipulator* getManipulator()
    {
        return m_manipulator.get();
    }

    inline osg::Vec3d getPosition() { return m_position; }

    /** */
    inline void setViewOrbit( osg::Node *node )
    {
        m_manipulator = m_manipulatorOrbit.get();
        m_manipulatorOrbit->setTrackNode( node );

        m_manipulatorOrbit->setElevation( M_PI_2 );
        m_manipulatorOrbit->setHeading( 0.0 );
        m_manipulatorOrbit->setDistance( 10000.0 );
    }

    /** */
    inline void setViewWorld( osg::Node *node )
    {
        m_manipulator = m_manipulatorWorld.get();
        m_manipulatorWorld->setNode( node );

        m_manipulatorOrbit->setElevation( M_PI_2 );
        m_manipulatorOrbit->setHeading( 0.0 );
        m_manipulatorOrbit->setDistance( 10000.0 );
    }

private:

    osg::ref_ptr<osgGA::CameraManipulator> m_manipulator;

    osg::ref_ptr<ManipulatorOrbit> m_manipulatorOrbit;
    osg::ref_ptr<ManipulatorWorld> m_manipulatorWorld;

    osg::Quat  m_attitude;
    osg::Vec3d m_position;
};

////////////////////////////////////////////////////////////////////////////////

#endif // CAMERA_H
