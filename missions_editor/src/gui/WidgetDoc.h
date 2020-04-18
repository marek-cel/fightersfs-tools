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
#ifndef WIDGETDOC_H
#define WIDGETDOC_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Node>

#include <QWidget>
#include <QListWidgetItem>

#include <editor/Mission.h>
#include <cgi/SceneRoot.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class WidgetDoc;
}

////////////////////////////////////////////////////////////////////////////////

/** Document (mission) editing widget class. */
class WidgetDoc : public QWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit WidgetDoc( QWidget *parent = 0 );

    /** Destructor. */
    ~WidgetDoc();

    void newEmpty();

    bool readFile( QString file );
    bool saveFile( QString file );

signals:

    void changed();

    void sceneryReloaded();

private:

    Ui::WidgetDoc *m_ui;

    int m_currentObjectIndex;
    int m_currentRouteIndex;
    int m_currentUnitIndex;
    int m_currentStageIndex;

    void editObject();
    void editRoute();
    void editStage();
    void editUnit();

    void updateGUI();

private slots:

    void on_pageRoute_changed();
    void on_pageUnit_changed();
    void on_pageStage_changed();

    void on_pageRoute_done();
    void on_pageUnit_done();
    void on_pageStage_done();

    void on_comboBoxLang_currentIndexChanged(int index);

    void on_toolBox_currentChanged(int index);

    void on_checkBoxTutorial_toggled(bool checked);

    void on_lineEditIcon_textChanged(const QString &arg1);
    void on_lineEditIconLocked_textChanged(const QString &arg1);
    void on_lineEditImage_textChanged(const QString &arg1);
    void on_lineEditImageLocked_textChanged(const QString &arg1);
    void on_lineEditName_textChanged(const QString &arg1);
    void on_textEditIntroduction_textChanged();
    void on_textEditSummaryFailure_textChanged();
    void on_textEditSummarySuccess_textChanged();

    void on_lineEditTerrain_textChanged(const QString &arg1);
    void on_lineEditGeneric_textChanged(const QString &arg1);
    void on_lineEditElevation_textChanged(const QString &arg1);
    void on_lineEditSkyDome_textChanged(const QString &arg1);
    void on_pushButtonTerrain_clicked();
    void on_pushButtonGeneric_clicked();
    void on_pushButtonElevation_clicked();
    void on_pushButtonSkyDome_clicked();
    void on_spinBoxVisibility_valueChanged(int arg1);
    void on_spinBoxSunCoef_valueChanged(double arg1);
    void on_pushButtonObjectAdd_clicked();
    void on_pushButtonObjectEdit_clicked();
    void on_pushButtonObjectRemove_clicked();
    void on_listWidgetObjects_currentRowChanged(int currentRow);
    void on_listWidgetObjects_doubleClicked(const QModelIndex &);

    void on_pushButtonRouteAdd_clicked();
    void on_pushButtonRouteEdit_clicked();
    void on_pushButtonRouteRemove_clicked();
    void on_listWidgetRoutes_currentRowChanged(int currentRow);
    void on_listWidgetRoutes_doubleClicked(const QModelIndex &);

    void on_pushButtonUnitAdd_clicked();
    void on_pushButtonUnitEdit_clicked();
    void on_pushButtonUnitRemove_clicked();
    void on_listWidgetUnits_currentRowChanged(int currentRow);
    void on_listWidgetUnits_doubleClicked(const QModelIndex &);

    void on_pushButtonStageAdd_clicked();
    void on_pushButtonStageEdit_clicked();
    void on_pushButtonStageRemove_clicked();
    void on_listWidgetStages_currentRowChanged(int currentRow);
    void on_listWidgetStages_doubleClicked(const QModelIndex &);
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETDOC_H
