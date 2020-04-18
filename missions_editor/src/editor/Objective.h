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
#ifndef OBJECTIVE_H
#define OBJECTIVE_H

////////////////////////////////////////////////////////////////////////////////

#include <QDomDocument>
#include <QDomElement>

////////////////////////////////////////////////////////////////////////////////

/** Abstract objective base class. */
class Objective
{
public:

    /**
     * Reads objective from XML file.
     * @param xmlNode objective XML node
     * @return returns objective object on success and null on failure
     */
    static Objective* readObjective( QDomElement &xmlNode );

    /** Destructor. */
    virtual ~Objective() {}

    virtual Objective* clone() = 0;

    /** */
    virtual const char* getTypeName() = 0;

    /**
     * Saves objective to XML file.
     * @param doc XML document
     * @param parentNode XML parent node
     */
    virtual void saveObjective( QDomDocument &doc, QDomElement &parentNode ) = 0;
};

////////////////////////////////////////////////////////////////////////////////

#endif // OBJECTIVE_H
