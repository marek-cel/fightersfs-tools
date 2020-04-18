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

#include "DialogEditWaypoint.h"
#include "ui_DialogEditWaypoint.h"

#include "Converter.h"

////////////////////////////////////////////////////////////////////////////////

DialogEditWaypoint::DialogEditWaypoint( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::DialogEditWaypoint )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditWaypoint::~DialogEditWaypoint()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditWaypoint::setWaypoint( Waypoint *waypoint )
{
    m_waypoint = waypoint;

    if ( m_waypoint )
    {
        m_ui->spinBoxPositionX->setValue( m_waypoint->first.x() );
        m_ui->spinBoxPositionY->setValue( m_waypoint->first.y() );
        m_ui->spinBoxPositionZ->setValue( m_waypoint->first.z() );

        m_ui->spinBoxSpeed->setValue( m_waypoint->second );
    }
    else
    {
        m_ui->spinBoxPositionX->setValue( 0.0 );
        m_ui->spinBoxPositionY->setValue( 0.0 );
        m_ui->spinBoxPositionZ->setValue( 0.0 );

        m_ui->spinBoxSpeed->setValue( 0.0 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditWaypoint::accept()
{
    if ( m_waypoint )
    {
        m_waypoint->first.x() = m_ui->spinBoxPositionX->value();
        m_waypoint->first.y() = m_ui->spinBoxPositionY->value();
        m_waypoint->first.z() = m_ui->spinBoxPositionZ->value();

        m_waypoint->second = m_ui->spinBoxSpeed->value();
    }

    //////////////////
    QDialog::accept();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditWaypoint::on_spinBoxPositionZ_valueChanged(double arg1)
{
    m_ui->spinBoxPositionZFt->setValue( floor( Converter::m2ft( arg1 ) + 0.5 ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditWaypoint::on_spinBoxSpeed_valueChanged(double arg1)
{
    m_ui->spinBoxSpeedKts->setValue( floor( Converter::mps2kts( arg1 ) + 0.5 ) );
}
