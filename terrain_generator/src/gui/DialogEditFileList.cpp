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

#include <QFileDialog>

#include <gui/DialogEditFileList.h>
#include <ui_DialogEditFileList.h>

////////////////////////////////////////////////////////////////////////////////

int DialogEditFileList::editFileList( QVector< QString > &fileList, QWidget *parent )
{
    DialogEditFileList *dialog = 0;

    dialog = new DialogEditFileList( fileList, parent );

    int result = dialog->exec();

    if ( result == QDialog::Accepted )
    {
        fileList.clear();
        fileList = dialog->m_fileList;
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

DialogEditFileList::DialogEditFileList( QVector< QString > fileList, QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::DialogEditFileList ),

    m_fileList ( fileList )
{
    m_ui->setupUi( this );

    updateListWidget();
}

////////////////////////////////////////////////////////////////////////////////

DialogEditFileList::~DialogEditFileList()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditFileList::updateListWidget()
{
    m_ui->listWidget->clear();

    for ( int i = 0; i < m_fileList.size(); i++ )
    {
        m_ui->listWidget->addItem( new QListWidgetItem( m_fileList.at( i ), m_ui->listWidget ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditFileList::on_listWidget_currentRowChanged( int currentRow )
{
    if ( currentRow > -1
      && currentRow < m_fileList.size() )
    {
        m_ui->pushButtonRemove->setEnabled( true );
    }
    else
    {
        m_ui->pushButtonRemove->setEnabled( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditFileList::on_pushButtonAdd_clicked()
{
    QString caption = "Open...";
    QString dir = "";
    QString filter;
    QString selectedFilter;

//    filter += selectedFilter = "TIFF (*.tif *.tiff)";
//    filter += "OpenSceneGraph (*.osg *.osgb *osgt)";

    QString newFile = QFileDialog::getOpenFileName( this, caption, dir, filter, &selectedFilter );
    QString workDir = QDir::currentPath();

    // relative path
    if ( 1 )
    {
        int n = newFile.length() - workDir.length() - 1;
        newFile = newFile.right( n );
    }

    if ( newFile.length() > 0 )
    {
        m_fileList.push_back( newFile );
    }

    updateListWidget();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditFileList::on_pushButtonRemove_clicked()
{
    if ( m_ui->listWidget->currentRow() > -1
      && m_ui->listWidget->currentRow() < m_fileList.size() )
    {
        m_fileList.erase( m_fileList.begin() + m_ui->listWidget->currentRow() );
    }

    updateListWidget();
}
