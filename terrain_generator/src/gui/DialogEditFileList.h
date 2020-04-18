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
#ifndef DIALOGEDITFILELIST_H
#define DIALOGEDITFILELIST_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogEditFileList;
}

////////////////////////////////////////////////////////////////////////////////

class DialogEditFileList : public QDialog
{
    Q_OBJECT

public:

    static int editFileList( QVector< QString > &fileList, QWidget *parent = 0 );

    explicit DialogEditFileList( QVector< QString > fileList, QWidget *parent = 0 );

    ~DialogEditFileList();

private:

    Ui::DialogEditFileList *m_ui;

    QVector< QString > m_fileList;

    void updateListWidget();

private slots:

    void on_listWidget_currentRowChanged( int currentRow );

    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGEDITFILELIST_H
