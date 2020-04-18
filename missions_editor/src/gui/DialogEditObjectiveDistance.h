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
#ifndef DIALOGEDITOBJECTIVEDISTANCE_H
#define DIALOGEDITOBJECTIVEDISTANCE_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>

#include <editor/Mission.h>
#include <editor/ObjectiveDistance.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogEditObjectiveDistance;
}

////////////////////////////////////////////////////////////////////////////////

/** Airspeed objective editing dialog class. */
class DialogEditObjectiveDistance : public QDialog
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DialogEditObjectiveDistance( QWidget *parent = 0 );

    /** Destructor. */
    ~DialogEditObjectiveDistance();

    void setObj( ObjectiveDistance *obj );

public slots:

    void accept();

private:

    Ui::DialogEditObjectiveDistance *m_ui;

    ObjectiveDistance *m_obj;

    float m_limit;

    QString m_leader;

    bool m_inited;

private slots:


    void on_spinBoxLimit_editingFinished();
    void on_comboBoxLeader_currentIndexChanged(int index);
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGEDITOBJECTIVEDISTANCE_H
