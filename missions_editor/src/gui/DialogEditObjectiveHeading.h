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
#ifndef DIALOGEDITOBJECTIVEHEADING_H
#define DIALOGEDITOBJECTIVEHEADING_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>

#include <editor/Mission.h>
#include <editor/ObjectiveHeading.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogEditObjectiveHeading;
}

////////////////////////////////////////////////////////////////////////////////

/** Airspeed objective editing dialog class. */
class DialogEditObjectiveHeading : public QDialog
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DialogEditObjectiveHeading( QWidget *parent = 0 );

    /** Destructor. */
    ~DialogEditObjectiveHeading();

    void setObj( ObjectiveHeading *obj );

public slots:

    void accept();

private:

    Ui::DialogEditObjectiveHeading *m_ui;

    ObjectiveHeading *m_obj;

    float m_above;
    float m_below;

private slots:


    void on_spinBoxAbove_editingFinished();
    void on_spinBoxBelow_editingFinished();
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGEDITOBJECTIVEHEADING_H
