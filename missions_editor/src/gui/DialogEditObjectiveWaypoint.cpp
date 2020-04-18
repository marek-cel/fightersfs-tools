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

#include "DialogEditObjectiveWaypoint.h"
#include "ui_DialogEditObjectiveWaypoint.h"

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveWaypoint::DialogEditObjectiveWaypoint( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::DialogEditObjectiveWaypoint ),

    m_obj ( 0 ),

    m_index ( 0 )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveWaypoint::~DialogEditObjectiveWaypoint()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveWaypoint::setObj( ObjectiveWaypoint *obj )
{
    m_obj = 0;

    m_index = 0;

    m_ui->spinBox->setMaximum( 0 );

    m_obj = obj;

    if ( m_obj )
    {
        Unit *unit = Mission::getInstance()->getOwnship();

        if ( unit )
        {
            std::string routeName = unit->getRoute();

            if ( routeName.length() > 0 )
            {
                Route *route = Mission::getInstance()->getRouteByName( routeName );

                if ( route )
                {
                    m_ui->spinBox->setMaximum( route->size() - 1 );
                    m_ui->spinBox->setValue( m_obj->getIndex() );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveWaypoint::accept()
{
    if ( m_obj )
    {
        m_obj->setIndex( m_index );
    }

    //////////////////
    QDialog::accept();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveWaypoint::on_spinBox_valueChanged(int arg1)
{
    m_index = arg1;
}
