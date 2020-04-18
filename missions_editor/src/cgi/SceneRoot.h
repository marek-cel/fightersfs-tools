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
#ifndef SCENEROOT_H
#define SCENEROOT_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Group>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <editor/Singleton.h>

#include <cgi/Camera.h>
#include <cgi/Routes.h>
#include <cgi/Scenery.h>
#include <cgi/Units.h>

////////////////////////////////////////////////////////////////////////////////

/** Scene root class. */
class SceneRoot : public Singleton< SceneRoot >
{
    friend class Singleton< SceneRoot >;


private:

    /** Constructor. */
    SceneRoot();

public:

    /** Destructor. */
    virtual ~SceneRoot();

    inline osgGA::CameraManipulator* getManipulator() { return m_camera->getManipulator(); }

    void reloadRoutes();
    void reloadScenery();
    void reloadUnits();

    void hideAll();

    void hideMarker();
    void showMarker( osg::Vec3d position );

    void hideRoute();
    void showRoute( int routeIndex );

    void hideUnit();
    void showUnit( int unitIndex );

    void update();

    void setViewOrbit();
    void setViewWorld();

    /** Returns root node. */
    inline osg::ref_ptr<osg::Group> getNode() { return m_root; }

private:

    osg::ref_ptr<osg::Group> m_root;            ///< root group

    Camera  *m_camera;
    Routes  *m_routes;
    Scenery *m_scenery;
    Units   *m_units;

    osg::ref_ptr<osg::Group> m_groupRoute;      ///< route group

    osg::Vec3d m_position;                      ///< marker position

    osg::ref_ptr<osg::PositionAttitudeTransform> m_patMarker;
    osg::ref_ptr<osg::Switch> m_switchMarker;

    void createMarker();
    void createSceneLight();

    void loadTerrain();
};

////////////////////////////////////////////////////////////////////////////////

#endif // SCENEROOT_H
