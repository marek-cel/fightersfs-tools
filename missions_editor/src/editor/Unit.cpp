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

#include <editor/Unit.h>

#include <iostream>
#include <sstream>
#include <limits>

#include <QFileInfo>

#include <editor/Misc.h>
#include <editor/Mission.h>
#include <editor/ObjectiveDestroy.h>


////////////////////////////////////////////////////////////////////////////////

Unit::Type Unit::nameToType( const QString &name )
{
    if      ( name == "aircraft"       ) return Aircraft;
    else if ( name == "balloon"        ) return Balloon;
    else if ( name == "bomber_dive"    ) return BomberDive;
    else if ( name == "bomber_level"   ) return BomberLevel;
    else if ( name == "bomber_torpedo" ) return BomberTorpedo;
    else if ( name == "building"       ) return Building;
    else if ( name == "fighter"        ) return Fighter;
    else if ( name == "kamikaze"       ) return Kamikaze;
    else if ( name == "warship"        ) return Warship;
    else
    {
        std::cerr << "Unknown unit type: " << name.toStdString() << std::endl;
    }

#   warning UNIT_TYPE

    return Unknown;
}

////////////////////////////////////////////////////////////////////////////////

std::string Unit::typeToName( Type type )
{
    switch ( type )
    {
        case Aircraft:      return "aircraft";       break;
        case Balloon:       return "balloon";        break;
        case BomberDive:    return "bomber_dive";    break;
        case BomberLevel:   return "bomber_level";   break;
        case BomberTorpedo: return "bomber_torpedo"; break;
        case Building:      return "building";       break;
        case Fighter:       return "fighter";        break;
        case Kamikaze:      return "kamikaze";       break;
        case Warship:       return "warship";        break;

        case Unknown: return ""; break;
        case Maximum: return ""; break;
    }

#warning UNIT_TYPE

    return "";
}

////////////////////////////////////////////////////////////////////////////////

bool Unit::isAircraft( Type type )
{
    bool result = type == Aircraft
               || type == BomberDive
               || type == BomberLevel
               || type == BomberTorpedo
               || type == Fighter
               || type == Kamikaze;

#warning UNIT_TYPE

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Unit* Unit::readUnit( QDomElement &xmlNode, Affiliation affiliation )
{
    if ( !xmlNode.isNull() )
    {
        Type type = nameToType( xmlNode.tagName() );

        if ( type != Unknown )
        {
            std::string file   = "";
            std::string name   = xmlNode.attributeNode( "name" ).value().toStdString();
            std::string route  = xmlNode.attributeNode( "route" ).value().toStdString();
            std::string leader = xmlNode.attributeNode( "leader" ).value().toStdString();
            std::string livery = "";
            int hp = xmlNode.attributeNode( "hp" ).value().toInt();
            bool unique = xmlNode.attributeNode( "unique" ).value().toInt() != 0;

            if ( hp < 1 || hp > 100 ) hp = 100;

            osg::Vec3d offset( 0.0, 0.0, 0.0 );
            osg::Vec3d position( 0.0, 0.0, 0.0 );

            double heading  = 0.0;
            double velocity = 0.0;

            // file
            QDomElement fileNode = xmlNode.firstChildElement( "file" );

            if ( !fileNode.isNull() )
            {
                file = fileNode.text().toStdString();
            }

            // offset
            QDomElement offsetNode = xmlNode.firstChildElement( "offset" );

            if ( !offsetNode.isNull() )
            {
                double x = std::numeric_limits< double >::quiet_NaN();
                double y = std::numeric_limits< double >::quiet_NaN();
                double z = std::numeric_limits< double >::quiet_NaN();

                std::stringstream ss( offsetNode.text().toStdString() );

                ss >> x;
                ss >> y;
                ss >> z;

                if ( Misc::isValid( x ) && Misc::isValid( y ) && Misc::isValid( z ) )
                {
                    offset = osg::Vec3d( x, y, z );
                }
            }

            // position
            QDomElement positionNode = xmlNode.firstChildElement( "position" );

            if ( !positionNode.isNull() )
            {
                double x = std::numeric_limits< double >::quiet_NaN();
                double y = std::numeric_limits< double >::quiet_NaN();
                double z = std::numeric_limits< double >::quiet_NaN();

                std::stringstream ss( positionNode.text().toStdString().c_str() );

                ss >> x;
                ss >> y;
                ss >> z;

                if ( Misc::isValid( x ) && Misc::isValid( y ) && Misc::isValid( z ) )
                {
                    position = osg::Vec3d( x, y, z );
                }
            }

            // livery
            QDomElement liveryNode = xmlNode.firstChildElement( "livery" );

            if ( !liveryNode.isNull() )
            {
                livery = liveryNode.text().toStdString();
            }

            // heading
            QDomElement headingNode = xmlNode.firstChildElement( "heading" );

            if ( !headingNode.isNull() )
            {
                heading = headingNode.text().toDouble();
            }

            // velocity
            QDomElement velocityNode = xmlNode.firstChildElement( "velocity" );

            if ( !velocityNode.isNull() )
            {
                velocity = velocityNode.text().toDouble();
            }

            Unit *unit = new Unit();

            unit->m_affiliation = affiliation;
            unit->m_type = type;
            unit->m_name = name;
            unit->m_route = route;
            unit->setFile( file );
            unit->m_position = position;
            unit->m_leader = leader;
            unit->m_offset = offset;
            unit->m_livery = livery;
            unit->setHeading( heading );
            unit->m_velocity = velocity;
            unit->m_hp = hp;
            unit->m_wingman = leader.length() > 0;
            unit->m_unique = unique;

            return unit;
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

Unit::Unit() :
    m_affiliation ( Neutral ),
    m_type ( Unknown ),
    m_name ( "" ),
    m_route ( "" ),
    m_file ( "" ),
    m_model ( "" ),
    m_position ( 0.0, 0.0, 0.0 ),
    m_leader ( "" ),
    m_livery ( "" ),
    m_offset ( 0.0, 0.0, 0.0 ),
    m_heading ( -M_PI_2 ),
    m_velocity ( 0.0 ),
    m_ownship ( false ),
    m_unique ( false ),
    m_wingman ( false ),
    m_hp ( 100 )
{
    setHeading( m_heading );
}

////////////////////////////////////////////////////////////////////////////////

Unit::~Unit()
{
    // remove unit from all units leaders
    for ( Mission::Units::iterator it = Mission::getInstance()->getUnits().begin();
          it != Mission::getInstance()->getUnits().end();
          ++it )
    {
        if ( *it )
        {
            if ( QString( (*it)->getLeader().c_str() ) == QString( m_name.c_str() ) )
            {
                (*it)->setLeader( "" );
            }
        }
    }

    for ( int i = 0; i < (int)Mission::getInstance()->getStages().size(); i++ )
    {
        // remove unit from all stages initialization lists
        Stage::InitUnits::iterator it = Mission::getInstance()->getStages()[ i ]->getInitUnits().begin();

        while ( it != Mission::getInstance()->getStages()[ i ]->getInitUnits().end() )
        {
            if ( QString( (*it).c_str() ) == QString( m_name.c_str() ) )
            {
                it = Mission::getInstance()->getStages()[ i ]->getInitUnits().erase( it );
            }
            else
            {
                ++it;
            }
        }

        // remove unit from all objectives destroy
        for ( Stage::Objectives::iterator it = Mission::getInstance()->getStages()[ i ]->getObjectives().begin();
              it != Mission::getInstance()->getStages()[ i ]->getObjectives().end();
              ++it )
        {
            ObjectiveDestroy *obj = dynamic_cast< ObjectiveDestroy* >( (*it) );

            if ( obj )
            {
                ObjectiveDestroy::Units::iterator it = obj->getUnits().begin();

                while ( it != obj->getUnits().end() )
                {
                    if ( QString( m_name.c_str() ) == QString( (*it).c_str() ) )
                    {
                        it = obj->getUnits().erase( it );
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Unit::saveUnit( QDomDocument &doc, QDomElement &parentNode )
{
    if ( m_type != Unknown )
    {
        QDomElement unitNode = doc.createElement( typeToName( m_type ).c_str() );
        parentNode.appendChild( unitNode );

        QDomAttr nodeUnitName = doc.createAttribute( "name" );
        nodeUnitName.setValue( m_name.c_str() );
        unitNode.setAttributeNode( nodeUnitName );

        QDomAttr nodeUnitHP = doc.createAttribute( "hp" );
        nodeUnitHP.setValue( QString::number( m_hp ) );
        unitNode.setAttributeNode( nodeUnitHP );

        if ( isAircraft( m_type ) )
        {
            if ( m_route.length() > 0 )
            {
                QDomAttr nodeUnitRoute = doc.createAttribute( "route" );
                nodeUnitRoute.setValue( m_route.c_str() );
                unitNode.setAttributeNode( nodeUnitRoute );
            }

            if ( m_wingman && m_leader.length() > 0 )
            {
                QDomAttr nodeUnitLeader = doc.createAttribute( "leader" );
                nodeUnitLeader.setValue( m_leader.c_str() );
                unitNode.setAttributeNode( nodeUnitLeader );

                QString offset = QString::number( m_offset.x() )
                         + " " + QString::number( m_offset.y() )
                         + " " + QString::number( m_offset.z() );

                Mission::saveTextNode( doc, unitNode, "offset" , offset.toStdString() );
            }

            if ( m_unique )
            {
                QDomAttr nodeUnique = doc.createAttribute( "unique" );
                nodeUnique.setValue( QString::number( 1 ) );
                unitNode.setAttributeNode( nodeUnique );
            }
        }

        Mission::saveTextNode( doc, unitNode, "file" , m_file );

        QString position = QString::number( m_position.x() )
                   + " " + QString::number( m_position.y() )
                   + " " + QString::number( m_position.z() );
        QString heading  = QString::number( m_heading );
        QString velocity = QString::number( m_velocity );

        Mission::saveTextNode( doc, unitNode, "position" , position.toStdString() );
        Mission::saveTextNode( doc, unitNode, "heading"  , heading.toStdString()  );

        if ( isAircraft( m_type ) )
        {
            Mission::saveTextNode( doc, unitNode, "velocity", velocity.toStdString() );
            Mission::saveTextNode( doc, unitNode, "livery", m_livery );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Unit::setFile( const std::string &file )
{
    m_file = file;
    m_model = "";

    QFile unitFile( ( "data/" + m_file ).c_str() );

    if ( unitFile.open( QFile::ReadOnly | QFile::Text ) )
    {
        QDomDocument doc;

        doc.setContent( &unitFile, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "unit" )
        {
            // file
            QDomElement modelNode = rootNode.firstChildElement( "model" );

            if ( !modelNode.isNull() )
            {
                m_model = modelNode.text().toStdString();
            }
        }

        unitFile.close();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Unit::setHeading( double heading )
{
    m_heading = heading;

    float psi = -M_PI_2 - osg::DegreesToRadians( heading );

    m_attitude = osg::Quat( psi, osg::Z_AXIS );
}

////////////////////////////////////////////////////////////////////////////////

void Unit::setOwnship( bool ownship )
{
    if ( ownship )
    {
        Mission::Units units = Mission::getInstance()->getUnits();

        for ( Mission::Units::iterator it = units.begin(); it != units.end(); ++it )
        {
            (*it)->setOwnship( false );
        }

        // after loop!
        m_ownship = true;
        setUnique( true );
    }
    else
    {
        m_ownship = false;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Unit::setHP( int hp )
{
    m_hp = hp;

    if ( m_hp < 1 || m_hp > 100 ) m_hp = 100;
}
