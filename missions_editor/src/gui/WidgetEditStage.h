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
#ifndef WIDGETEDITSTAGE_H
#define WIDGETEDITSTAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>
#include <QListWidgetItem>

#include <editor/Mission.h>

#include <gui/DialogEditMessage.h>

#include <gui/DialogEditObjectiveAirspeed.h>
#include <gui/DialogEditObjectiveAltitude.h>
#include <gui/DialogEditObjectiveDefend.h>
#include <gui/DialogEditObjectiveDestroy.h>
#include <gui/DialogEditObjectiveDistance.h>
#include <gui/DialogEditObjectiveHeading.h>
#include <gui/DialogEditObjectiveTimeout.h>
#include <gui/DialogEditObjectiveWaypoint.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class WidgetEditStage;
}

////////////////////////////////////////////////////////////////////////////////

/** Stage editing widget class. */
class WidgetEditStage : public QWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit WidgetEditStage( QWidget *parent = 0 );

    /** Destructor. */
    ~WidgetEditStage();

    void edit( int stageIndex );

signals:

    void changed();

    void done();

private:

    Ui::WidgetEditStage *m_ui;

    bool m_inited;

    DialogEditMessage *m_dialogEditMessage;

    DialogEditObjectiveAirspeed *m_dialogEditObjectiveAirspeed;
    DialogEditObjectiveAltitude *m_dialogEditObjectiveAltitude;
    DialogEditObjectiveDefend   *m_dialogEditObjectiveDefend;
    DialogEditObjectiveDestroy  *m_dialogEditObjectiveDestroy;
    DialogEditObjectiveDistance *m_dialogEditObjectiveDistance;
    DialogEditObjectiveHeading  *m_dialogEditObjectiveHeading;
    DialogEditObjectiveTimeout  *m_dialogEditObjectiveTimeout;
    DialogEditObjectiveWaypoint *m_dialogEditObjectiveWaypoint;

    int m_currentMessageIndex;
    int m_currentInitUnitIndex;
    int m_currentObjectiveIndex;

    int m_stageIndex;
    Stage *m_stage;

    void editMessage();
    void editObjective();

    void updateGUI();

private slots:

    void on_lineEditStageName_editingFinished();
    void on_spinBoxTimeLimit_valueChanged(double arg1);

    void on_pushButtonMessagesAdd_clicked();
    void on_pushButtonMessagesEdit_clicked();
    void on_pushButtonMessagesRemove_clicked();
    void on_listWidgetMessages_currentRowChanged(int currentRow);
    void on_listWidgetMessages_doubleClicked(const QModelIndex &);

    void on_pushButtonInitUnitAdd_clicked();
    void on_pushButtonInitUnitRemove_clicked();
    void on_listWidgetInitUnits_currentRowChanged(int currentRow);

    void on_pushButtonObjAdd_clicked();
    void on_pushButtonObjEdit_clicked();
    void on_pushButtonObjRemove_clicked();
    void on_listWidgetObjectives_currentRowChanged(int currentRow);
    void on_listWidgetObjectives_doubleClicked(const QModelIndex &);

    void on_pushButtonDone_clicked();
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETEDITSTAGE_H
