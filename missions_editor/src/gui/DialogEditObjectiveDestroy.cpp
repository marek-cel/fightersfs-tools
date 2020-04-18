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

#include "DialogEditObjectiveDestroy.h"
#include "ui_DialogEditObjectiveDestroy.h"

#include <gui/DialogAddUnit.h>

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveDestroy::DialogEditObjectiveDestroy( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::DialogEditObjectiveDestroy ),

    m_obj ( 0 ),

    m_currentUnitIndex ( -1 )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditObjectiveDestroy::~DialogEditObjectiveDestroy()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDestroy::setObj( ObjectiveDestroy *obj )
{
    m_obj = 0;

    m_units.clear();

    m_ui->listWidgetDestroyUnits->clear();

    m_obj = obj;

    if ( m_obj )
    {
        m_units = m_obj->getUnits();

        for ( int i = 0; i < (int)m_units.size(); i++ )
        {
            m_ui->listWidgetDestroyUnits->addItem( new QListWidgetItem( m_units.at( i ).c_str(), m_ui->listWidgetDestroyUnits ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDestroy::accept()
{
    if ( m_obj )
    {
        m_obj->setUnits( m_units );
    }

    //////////////////
    QDialog::accept();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDestroy::on_pushButtonDestroyUnitAdd_clicked()
{
    if ( m_obj )
    {
        std::string unit = DialogAddUnit::getUnit( this ).toStdString();

        if ( unit.length() > 0 )
        {
            m_units.push_back( unit );

            m_ui->listWidgetDestroyUnits->clear();

            if ( m_obj )
            {
                for ( int i = 0; i < (int)m_units.size(); i++ )
                {
                    m_ui->listWidgetDestroyUnits->addItem( new QListWidgetItem( m_units.at( i ).c_str(), m_ui->listWidgetDestroyUnits ) );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDestroy::on_pushButtonDestroyUnitRemove_clicked()
{
    if ( m_obj )
    {
        if ( m_currentUnitIndex >= 0 && m_currentUnitIndex < (int)m_units.size() )
        {
            m_units.erase( m_units.begin() + m_currentUnitIndex );

            m_ui->listWidgetDestroyUnits->clear();

            if ( m_obj )
            {
                for ( int i = 0; i < (int)m_units.size(); i++ )
                {
                    m_ui->listWidgetDestroyUnits->addItem( new QListWidgetItem( m_units.at( i ).c_str(), m_ui->listWidgetDestroyUnits ) );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditObjectiveDestroy::on_listWidgetDestroyUnits_currentRowChanged(int currentRow)
{
    if ( m_obj && currentRow >= 0 && currentRow < (int)m_units.size() )
    {
        m_currentUnitIndex = currentRow;

        m_ui->pushButtonDestroyUnitRemove->setEnabled( true );
    }
    else
    {
        m_currentUnitIndex = -1;

        m_ui->pushButtonDestroyUnitRemove->setEnabled( false );
    }
}
