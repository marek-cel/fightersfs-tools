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
#ifndef OBJECTIVEAIRSPEED_H
#define OBJECTIVEAIRSPEED_H

////////////////////////////////////////////////////////////////////////////////

#include <editor/Objective.h>

////////////////////////////////////////////////////////////////////////////////

/** Airspeed objective class. */
class ObjectiveAirspeed : public Objective
{
public:

    /**
     * Reads objective from XML file.
     * @param xmlNode objective XML node
     * @return returns objective object on success and null on failure
     */
    static ObjectiveAirspeed* readObjective( QDomElement &xmlNode );

    /** */
    ObjectiveAirspeed();

    Objective* clone();

    /**
     * Saves objective to XML file.
     * @param doc XML document
     * @param parentNode XML parent node
     */
    void saveObjective( QDomDocument &doc, QDomElement &parentNode );

    /** */
    const char* getTypeName() { return "Airspeed"; }

    inline float getAbove() const { return m_above; }
    inline float getBelow() const { return m_below; }

    inline void setAbove( float val ) { m_above = val; }
    inline void setBelow( float val ) { m_below = val; }

private:

    float m_above;
    float m_below;
};

////////////////////////////////////////////////////////////////////////////////

#endif // OBJECTIVEAIRSPEED_H
