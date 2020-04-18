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
#ifndef MODELS_H
#define MODELS_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Node>

#include "Singleton.h"

////////////////////////////////////////////////////////////////////////////////

/** 3D models. */
class Models : public Singleton< Models >
{
    friend class Singleton< Models >;

public:

    typedef std::vector< osg::ref_ptr<osg::Node> > List;

    /** */
    static osg::Node* get( std::string objectFile );

    static void reset();

    /** Destructor. */
    virtual ~Models();

private:

    List m_objects;
    std::vector< std::string > m_fileNames;

    /**
     * You should use static function getInstance() due to get refernce
     * to Models class instance.
     */
    Models();

    /** Using this constructor is forbidden. */
    Models( const Models & ) {}
};

////////////////////////////////////////////////////////////////////////////////

#endif // MODELS_H
