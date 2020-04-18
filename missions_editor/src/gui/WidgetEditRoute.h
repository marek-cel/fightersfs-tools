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
#ifndef WIDGETEDITROUTE_H
#define WIDGETEDITROUTE_H

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>
#include <QListWidgetItem>

#include <editor/Mission.h>
#include <cgi/SceneRoot.h>

#include <gui/DialogEditWaypoint.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class WidgetEditRoute;
}

////////////////////////////////////////////////////////////////////////////////

/** Route editing widget class. */
class WidgetEditRoute : public QWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit WidgetEditRoute( QWidget *parent = 0 );

    /** Destructor. */
    ~WidgetEditRoute();

    void edit( int routeIndex );

signals:

    void changed();

    void done();

private:

    Ui::WidgetEditRoute *m_ui;

    bool m_inited;

    DialogEditWaypoint *m_dialogEditWaypoint;

    int m_routeIndex;
    int m_currentWaypointIndex;
    Route *m_route;

    void editWaypoint();

    void updateGUI();

private slots:

    void on_lineEditRouteName_editingFinished();

    void on_pushButtonAdd_clicked();
    void on_pushButtonEdit_clicked();
    void on_pushButtonRemove_clicked();

    void on_listWidgetWaypoints_currentRowChanged(int currentRow);
    void on_listWidgetWaypoints_doubleClicked(const QModelIndex &);

    void on_pushButtonDone_clicked();
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETEDITROUTE_H
