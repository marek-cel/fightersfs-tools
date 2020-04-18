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
#ifndef DIALOGEDITOBJECTIVEWAYPOINT_H
#define DIALOGEDITOBJECTIVEWAYPOINT_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>

#include <editor/Mission.h>
#include <editor/ObjectiveWaypoint.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogEditObjectiveWaypoint;
}

////////////////////////////////////////////////////////////////////////////////

/** Waypoint objective editing dialog class. */
class DialogEditObjectiveWaypoint : public QDialog
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DialogEditObjectiveWaypoint( QWidget *parent = 0 );

    /** Destructor. */
    ~DialogEditObjectiveWaypoint();

    void setObj( ObjectiveWaypoint *obj );

public slots:

    void accept();

private:

    Ui::DialogEditObjectiveWaypoint *m_ui;

    ObjectiveWaypoint *m_obj;

    int m_index;

private slots:

    void on_spinBox_valueChanged(int arg1);
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGEDITOBJECTIVEWAYPOINT_H
