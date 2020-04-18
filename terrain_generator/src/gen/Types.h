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
#ifndef TYPES_H
#define TYPES_H

////////////////////////////////////////////////////////////////////////////////

#define TILE_MAX_LEVELS 3

#define TILE_MAX_COVER_TYPES 5

#define TILE_MAX_COAST_SQUARES_TYPES 10
#define TILE_MAX_COAST_ROWS_TYPES     2
#define TILE_MAX_COAST_COLS_TYPES     2

#define TILE_MAX_TREES_SQUARES_TYPES 10
#define TILE_MAX_TREES_ROWS_TYPES     2
#define TILE_MAX_TREES_COLS_TYPES     2

////////////////////////////////////////////////////////////////////////////////

enum Level
{
    LevelH = 0,
    LevelM = 1,
    LevelL = 2
};

#if ( TILE_MAX_LEVELS != 3 )
#   error 'TILE_MAX_LEVELS' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

enum CoverType
{
    CoverWater = 0, ///< 0
    CoverGrass,     ///< 1
    CoverCrops,     ///< 2
    CoverTrees,     ///< 3
    CoverUrban      ///< 4
};

#if ( TILE_MAX_COVER_TYPES != 5 )
#   error 'TILE_MAX_COVER_TYPES' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

enum CoastTypeSquares
{
    Coast1NW = 0,   ///< 0
    Coast1NE,       ///< 1
    Coast1SE,       ///< 2
    Coast1SW,       ///< 3

    Coast3NW,       ///< 4
    Coast3NE,       ///< 5
    Coast3SE,       ///< 6
    Coast3SW,       ///< 7

    CoastNESW,      ///< 8
    CoastSENW       ///< 9
};

#if ( TILE_MAX_COAST_SQUARES_TYPES != 10 )
#   error 'TILE_MAX_COAST_SQUARES_TYPES' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

enum CoastTypeRows
{
    CoastN = 0,     ///< 0
    CoastS          ///< 1
};

#if ( TILE_MAX_COAST_ROWS_TYPES != 2 )
#   error 'TILE_MAX_COAST_ROWS_TYPES' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

enum CoastTypeCols
{
    CoastE = 0,     ///< 0
    CoastW,         ///< 1
};

#if ( TILE_MAX_COAST_COLS_TYPES != 2 )
#   error 'TILE_MAX_COAST_COLS_TYPES' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

enum TreesTypeSquares
{
    Trees1NW = 0,   ///< 0
    Trees1NE,       ///< 1
    Trees1SE,       ///< 2
    Trees1SW,       ///< 3

    Trees3NW,       ///< 4
    Trees3NE,       ///< 5
    Trees3SE,       ///< 6
    Trees3SW,       ///< 7

    TreesNESW,      ///< 8
    TreesSENW       ///< 9
};

#if ( TILE_MAX_TREES_SQUARES_TYPES != 10 )
#   error 'TILE_MAX_TREES_SQUARES_TYPES' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

enum TreesTypeRows
{
    TreesN = 0,     ///< 0
    TreesS          ///< 1
};

#if ( TILE_MAX_TREES_ROWS_TYPES != 2 )
#   error 'TILE_MAX_TREES_ROWS_TYPES' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

enum TreesTypeCols
{
    TreesE = 0,     ///< 0
    TreesW,         ///< 1
};

#if ( TILE_MAX_TREES_COLS_TYPES != 2 )
#   error 'TILE_MAX_TREES_COLS_TYPES' has been changed! Check code above this line!
#endif

////////////////////////////////////////////////////////////////////////////////

#endif // TYPES_H
