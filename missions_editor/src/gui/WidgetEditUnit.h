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
#ifndef WIDGETEDITUNIT_H
#define WIDGETEDITUNIT_H

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>

#include <editor/Mission.h>
#include <cgi/SceneRoot.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class WidgetEditUnit;
}

////////////////////////////////////////////////////////////////////////////////

/** Unit editing widget class. */
class WidgetEditUnit : public QWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit WidgetEditUnit( QWidget *parent = 0 );

    /** Destructor. */
    ~WidgetEditUnit();

    void edit( int unitIndex );

signals:

    void changed();

    void done();

private:

    Ui::WidgetEditUnit *m_ui;

    bool m_inited;

    int m_unitIndex;
    Unit *m_unit;

    void updateGUI();

private slots:

    void on_lineEditUnitName_editingFinished();

    void on_radioButtonFriend_toggled(bool checked);
    void on_radioButtonHostile_toggled(bool checked);
    void on_radioButtonNeutral_toggled(bool checked);

    void on_comboBoxUnitType_currentIndexChanged(int index);

    void on_lineEditUnitFile_textChanged(const QString &arg1);
    void on_pushButtonUnitFile_clicked();

    void on_spinBoxUnitHP_valueChanged(int arg1);

    void on_lineEditUnitLivery_textChanged(const QString &arg1);
    void on_pushButtonUnitLivery_clicked();

    void on_checkBoxUnique_toggled(bool checked);

    void on_checkBoxWingman_toggled(bool checked);
    void on_comboBoxLeader_currentIndexChanged(int index);
    void on_spinBoxLeaderX_editingFinished();
    void on_spinBoxLeaderY_editingFinished();
    void on_spinBoxLeaderZ_editingFinished();

    void on_checkBoxUnitRoute_toggled(bool checked);
    void on_comboBoxUnitRoute_currentIndexChanged(int index);

    void on_spinBoxUnitPositionX_editingFinished();
    void on_spinBoxUnitPositionY_editingFinished();
    void on_spinBoxUnitPositionZ_editingFinished();
    void on_spinBoxUnitPositionZ_valueChanged(double arg1);

    void on_spinBoxUnitSpeed_editingFinished();
    void on_spinBoxUnitSpeed_valueChanged(double arg1);

    void on_spinBoxUnitHeading_editingFinished();

    void on_checkBoxOwnship_toggled(bool checked);

    void on_pushButtonDone_clicked();
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETEDITUNIT_H
