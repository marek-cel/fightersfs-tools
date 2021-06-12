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

#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include <defs.h>
#include <gui/MainWindow.h>
#include <ui_MainWindow.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow ( parent ),
    m_ui ( new Ui::MainWindow ),

    m_shortcutSave ( 0 ),

    m_saved ( true ),
    m_timerId ( 0 )
{
    m_ui->setupUi( this );

    m_shortcutSave = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(on_actionSave_triggered()) );

    settingsRead();

    m_timerId = startTimer( 50 );

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    killTimer( m_timerId );

    settingsSave();

    if ( m_shortcutSave ) delete m_shortcutSave;
    m_shortcutSave = 0;

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::closeEvent( QCloseEvent *event )
{
    askIfSave();

    /////////////////////////////////
    QMainWindow::closeEvent( event );
    /////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::keyPressEvent( QKeyEvent *event )
{
    ////////////////////////////////////
    QMainWindow::keyPressEvent( event );
    ////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    if ( m_ui->widgetCGI->isVisible() )
    {
        m_ui->widgetCGI->update();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::askIfSave()
{
    if ( !m_saved )
    {
        QString title = windowTitle();
        QString text = "File have unsaved changes.";

        QMessageBox::StandardButton result = QMessageBox::question( this, title, text,
                                                                    QMessageBox::Save | QMessageBox::Discard,
                                                                    QMessageBox::Save );

        if ( result == QMessageBox::Save )
        {
            saveFile();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::newFile()
{
    askIfSave();

    m_currentFile = "";

    m_ui->widgetDoc->newEmpty();

    SceneRoot::getInstance()->reloadRoutes();
    SceneRoot::getInstance()->reloadUnits();
    SceneRoot::getInstance()->reloadScenery();

    m_ui->widgetCGI->setViewWorld();

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::openFile()
{
    askIfSave();

    QString caption = "Open...";
    QString dir = ( m_currentFile.length() > 0 ) ? QFileInfo( m_currentFile ).absolutePath() : "data/missions";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "XML (*.xml)";

    QString file = QFileDialog::getOpenFileName( this, caption, dir, filter, &selectedFilter );

    if ( file.length() > 0 )
    {
        m_currentFile = file;

        updateRecentFiles( m_currentFile );
        readFile( m_currentFile );
    }

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::saveFile()
{
    if ( m_currentFile.length() > 0 )
    {
        saveFile( m_currentFile );
    }
    else
    {
        saveFileAs();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::saveFileAs()
{
    QString caption = "Save as...";
    QString dir = ( m_currentFile.length() > 0 ) ? QFileInfo( m_currentFile ).absolutePath() : ".";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "XML (*.xml)";

    QString newFile = QFileDialog::getSaveFileName( this, caption, dir, filter, &selectedFilter );

    if ( newFile.length() > 0 )
    {
        m_currentFile = newFile;
        saveFile( m_currentFile );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::readFile( QString fileName )
{
    if ( QFileInfo( fileName ).suffix() == QString( "xml" ) )
    {
        if ( !m_ui->widgetDoc->readFile( fileName ) )
        {
            QMessageBox::warning( this, tr( APP_TITLE ),
                                  tr( "Cannot read file %1:." ).arg( fileName ) );
        }
        else
        {
            m_saved = true;
        }
    }

    updateGUI();

    m_ui->widgetCGI->setViewWorld();

    SceneRoot::getInstance()->reloadRoutes();
    SceneRoot::getInstance()->reloadUnits();
    SceneRoot::getInstance()->reloadScenery();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::saveFile( QString fileName )
{
    if ( m_ui->widgetDoc->saveFile( fileName ) )
    {
        m_saved = true;
    }
    else
    {
        QMessageBox::warning( this, tr( APP_TITLE ),
                              tr( "Cannot save file %1:." ).arg(fileName) );
    }

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead()
{
    QSettings settings( ORG_NAME, APP_NAME );

    settings.beginGroup( "main_window" );

    restoreState( settings.value( "state" ).toByteArray() );
    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    settingsRead_RecentFiles( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead_RecentFiles( QSettings &settings )
{
    m_recentFilesList = settings.value( "recent_files" ).toStringList();

    updateRecentFiles();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave()
{
    QSettings settings( ORG_NAME, APP_NAME );

    settings.beginGroup( "main_window" );

    settings.setValue( "state"    , saveState() );
    settings.setValue( "geometry" , saveGeometry() );

    settingsSave_RecentFiles( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave_RecentFiles( QSettings &settings )
{
    settings.setValue( "recent_files", m_recentFilesList );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateGUI()
{
    QString title = tr( APP_TITLE );

    if ( m_currentFile.length() > 0 )
    {
        title += " - " + QFileInfo( m_currentFile ).fileName();
    }

    if ( !m_saved ) title += " (*)";

    setWindowTitle( title );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateRecentFiles( QString file )
{
    if ( file.length() > 0 )
    {
        QStringList temp;

        temp.push_back( file );

        for ( int i = 0; i < m_recentFilesList.size() && i < 4; i++ )
        {
            temp.push_back( m_recentFilesList.at( i ) );
        }

        m_recentFilesList = temp;
    }

    m_recentFilesActions.clear();
    m_ui->menuRecentFiles->clear();

    for ( int i = 0; i < m_recentFilesList.size(); i++ )
    {
        m_recentFilesActions.push_back( new RecentFileAction( i, m_recentFilesList.at( i ), m_ui->menuRecentFiles ) );
        connect( m_recentFilesActions.at( i ), SIGNAL(triggered(int)), this, SLOT(recentFile_triggered(int)) );
        m_ui->menuRecentFiles->addAction( m_recentFilesActions.at( i ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_widgetDoc_changed()
{
    m_saved = false;
    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_widgetDoc_sceneryReloaded()
{
    m_ui->widgetCGI->setViewWorld();

    on_widgetDoc_changed();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::recentFile_triggered( int id )
{
    m_currentFile = m_recentFilesList.at( id );

    readFile( m_currentFile );
    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionNew_triggered()
{
    newFile();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionOpen_triggered()
{
    openFile();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionClearRecent_triggered()
{
    m_recentFilesList.clear();
    m_recentFilesActions.clear();
    m_ui->menuRecentFiles->clear();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionSave_triggered()
{
    saveFile();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionSaveAs_triggered()
{
    saveFileAs();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionExit_triggered()
{
    close();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewOrbit_triggered()
{
    m_ui->widgetCGI->setViewOrbit();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewWorld_triggered()
{
    m_ui->widgetCGI->setViewWorld();
}
