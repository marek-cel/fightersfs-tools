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
#ifndef OBJECTIVEDEFEND_H
#define OBJECTIVEDEFEND_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <editor/Objective.h>

////////////////////////////////////////////////////////////////////////////////

/** Defend objective class. */
class ObjectiveDefend : public Objective
{
public:

    typedef std::vector< std::string > Units;

    /**
     * Reads objective from XML file.
     * @param xmlNode objective XML node
     * @return returns objective object on success and null on failure
     */
    static ObjectiveDefend* readObjective( QDomElement &xmlNode );

    Objective* clone();

    /** */
    const char* getTypeName() { return "Defend"; }

    Units& getUnits() { return m_units; }

    int getMinimum() { return m_minimum; }

    void setUnits( const Units &units ) { m_units = units; }

    void setMinimum( int minimum ) { m_minimum = minimum; }

    /**
     * Saves objective to XML file.
     * @param doc XML document
     * @param parentNode XML parent node
     */
    void saveObjective( QDomDocument &doc, QDomElement &parentNode );

private:

    Units m_units;  ///< units to be defended

    int m_minimum;  ///< minimum units
};

////////////////////////////////////////////////////////////////////////////////

#endif // OBJECTIVEDEFEND_H
