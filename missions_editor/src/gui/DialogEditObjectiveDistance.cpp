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

#include "DialogEditObjectiveDistance.h"
#include "ui_DialogEditObjectiveDistance.h"

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveDistance::DialogEditObjectiveDistance( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::DialogEditObjectiveDistance ),

    m_obj ( 0 ),

    m_limit ( 0.0f )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveDistance::~DialogEditObjectiveDistance()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDistance::setObj( ObjectiveDistance *obj )
{
    m_obj = 0;

    m_limit = 0.0f;

    m_obj = obj;

    if ( m_obj )
    {
        m_limit = m_obj->getLimit();
        m_leader = m_obj->getLeader().c_str();
    }

    m_ui->spinBoxLimit->setValue( m_limit );

    m_inited = false;

    m_ui->comboBoxLeader->clear();

    for ( int i = 0; i < (int)Mission::getInstance()->getUnits().size(); i++ )
    {
        m_ui->comboBoxLeader->addItem( Mission::getInstance()->getUnits().at( i )->getName().c_str() );
    }

    m_ui->comboBoxLeader->setCurrentIndex( Mission::getInstance()->getUnitIndex( m_leader.toStdString() ) );

    m_inited = true;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDistance::accept()
{
    if ( m_obj )
    {
        m_obj->setLimit( m_limit );
        m_obj->setLeader( m_leader.toStdString() );
    }

    //////////////////
    QDialog::accept();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDistance::on_spinBoxLimit_editingFinished()
{
    m_limit = m_ui->spinBoxLimit->value();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDistance::on_comboBoxLeader_currentIndexChanged(int index)
{
    if ( m_inited )
    {
        if ( index >=0 && index < (int)Mission::getInstance()->getUnits().size() )
        {
            m_leader = Mission::getInstance()->getUnitName( index ).c_str();
        }
    }
}
