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

#include "DialogEditObject.h"
#include "ui_DialogEditObject.h"

////////////////////////////////////////////////////////////////////////////////

QString DialogEditObject::getObjectFile( QString objectFile, QWidget *parent )
{
    DialogEditObject *dialog = new DialogEditObject( parent );

    dialog->m_ui->lineEditObject->setText( objectFile );

    if ( dialog->exec() == QDialog::Accepted )
    {
        objectFile = dialog->m_objectFile;
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return objectFile;
}

////////////////////////////////////////////////////////////////////////////////

DialogEditObject::DialogEditObject( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::DialogEditObject )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditObject::~DialogEditObject()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObject::on_lineEditObject_textChanged(const QString &arg1)
{
    m_objectFile = arg1;
}
