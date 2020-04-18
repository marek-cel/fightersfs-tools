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
#ifndef COLORS_H
#define COLORS_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Vec4>

////////////////////////////////////////////////////////////////////////////////

class Colors
{
public:

    static const osg::Vec4 Black;       ///< black
    static const osg::Vec4 White;       ///< white
    static const osg::Vec4 Red;         ///< red
    static const osg::Vec4 Green;       ///< green
    static const osg::Vec4 Grey;        ///< grey
    static const osg::Vec4 Blue;        ///< blue
    static const osg::Vec4 Cyan;        ///< cyan
    static const osg::Vec4 Magenta;     ///< magenta
    static const osg::Vec4 Yellow;      ///< yellow
    static const osg::Vec4 Fuchsia;     ///< fuchsia
};

////////////////////////////////////////////////////////////////////////////////

#endif // COLORS_H
