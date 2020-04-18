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

#include "DialogEditObjectiveTimeout.h"
#include "ui_DialogEditObjectiveTimeout.h"

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveTimeout::DialogEditObjectiveTimeout( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::DialogEditObjectiveTimeout ),

    m_obj ( 0 ),

    m_limit ( 0.0f )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveTimeout::~DialogEditObjectiveTimeout()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveTimeout::setObj( ObjectiveTimeout *obj )
{
    m_obj = 0;

    m_limit = 0.0f;

    m_obj = obj;

    if ( m_obj )
    {
        m_limit = m_obj->getLimit();
    }

    m_ui->spinBoxLimit->setValue( m_limit );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveTimeout::accept()
{
    if ( m_obj )
    {
        m_obj->setLimit( m_limit );
    }

    //////////////////
    QDialog::accept();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveTimeout::on_spinBoxLimit_editingFinished()
{
    m_limit = m_ui->spinBoxLimit->value();
}
