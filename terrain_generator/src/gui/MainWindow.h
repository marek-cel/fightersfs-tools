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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

////////////////////////////////////////////////////////////////////////////////

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>

#include "RecentFileAction.h"

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class MainWindow;
}

////////////////////////////////////////////////////////////////////////////////

/** */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    /** */
    explicit MainWindow(QWidget *parent = 0);

    /** */
    virtual ~MainWindow();

protected:

    /** */
    void closeEvent( QCloseEvent *event );

    /** */
    void keyPressEvent( QKeyEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );
    
private:

    Ui::MainWindow *m_ui;       ///<

    QShortcut *m_shortcutSave;
    QShortcut *m_shortcutRefresh;

    bool m_saved;               ///<
    int m_timerId;              ///<

    QString m_currentFile;      ///<

    QStringList m_recentFilesList;  ///<
    std::vector< RecentFileAction* > m_recentFilesActions;

    void askIfSave();

    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void exportFileAs();
    void generateElevation();

    void readFile( QString fileName );
    void saveFile( QString fileName );
    void exportAs( QString fileName );
    void generateElevation( QString fileName );

    void settingsRead();
    void settingsRead_RecentFiles( QSettings &settings );

    void settingsSave();
    void settingsSave_RecentFiles( QSettings &settings );

    void updateGUI();
    void updateRecentFiles( QString file = "" );

public slots:

    void document_changed();
    void recentFile_triggered( int id );

private slots:

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExport_triggered();
    void on_actionGenerate_triggered();
    void on_actionExit_triggered();

    void on_actionClearRecent_triggered();

    void on_actionRefresh_triggered();

    void on_actionViewOrbit_triggered();
    void on_actionViewWorld_triggered();
};

////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
