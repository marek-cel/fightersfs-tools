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
#ifndef CONVERTER_H
#define CONVERTER_H

////////////////////////////////////////////////////////////////////////////////

/** Units converter class. */
class Converter
{
public:

    /**
     * Converts given angle from degrees to radians.
     * @param angle angle expressed in degrees
     * @return angle expressed in radians
     */
    static inline double deg2rad( double angle = 1.0 )
    {
        return angle * M_PI / 180.0;
    }

    /**
     * Converts given angle from radians to degrees.
     * @param angle angle expressed in radians
     * @return angle expressed in degrees
     */
    static inline double rad2deg( double angle = 1.0 )
    {
        return angle * 180.0 / M_PI;
    }

    /**
     * Converts given length from feet to metres.
     * @param length length expressed in feet
     * @return length expressed in metres
     */
    static inline float ft2m( float length = 1.0f )
    {
        return length * 0.3048f;
    }

    /**
     * Converts given length from metres to feet.
     * @param length length expressed in metres
     * @return length expressed in feet
     */
    static inline float m2ft( float length = 1.0f )
    {
        return length * 3.2808399f;
    }

    /**
     * Converts given length from metres to nautical miles.
     * @param length length expressed in metres
     * @return length expressed in nautical miles
     */
    static inline float m2nm( float length = 1.0f )
    {
        return length * 0.000539956803f;
    }

    /**
     * Converts given length from nautical miles to metres.
     * @param length length expressed in nautical miles
     * @return length expressed in metres
     */
    static inline float nm2m( float length = 1.0 )
    {
        return length * 1852.0f;
    }

    /**
     * Converts given velocity from m/s to ft/min.
     * @param velocity velocity expressed in m/s
     * @return velocity expressed in ft/min
     */
    static inline float mps2fpm( float vel = 1.0f )
    {
        return vel * 196.850394f;
    }

    /**
     * Converts given velocity from m/s to knots.
     * @param velocity velocity expressed in m/s
     * @return velocity expressed in knots
     */
    static inline float mps2kts( float vel = 1.0f )
    {
        return vel * 1.943844491f;
    }

    /**
     * Converts given velocity from kntos to m/s.
     * @param velocity velocity expressed in knots
     * @return velocity expressed in m/s
     */
    static inline double kts2mps( double vel = 1.0 )
    {
        return vel * 0.5144444444444;
    }
};

////////////////////////////////////////////////////////////////////////////////

#endif // CONVERTER_H
