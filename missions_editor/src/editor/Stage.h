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
#ifndef STAGE_H
#define STAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <QDomDocument>
#include <QDomElement>

#include <editor/Message.h>
#include <editor/Objective.h>

////////////////////////////////////////////////////////////////////////////////

/** Mission stage class. */
class Stage
{
public:

    typedef std::vector< std::string > InitUnits;
    typedef std::vector< Objective* > Objectives;
    typedef std::vector< Message* > Messages;

    /**
     * Reads stage from XML file.
     * @param xmlNode stage XML node
     * @return returns stage object on success and null on failure
     */
    static Stage* readStage( QDomElement &xmlNode );

    /** Constructor. */
    Stage();

    /** Destructor. */
    virtual ~Stage();

    /**
     * Saves stage to XML file.
     * @param doc XML document
     * @param parentNode XML parent node
     */
    void saveStage( QDomDocument &doc, QDomElement &parentNode );

    std::string getName() const { return m_name; }

    float getTimeLimit() const { return m_timeLimit; }

    InitUnits& getInitUnits() { return m_initUnits; }
    Messages& getMessages() { return m_messages; }
    Objectives& getObjectives() { return m_objectives; }

    void setName( const std::string &name ) { m_name = name; }

    void setTimeLimit( float timeLimit ) { m_timeLimit = timeLimit; }

private:

    std::string m_name;         ///< stage name

    float m_timeLimit;          ///< [s] stage time limit

    InitUnits m_initUnits;      ///< list of units to be inited when stage starts
    Messages m_messages;        ///< stage messages
    Objectives m_objectives;    ///< list of stage objectives
};

////////////////////////////////////////////////////////////////////////////////

#endif // STAGE_H
