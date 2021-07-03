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

#include <editor/Mission.h>

#include <iostream>
#include <sstream>

#include <osg/Vec3d>

#include <QFileInfo>
#include <QTextStream>

#include <Languages.h>

#include <editor/Misc.h>

////////////////////////////////////////////////////////////////////////////////

void Mission::saveTextNode( QDomDocument &doc, QDomElement &parentNode,
                            const std::string &tagName, const std::string &tagValue )
{
    QDomElement node = doc.createElement( tagName.c_str() );
    parentNode.appendChild( node );

    QDomNode text = doc.createTextNode( tagValue.c_str() );
    node.appendChild( text );
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveTextNode( QDomDocument &doc, QDomElement &parentNode,
                            const std::string &tagName, const Text &tagValue )
{
    QDomElement node = doc.createElement( tagName.c_str() );
    parentNode.appendChild( node );

    for ( int i = 0; i < Languages::instance()->getCount(); i++ )
    {
        QDomElement nodeText = doc.createElement( "text" );
        node.appendChild( nodeText );

        QDomAttr nodeTextLang = doc.createAttribute( "lang" );
        nodeTextLang.setValue( Languages::instance()->getCodeByIndex( i ) );
        nodeText.setAttributeNode( nodeTextLang );

        QString string = tagValue.get( i ).c_str();

        QDomNode text = doc.createTextNode( string );
        nodeText.appendChild( text );
    }
}

////////////////////////////////////////////////////////////////////////////////

Mission::Mission()
{
    newEmpty();
}

////////////////////////////////////////////////////////////////////////////////

Mission::~Mission()
{
    newEmpty();
}

////////////////////////////////////////////////////////////////////////////////

void Mission::newEmpty()
{
    m_tutorial = false;

    m_missionIcon.clear();
    m_missionIconLocked.clear();
    m_missionImage.clear();
    m_missionImageLocked.clear();
    m_missionName = Text();
    m_missionIntroduction = Text();
    m_missionSummaryFailure = Text();
    m_missionSummarySuccess = Text();
    m_elevationFile.clear();
    m_terrainFile.clear();
    m_genericFile.clear();
    m_skyDomeFile.clear();
    m_objectFiles.clear();

    m_visibility = 7200;
    m_sunCoef = 1.0f;

    Routes::iterator itR = m_routes.begin();

    while ( itR != m_routes.end() )
    {
        if ( *itR ) delete (*itR);
        (*itR) = 0;

        itR = m_routes.erase( itR );
    }

    m_routes.clear();

    Units::iterator itU = m_units.begin();

    while ( itU != m_units.end() )
    {
        if ( *itU ) delete (*itU);
        (*itU) = 0;

        itU = m_units.erase( itU );
    }

    m_units.clear();

    Stages::iterator itS = m_stages.begin();

    while ( itS != m_stages.end() )
    {
        if ( *itS ) delete (*itS);
        (*itS) = 0;

        itS = m_stages.erase( itS );
    }

    m_stages.clear();
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readFile( QString fileName )
{
    bool status = false;

    newEmpty();

    QFile devFile( fileName );

    if ( devFile.open( QFile::ReadOnly | QFile::Text ) )
    {
        QDomDocument doc;

        doc.setContent( &devFile, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "mission" )
        {
            status = readMission( rootNode );
        }

        devFile.close();
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::saveFile( QString fileName )
{
    QString fileTemp = fileName;

    if ( QFileInfo( fileTemp ).suffix() != QString( "xml" ) )
    {
        fileTemp += ".xml";
    }

    QFile devFile( fileTemp );

    if ( devFile.open( QFile::WriteOnly | QFile::Truncate | QFile::Text ) )
    {
        QTextStream out;
        out.setDevice( &devFile );
        out.setCodec("UTF-8");
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

        QDomDocument doc( "mission" );

        doc.setContent( &devFile, false );

        QDomElement rootNode = doc.createElement( "mission" );
        doc.appendChild( rootNode );

        saveMission( doc, rootNode );

        out << doc.toString();

        devFile.close();

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

Unit* Mission::getOwnship()
{
    for ( Units::iterator it = m_units.begin(); it != m_units.end(); ++it )
    {
        Unit *unit = *it;

        if ( unit )
        {
            if ( unit->isOwnship() )
            {
                return unit;
            }
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

Route* Mission::getRouteByName( const std::string &routeName )
{
    for ( Routes::iterator it = m_routes.begin(); it != m_routes.end(); ++it )
    {
        if ( QString( (*it)->name.c_str() ) == QString( routeName.c_str() ) )
        {
            return *it;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int Mission::getRouteIndex( const std::string &routeName )
{
    for ( int i = 0; i < (int)m_routes.size(); i++ )
    {
        if ( QString( m_routes[ i ]->name.c_str() ) == QString( routeName.c_str() ) )
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////

std::string Mission::getRouteName( int routeIndex )
{
    if ( routeIndex >= 0 && routeIndex < (int)m_routes.size() )
    {
        return m_routes[ routeIndex ]->name;
    }

    return "";
}

////////////////////////////////////////////////////////////////////////////////

int Mission::getUnitIndex( const std::string &unitName )
{
    for ( int i = 0; i < (int)m_units.size(); i++ )
    {
        if ( QString( m_units[ i ]->getName().c_str() ) == QString( unitName.c_str() ) )
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////

std::string Mission::getUnitName( int unitIndex )
{
    if ( unitIndex >= 0 && unitIndex < (int)m_units.size() )
    {
        return m_units[ unitIndex ]->getName();
    }

    return "";
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionIcon( const std::string &missionIcon )
{
    m_missionIcon = missionIcon;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionIconLocked( const std::string &missionIconLocked )
{
    m_missionIconLocked = missionIconLocked;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionImage( const std::string &missionImage )
{
    m_missionImage = missionImage;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionImageLocked( const std::string &missionImageLocked )
{
    m_missionImageLocked = missionImageLocked;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionName( const Text &missionName )
{
    m_missionName = missionName;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionIntroduction( const Text &missionIntroduction )
{
    m_missionIntroduction = missionIntroduction;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionSummaryFailure( const Text &missionSummaryFailure )
{
    m_missionSummaryFailure = missionSummaryFailure;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setMissionSummarySuccess( const Text &missionSummarySuccess )
{
    m_missionSummarySuccess = missionSummarySuccess;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setElevationFile( const std::string &elevationFile )
{
    m_elevationFile = elevationFile;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setTerrainFile( const std::string &terrainFile )
{
    m_terrainFile = terrainFile;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setGenericFile( const std::string &genericFile )
{
    m_genericFile = genericFile;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setSkyDomeFile( const std::string &skyDomeFile )
{
    m_skyDomeFile = skyDomeFile;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setVisibility( int visibility )
{
    m_visibility = visibility;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::setSunCoef( float sunCoef )
{
    m_sunCoef = sunCoef;

    if ( m_sunCoef < 0.0 ) m_sunCoef = 0.0;
    if ( m_sunCoef > 1.0 ) m_sunCoef = 1.0;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readMission( QDomElement &xmlNode )
{
    bool status = true;

    // tutorial
    if ( status )
    {
        m_tutorial = xmlNode.attributeNode( "tutorial" ).value().toInt() != 0;
    }

    // info
    if ( status )
    {
        QDomElement dataNode = xmlNode.firstChildElement( "data" );

        if ( !dataNode.isNull() )
        {
            status = readData( dataNode );
        }
        else
        {
            status = false;
        }

        if ( !status )
        {
            std::cerr << "Error reading data" << std::endl;
        }
    }

    // scenery
    if ( status )
    {
        QDomElement sceneryNode = xmlNode.firstChildElement( "scenery" );

        if ( !sceneryNode.isNull() )
        {
            status = readScenery( sceneryNode );
        }
        else
        {
            status = false;
        }

        if ( !status )
        {
            std::cerr << "Error reading scenery" << std::endl;
        }
    }

    // units
    if ( status )
    {
        QDomElement unitsNode = xmlNode.firstChildElement( "units" );

        if ( !unitsNode.isNull() )
        {
            status = readUnits( unitsNode );
        }
        else
        {
            status = false;
        }

        if ( !status )
        {
            std::cerr << "Error reading units" << std::endl;
        }
    }

    // routes
    if ( status )
    {
        QDomElement routesNode = xmlNode.firstChildElement( "routes" );

        if ( !routesNode.isNull() )
        {
            status = readRoutes( routesNode );
        }
        else
        {
            status = false;
        }

        if ( !status )
        {
            std::cerr << "Error reading routes" << std::endl;
        }
    }

    // stages
    if ( status )
    {
        QDomElement stagesNode = xmlNode.firstChildElement( "stages" );

        if ( !stagesNode.isNull() )
        {
            status = readStages( stagesNode );
        }
        else
        {
            status = false;
        }

        if ( !status )
        {
            std::cerr << "Error reading stages" << std::endl;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readData( QDomElement &xmlNode )
{
    if ( !xmlNode.isNull() )
    {
        bool status = true;

        // icon
        //if ( status )
        //{
        //    QDomElement iconNode = xmlNode.firstChildElement( "icon" );
        //
        //    if ( !iconNode.isNull() )
        //    {
        //        m_missionIcon = iconNode.text().toStdString();
        //    }
        //    else
        //    {
        //        status = false;
        //    }
        //}

        // icon locked
        //if ( status )
        //{
        //    QDomElement iconLockedNode = xmlNode.firstChildElement( "icon_locked" );
        //
        //    if ( !iconLockedNode.isNull() )
        //    {
        //        m_missionIconLocked = iconLockedNode.text().toStdString();
        //    }
        //    else
        //    {
        //        status = false;
        //    }
        //}

        // image
        if ( status )
        {
            QDomElement imageNode = xmlNode.firstChildElement( "image" );

            if ( !imageNode.isNull() )
            {
                m_missionImage = imageNode.text().toStdString();
            }
            else
            {
                status = false;
            }
        }

        // image locked
        if ( status )
        {
            QDomElement imageLockedNode = xmlNode.firstChildElement( "image_locked" );

            if ( !imageLockedNode.isNull() )
            {
                m_missionImageLocked = imageLockedNode.text().toStdString();
            }
            else
            {
                status = false;
            }
        }

        // name
        if ( status )
        {
            QDomElement nameNode = xmlNode.firstChildElement( "name" );

            if ( !nameNode.isNull() )
            {
                Text *text = Text::readText( nameNode );

                m_missionName = *text;

                if ( text ) delete text;
                text = 0;
            }
            else
            {
                status = false;
            }
        }

        // introduction
        if ( status )
        {
            QDomElement introductionNode = xmlNode.firstChildElement( "introduction" );

            if ( !introductionNode.isNull() )
            {
                Text *text = Text::readText( introductionNode );

                m_missionIntroduction = *text;

                if ( text ) delete text;
                text = 0;
            }
            else
            {
                status = false;
            }
        }

        // summary_failure
        if ( status )
        {
            QDomElement summaryFailureNode = xmlNode.firstChildElement( "summary_failure" );

            if ( !summaryFailureNode.isNull() )
            {
                Text *text = Text::readText( summaryFailureNode );

                m_missionSummaryFailure = *text;

                if ( text ) delete text;
                text = 0;
            }
            else
            {
                status = false;
            }
        }

        // summary_success
        if ( status )
        {
            QDomElement summarySuccessNode = xmlNode.firstChildElement( "summary_success" );

            if ( !summarySuccessNode.isNull() )
            {
                Text *text = Text::readText( summarySuccessNode );

                m_missionSummarySuccess = *text;

                if ( text ) delete text;
                text = 0;
            }
            else
            {
                status = false;
            }
        }

        return status;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readScenery( QDomElement &xmlNode )
{
    if ( !xmlNode.isNull() )
    {
        QDomElement terrainNode = xmlNode.firstChildElement( "terrain" );
        QDomElement genericNode = xmlNode.firstChildElement( "generic" );
        QDomElement skyDomeNode = xmlNode.firstChildElement( "sky_dome" );

        if ( !terrainNode.isNull() && !genericNode.isNull() && !skyDomeNode.isNull() )
        {
            m_terrainFile = terrainNode.text().toStdString();
            m_genericFile = genericNode.text().toStdString();
            m_skyDomeFile = skyDomeNode.text().toStdString();

            // visibility
            QDomElement visibilityNode = xmlNode.firstChildElement( "visibility" );

            if ( !visibilityNode.isNull() )
            {
                m_visibility = visibilityNode.text().toInt();
            }
            else
            {
                m_visibility = 7200;
            }


            // sun_coef
            QDomElement sunCoefNode = xmlNode.firstChildElement( "sun_coef" );

            if ( !sunCoefNode.isNull() )
            {
                m_sunCoef = sunCoefNode.text().toFloat();
            }
            else
            {
                m_sunCoef = 1.0;
            }


            // elevation
            QDomElement elevationNode = xmlNode.firstChildElement( "elevation" );

            if ( !elevationNode.isNull() )
            {
                m_elevationFile = elevationNode.text().toStdString();
            }
            else
            {
                m_elevationFile = "";
            }

            // objects
            QDomElement objectsNode = xmlNode.firstChildElement( "objects" );

            if ( !objectsNode.isNull() )
            {
                QDomElement objectNode = objectsNode.firstChildElement( "object" );

                while ( !objectNode.isNull() )
                {
                    std::string objectFile = objectNode.text().toStdString();

                    if ( objectFile.length() > 0 )
                    {
                        m_objectFiles.push_back( objectFile );
                    }

                    objectNode = objectNode.nextSiblingElement( "object" );
                }
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readRoutes( QDomElement &xmlNode )
{
    QDomElement routeNode = xmlNode.firstChildElement( "route" );

    while ( !routeNode.isNull() )
    {
        Route *route = new Route();

        route->name = routeNode.attributeNode( "name" ).value().toStdString();

        QDomElement waypointNode = routeNode.firstChildElement( "waypoint" );

        while ( !waypointNode.isNull() )
        {
            QDomElement positionNode = waypointNode.firstChildElement( "position" );
            QDomElement speedNode    = waypointNode.firstChildElement( "speed" );

            if ( !positionNode.isNull() && !speedNode.isNull() )
            {
                int x = std::numeric_limits< int >::quiet_NaN();
                int y = std::numeric_limits< int >::quiet_NaN();
                int z = std::numeric_limits< int >::quiet_NaN();
                int v = std::numeric_limits< int >::quiet_NaN();

                std::stringstream ssp( positionNode .text().toStdString() );
                std::stringstream ssv( speedNode    .text().toStdString() );

                ssp >> x;
                ssp >> y;
                ssp >> z;
                ssv >> v;

                if ( Misc::isValid( x ) && Misc::isValid( y ) && Misc::isValid( z )
                  && Misc::isValid( v ))
                {
                    Waypoint waypoint;

                    waypoint.first = osg::Vec3d( x, y, z );
                    waypoint.second = v;

                    route->push_back( waypoint );
                }
                else
                {
                    if ( route ) delete route;
                    route = 0;

                    return false;
                }
            }
            else
            {
                if ( route ) delete route;
                route = 0;

                return false;
            }

            waypointNode = waypointNode.nextSiblingElement( "waypoint" );
        }

        m_routes.push_back( route );

        routeNode = routeNode.nextSiblingElement( "route" );
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readStages( QDomElement &xmlNode )
{
    if ( !xmlNode.isNull() )
    {
        QDomElement stageNode = xmlNode.firstChildElement( "stage" );

        while ( !stageNode.isNull() )
        {
            Stage *stage = Stage::readStage( stageNode );

            if ( stage )
            {
                m_stages.push_back( stage );
            }
            else
            {
                return false;
            }

            stageNode = stageNode.nextSiblingElement( "stage" );
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readUnits( QDomElement &xmlNode )
{
    bool status = true;

    if ( !xmlNode.isNull() )
    {
        // ownship
        if ( status )
        {
            QDomElement ownshipNode = xmlNode.firstChildElement( "ownship" );

            if ( !ownshipNode.isNull() )
            {
                status = readUnits( ownshipNode, Unit::Friend, true );
            }
        }

        // friends
        if ( status )
        {
            QDomElement friendsNode = xmlNode.firstChildElement( "friends" );

            if ( !friendsNode.isNull() )
            {
                status = readUnits( friendsNode, Unit::Friend );
            }
        }

        // hostiles
        if ( status )
        {
            QDomElement hostilesNode = xmlNode.firstChildElement( "hostiles" );

            if ( !hostilesNode.isNull() )
            {
                status = readUnits( hostilesNode, Unit::Hostile );
            }
        }

        // neutrals
        if ( status )
        {
            QDomElement neutralsNode = xmlNode.firstChildElement( "neutrals" );

            if ( !neutralsNode.isNull() )
            {
                status = readUnits( neutralsNode, Unit::Neutral );
            }
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////

bool Mission::readUnits( QDomElement &xmlNode, Unit::Affiliation affiliation,
                         bool ownship )
{
    if ( !xmlNode.isNull() )
    {
        QDomElement unitNode = xmlNode.firstChildElement();

        while ( !unitNode.isNull() )
        {
            Unit *unit = Unit::readUnit( unitNode, affiliation );

            if ( unit )
            {
                if ( ownship ) unit->setOwnship( true );

                m_units.push_back( unit );
            }
            else
            {
                return false;
            }

            unitNode = unitNode.nextSiblingElement();
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveMission( QDomDocument &doc, QDomElement &parentNode )
{
    // tutorial
    QDomAttr nodeTutorial = doc.createAttribute( "tutorial" );
    nodeTutorial.setValue( m_tutorial ? "1" : "0" );
    parentNode.setAttributeNode( nodeTutorial );

    // data
    QDomElement dataNode = doc.createElement( "data" );
    parentNode.appendChild( dataNode );

    saveData( doc, dataNode );

    // scenery
    QDomElement sceneryNode = doc.createElement( "scenery" );
    parentNode.appendChild( sceneryNode );

    saveScenery( doc, sceneryNode );

    // units
    QDomElement unitsNode = doc.createElement( "units" );
    parentNode.appendChild( unitsNode );

    saveUnits( doc, unitsNode );

    // routes
    QDomElement routesNode = doc.createElement( "routes" );
    parentNode.appendChild( routesNode );

    saveRoutes( doc, routesNode );

    // stages
    QDomElement stagesNode = doc.createElement( "stages" );
    parentNode.appendChild( stagesNode );

    saveStages( doc, stagesNode );
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveData( QDomDocument &doc, QDomElement &parentNode )
{
    //saveTextNode( doc, parentNode, "icon", m_missionIcon );
    //saveTextNode( doc, parentNode, "icon_locked", m_missionIconLocked );
    saveTextNode( doc, parentNode, "image", m_missionImage );
    saveTextNode( doc, parentNode, "image_locked", m_missionImageLocked );
    saveTextNode( doc, parentNode, "name", m_missionName );
    saveTextNode( doc, parentNode, "introduction", m_missionIntroduction );
    saveTextNode( doc, parentNode, "summary_failure", m_missionSummaryFailure );
    saveTextNode( doc, parentNode, "summary_success", m_missionSummarySuccess );
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveScenery( QDomDocument &doc, QDomElement &parentNode )
{
    // elevation
    if ( m_elevationFile.length() > 0 )
    {
        saveTextNode( doc, parentNode, "elevation", m_elevationFile );
    }

    // terrain
    saveTextNode( doc, parentNode, "terrain", m_terrainFile );

    // generic
    saveTextNode( doc, parentNode, "generic", m_genericFile );

    // sky dome
    saveTextNode( doc, parentNode, "sky_dome", m_skyDomeFile );

    // visibility
    saveTextNode( doc, parentNode, "visibility", QString::number( m_visibility ).toStdString() );

    // sun_coef
    saveTextNode( doc, parentNode, "sun_coef", QString::number( m_sunCoef ).toStdString() );

    // objects
    QDomElement objectsNode = doc.createElement( "objects" );
    parentNode.appendChild( objectsNode );

    for ( ObjectFiles::iterator it = m_objectFiles.begin(); it != m_objectFiles.end(); ++it )
    {
        if ( (*it).length() > 0 )
        {
            saveTextNode( doc, objectsNode, "object", *it );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveRoutes( QDomDocument &doc, QDomElement &parentNode )
{
    for ( int i = 0; i < (int)m_routes.size(); i++ )
    {
        Route *route = m_routes[ i ];
        std::string name = m_routes[ i ]->name;

        QDomElement routeNode = doc.createElement( "route" );
        parentNode.appendChild( routeNode );

        QDomAttr nodeRouteName = doc.createAttribute( "name" );
        nodeRouteName.setValue( name.c_str() );
        routeNode.setAttributeNode( nodeRouteName );

        for ( Route::iterator it = route->begin(); it != route->end(); ++it )
        {
            Waypoint waypoint = *it;

            QDomElement waypointNode = doc.createElement( "waypoint" );
            routeNode.appendChild( waypointNode );

            QString position = QString::number( (int)waypoint.first.x() ) + " "
                             + QString::number( (int)waypoint.first.y() ) + " "
                             + QString::number( (int)waypoint.first.z() );
            QString speed = QString::number( (int)waypoint.second );

            saveTextNode( doc, waypointNode, "position" , position.toStdString() );
            saveTextNode( doc, waypointNode, "speed"    , speed.toStdString() );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveStages( QDomDocument &doc, QDomElement &parentNode )
{
    for ( Stages::iterator it = m_stages.begin(); it != m_stages.end(); ++it )
    {
        (*it)->saveStage( doc, parentNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveUnits( QDomDocument &doc, QDomElement &parentNode )
{
    // ownship
    QDomElement ownshipNode = doc.createElement( "ownship" );
    parentNode.appendChild( ownshipNode );

    saveOwnship( doc, ownshipNode );

    // friends
    QDomElement friendsNode = doc.createElement( "friends" );
    parentNode.appendChild( friendsNode );

    saveUnits( doc, friendsNode, Unit::Friend );

    // hostiles
    QDomElement hostilesNode = doc.createElement( "hostiles" );
    parentNode.appendChild( hostilesNode );

    saveUnits( doc, hostilesNode, Unit::Hostile );

    // neutrals
    QDomElement neutralsNode = doc.createElement( "neutrals" );
    parentNode.appendChild( neutralsNode );

    saveUnits( doc, neutralsNode, Unit::Neutral );
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveOwnship( QDomDocument &doc, QDomElement &parentNode )
{
    for ( Units::iterator it = m_units.begin(); it != m_units.end(); ++it )
    {
        if ( (*it)->isOwnship() )
        {
            (*it)->saveUnit( doc, parentNode );
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mission::saveUnits( QDomDocument &doc, QDomElement &parentNode, Unit::Affiliation affiliation )
{
    for ( Units::iterator it = m_units.begin(); it != m_units.end(); ++it )
    {
        if ( !(*it)->isOwnship() )
        {
            if ( affiliation == (*it)->getAffiliation() )
            {
                (*it)->saveUnit( doc, parentNode );
            }
        }
    }
}
