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

#include "DialogEditObjectiveAirspeed.h"
#include "ui_DialogEditObjectiveAirspeed.h"

#include "Converter.h"

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveAirspeed::DialogEditObjectiveAirspeed( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::DialogEditObjectiveAirspeed ),

    m_obj ( 0 ),

    m_above ( 0.0f ),
    m_below ( 300.0f )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveAirspeed::~DialogEditObjectiveAirspeed()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveAirspeed::setObj( ObjectiveAirspeed *obj )
{
    m_obj = 0;

    m_above = 50.0f;
    m_below = 400.0f;

    m_obj = obj;

    if ( m_obj )
    {
        m_above = m_obj->getAbove();
        m_below = m_obj->getBelow();
    }

    m_ui->spinBoxAbove->setValue( m_above );
    m_ui->spinBoxBelow->setValue( m_below );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveAirspeed::accept()
{
    if ( m_obj )
    {
        m_obj->setAbove( m_above );
        m_obj->setBelow( m_below );
    }

    //////////////////
    QDialog::accept();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveAirspeed::on_spinBoxAbove_editingFinished()
{
    m_above = m_ui->spinBoxAbove->value();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveAirspeed::on_spinBoxAbove_valueChanged(double arg1)
{
    m_ui->spinBoxAboveKts->setValue( floor( Converter::mps2kts( arg1 ) + 0.5 ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveAirspeed::on_spinBoxBelow_editingFinished()
{
    m_below = m_ui->spinBoxBelow->value();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveAirspeed::on_spinBoxBelow_valueChanged(double arg1)
{
    m_ui->spinBoxBelowKts->setValue( floor( Converter::mps2kts( arg1 ) + 0.5 ) );
}
