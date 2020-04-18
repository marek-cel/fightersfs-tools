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
#ifndef UNIT_H
#define UNIT_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Quat>
#include <osg/Vec3d>

#include <QDomDocument>
#include <QDomElement>

////////////////////////////////////////////////////////////////////////////////

/** Unit class. */
class Unit
{
public:

    enum Affiliation
    {
        Friend  = 0x1,      ///< friend
        Hostile = 0x2,      ///< hostile
        Neutral = 0x3       ///< neutral
    };

    enum Type
    {
        Unknown = 0x0,      ///<

        Aircraft,           ///<
        Balloon,            ///<
        BomberDive,         ///<
        BomberLevel,        ///<
        BomberTorpedo,      ///<
        Building,           ///<
        Fighter,            ///<
        Kamikaze,           ///<
        Warship,            ///<

        Maximum             ///<
    };

    static Type nameToType( const QString &name );
    static std::string typeToName( Type type );

    static bool isAircraft( Type type );

    /**
     * Reads unit from XML file.
     * @param xmlNode unit XML node
     * @return returns unit object on success and null on failure
     */
    static Unit* readUnit( QDomElement &xmlNode, Affiliation affiliation );

    /** Constructor. */
    Unit();

    /** Destructor. */
    virtual ~Unit();

    bool isOwnship() const { return m_ownship; }

    /**
     * Saves unit to XML file.
     * @param doc XML document
     * @param parentNode XML parent node
     */
    void saveUnit( QDomDocument &doc, QDomElement &parentNode );

    inline Affiliation getAffiliation() const { return m_affiliation; }
    inline Type        getType()        const { return m_type; }
    inline std::string getName()        const { return m_name; }
    inline std::string getRoute()       const { return m_route; }
    inline std::string getFile()        const { return m_file; }
    inline std::string getModel()       const { return m_model; }
    inline osg::Vec3d  getPosition()    const { return m_position; }
    inline osg::Quat   getAttitude()    const { return m_attitude; }
    inline std::string getLeader()      const { return m_leader; }
    inline osg::Vec3d  getOffset()      const { return m_offset; }
    inline std::string getLivery()      const { return m_livery; }
    inline double      getHeading()     const { return m_heading; }
    inline double      getVelocity()    const { return m_velocity; }
    inline int         getHP()          const { return m_hp; }
    inline bool        getUnique()      const { return m_unique; }
    inline bool        getWingman()     const { return m_wingman; }

    void setAffiliation( Affiliation affiliation ) { m_affiliation = affiliation; }
    void setType( Type type ) { m_type = type; }
    void setName( const std::string &name ) { m_name = name; }
    void setRoute( const std::string &route ) { m_route = route; }
    void setFile( const std::string &file );
    void setPosition( osg::Vec3d position ) { m_position = position; }
    void setLeader( const std::string &leader ) { m_leader = leader; }
    void setOffset( const osg::Vec3d &offset ) { m_offset = offset; }
    void setLivery( const std::string &livery ) { m_livery = livery; }
    void setHeading( double heading );
    void setVelocity( double velocity ) { m_velocity = velocity; }
    void setUnique( bool unique ) { m_unique = unique; }
    void setWingman( bool wingman ) { m_wingman = wingman; }

    void setOwnship( bool ownship );

    void setHP( int hp );

private:

    Affiliation m_affiliation;  ///< unit affiliation

    Type m_type;                ///< unit type

    std::string m_name;         ///< unit name
    std::string m_route;        ///< name of route (aircraft only)

    std::string m_file;         ///< unit file path

    std::string m_model;        ///< unit model file path

    osg::Vec3d m_position;      ///< [m] initial position expressed in East-North-Up coordinate system
    osg::Quat  m_attitude;      ///< initial attitude expressed as a quaternion

    std::string m_leader;       ///< name of a formation leader (aircraft only)

    std::string m_livery;       ///< livery file path

    osg::Vec3d m_offset;        ///< [m] offset to formation leader (aircraft only)

    double m_heading;           ///< [deg] initial heading
    double m_velocity;          ///< [kts] initial velocity

    bool m_ownship;             ///< specifies if unit is an ownship
    bool m_unique;              ///< specifies in unit is unique
    bool m_wingman;             ///< specifies if unit is a wingman

    int m_hp;                   ///< hit points
};

////////////////////////////////////////////////////////////////////////////////

#endif // UNIT_H
