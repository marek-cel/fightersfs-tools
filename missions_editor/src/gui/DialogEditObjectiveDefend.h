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
#ifndef DIALOGEDITOBJECTIVEDEFEND_H
#define DIALOGEDITOBJECTIVEDEFEND_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>

#include <editor/Mission.h>
#include <editor/ObjectiveDefend.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogEditObjectiveDefend;
}

////////////////////////////////////////////////////////////////////////////////

/** Defend objective editing dialog class. */
class DialogEditObjectiveDefend : public QDialog
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DialogEditObjectiveDefend( QWidget *parent = 0 );

    /** Destructor. */
    ~DialogEditObjectiveDefend();

    void setObj( ObjectiveDefend *obj );

public slots:

    void accept();

private:

    Ui::DialogEditObjectiveDefend *m_ui;

    ObjectiveDefend *m_obj;

    int m_currentUnitIndex;
    ObjectiveDefend::Units m_units;

private slots:

    void on_pushButtonDefendUnitAdd_clicked();
    void on_pushButtonDefendUnitRemove_clicked();

    void on_listWidgetDefendUnits_currentRowChanged(int currentRow);
    void on_spinBoxMinimum_valueChanged(int arg1);
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGEDITOBJECTIVEDEFEND_H
