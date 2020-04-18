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
#ifndef MISSION_H
#define MISSION_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <QDomDocument>
#include <QDomElement>

#include <editor/Singleton.h>

#include <editor/Route.h>
#include <editor/Stage.h>
#include <editor/Text.h>
#include <editor/Unit.h>

////////////////////////////////////////////////////////////////////////////////

/** Mission class. */
class Mission : public Singleton< Mission >
{
    friend class Singleton< Mission >;

public:

    typedef std::vector< std::string > ObjectFiles;

    typedef std::vector< Route* > Routes;
    typedef std::vector< Unit* >  Units;
    typedef std::vector< Stage* > Stages;

    static void saveTextNode( QDomDocument &doc, QDomElement &parentNode,
                              const std::string &tagName, const std::string &tagValue );

    static void saveTextNode( QDomDocument &doc, QDomElement &parentNode,
                              const std::string &tagName, const Text &tagValue );

private:

    /**
     * Constructor.
     * Using of this constructor is forbidden.
     * Instead use static function getInstance() to get Mission object reference.
     */
    Mission();

public:

    /** Destructor. */
    virtual ~Mission();

    /** */
    void newEmpty();

    /** */
    bool readFile( QString fileName );

    /** */
    bool saveFile( QString fileName );

    inline bool getTutorial() const { return m_tutorial; }

    inline std::string getMissionIcon() const { return m_missionIcon; }
    inline std::string getMissionIconLocked() const { return m_missionIconLocked; }
    inline std::string getMissionImage() const { return m_missionImage; }
    inline std::string getMissionImageLocked() const { return m_missionImageLocked; }
    inline Text getMissionName() const { return m_missionName; }
    inline Text getMissionIntroduction() const { return m_missionIntroduction; }
    inline Text getMissionSummaryFailure() const { return m_missionSummaryFailure; }
    inline Text getMissionSummarySuccess() const { return m_missionSummarySuccess; }
    inline std::string getElevationFile() const { return m_elevationFile; }
    inline std::string getTerrainFile() const { return m_terrainFile; }
    inline std::string getGenericFile() const { return m_genericFile; }
    inline std::string getSkyDomeFile() const { return m_skyDomeFile; }
    inline int getVisibility() const { return m_visibility; }
    inline float getSunCoef() const { return m_sunCoef; }

    inline ObjectFiles& getObjects() { return m_objectFiles; }

    inline Routes& getRoutes() { return m_routes; }
    inline Units&  getUnits()  { return m_units; }
    inline Stages& getStages() { return m_stages; }

    Unit* getOwnship();

    Route* getRouteByName( const std::string &routeName );

    int getRouteIndex( const std::string &routeName );

    std::string getRouteName( int routeIndex );

    int getUnitIndex( const std::string &unitName );

    std::string getUnitName( int unitIndex );

    inline void setTutorial( bool tutorial ) { m_tutorial = tutorial; }

    void setMissionIcon( const std::string &missionIcon );
    void setMissionIconLocked( const std::string &missionIconLocked );
    void setMissionImage( const std::string &missionImage );
    void setMissionImageLocked( const std::string &missionImageLocked );
    void setMissionName( const Text &missionName );
    void setMissionIntroduction( const Text &missionIntroduction );
    void setMissionSummaryFailure( const Text &missionSummaryFailure );
    void setMissionSummarySuccess( const Text &missionSummarySuccess );
    void setElevationFile( const std::string &elevationFile );
    void setTerrainFile( const std::string &terrainFile );
    void setGenericFile( const std::string &genericFile );
    void setSkyDomeFile( const std::string &skyDomeFile );
    void setVisibility( int visibility );
    void setSunCoef( float sunCoef );

private:

    bool m_tutorial;                        ///< tutorial mission

    std::string m_missionIcon;              ///<
    std::string m_missionIconLocked;        ///<
    std::string m_missionImage;             ///<
    std::string m_missionImageLocked;       ///<
    Text m_missionName;                     ///< mission name
    Text m_missionIntroduction;             ///< mission introduction
    Text m_missionSummaryFailure;           ///< mission summary failure
    Text m_missionSummarySuccess;           ///< mission summary success
    std::string m_elevationFile;            ///< elevation file
    std::string m_terrainFile;              ///< terrain file
    std::string m_genericFile;              ///< generic terrain file
    ObjectFiles m_objectFiles;              ///< object files
    std::string m_skyDomeFile;              ///< sky dome file

    int m_visibility;                       ///< [m] visibility

    float m_sunCoef;                        ///< [-]

    Routes m_routes;                        ///< routes
    Units  m_units;                         ///< units
    Stages m_stages;                        ///< stages

    bool readMission( QDomElement &xmlNode );
    bool readData( QDomElement &xmlNode );
    bool readScenery( QDomElement &xmlNode );
    bool readRoutes( QDomElement &xmlNode );
    bool readStages( QDomElement &xmlNode );
    bool readUnits( QDomElement &xmlNode );
    bool readUnits( QDomElement &xmlNode, Unit::Affiliation affiliation,
                    bool ownship = false );

    void saveMission( QDomDocument &doc, QDomElement &parentNode );
    void saveData( QDomDocument &doc, QDomElement &parentNode );
    void saveScenery( QDomDocument &doc, QDomElement &parentNode );
    void saveRoutes( QDomDocument &doc, QDomElement &parentNode );
    void saveStages( QDomDocument &doc, QDomElement &parentNode );
    void saveUnits( QDomDocument &doc, QDomElement &parentNode );
    void saveOwnship( QDomDocument &doc, QDomElement &parentNode );
    void saveUnits( QDomDocument &doc, QDomElement &parentNode, Unit::Affiliation affiliation );
};

////////////////////////////////////////////////////////////////////////////////

#endif // MISSION_H
