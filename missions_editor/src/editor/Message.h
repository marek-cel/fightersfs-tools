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
#ifndef MESSAGE_H
#define MESSAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <QDomDocument>
#include <QDomElement>

#include "Text.h"

////////////////////////////////////////////////////////////////////////////////

/** Stage message class. */
class Message
{
public:

    typedef std::vector< Text > Lines;

    /**
     * Reads message from XML file.
     * @param xmlNode message XML node
     * @return returns message object on success and null on failure
     */
    static Message* readMessage( QDomElement &xmlNode );

    /** Constructor. */
    Message();

    /** Destructor. */
    virtual ~Message();

    /**
     * Saves message to XML file.
     * @param doc XML document
     * @param parentNode XML parent node
     */
    void saveMessage( QDomDocument &doc, QDomElement &parentNode );

    inline bool getOverlay() const { return m_overlay; }

    inline double getDuration() const { return m_duration; }
    inline double getDelay() const { return m_delay; }

    inline Lines getLines() const { return m_lines; }

    inline bool getPointerCustom()  const { return m_pointer_custom;  }
    inline bool getPointerTarget()  const { return m_pointer_target;  }
    inline bool getPointerRpmDec()  const { return m_pointer_rpm_dec; }
    inline bool getPointerRpmInc()  const { return m_pointer_rpm_inc; }
    inline bool getPointerTrigger() const { return m_pointer_trigger; }

    inline double getPointerX() const { return m_pointer_x; }
    inline double getPointerY() const { return m_pointer_y; }
    inline double getPointerPhi() const { return m_pointer_phi; }

    inline int getTutorialTip() const { return m_tutorial_tip; }

    inline void setOverlay( bool overlay ) { m_overlay = overlay; }
    inline void setDuration( double duration ) { m_duration = duration; }
    inline void setDelay( double delay ) { m_delay = delay; }
    inline void setLines( const Lines &lines ) { m_lines = lines; }

    inline void setPointerCustom( bool val )  { m_pointer_custom  = val; }
    inline void setPointerTarget( bool val )  { m_pointer_target  = val; }
    inline void setPointerRpmDec( bool val )  { m_pointer_rpm_dec = val; }
    inline void setPointerRpmInc( bool val )  { m_pointer_rpm_inc = val; }
    inline void setPointerTrigger( bool val ) { m_pointer_trigger = val; }

    inline void setPointerX( double val ) { m_pointer_x = val; }
    inline void setPointerY( double val ) { m_pointer_y = val; }
    inline void setPointerPhi( double val ) { m_pointer_phi = val; }

    inline void setTutorialTip( int val ) { m_tutorial_tip = val; }

private:

    bool m_overlay;

    double m_duration;  ///< [s]
    double m_delay;     ///< [s]

    Lines m_lines;

    bool m_pointer_custom;
    bool m_pointer_target;
    bool m_pointer_rpm_dec; ///< specifies if decrease RPM pointer is visible
    bool m_pointer_rpm_inc; ///< specifies if increase RPM pointer is visible
    bool m_pointer_trigger; ///< specifies if trigger pointer is visible

    double m_pointer_x;
    double m_pointer_y;
    double m_pointer_phi;

    int m_tutorial_tip;
};

////////////////////////////////////////////////////////////////////////////////

#endif // MESSAGE_H
