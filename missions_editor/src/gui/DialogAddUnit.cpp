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

#include "DialogAddUnit.h"
#include "ui_DialogAddUnit.h"

////////////////////////////////////////////////////////////////////////////////

QString DialogAddUnit::getUnit( QWidget *parent )
{
    QString result;

    DialogAddUnit *dialog = new DialogAddUnit( parent );

    if ( dialog->exec() == QDialog::Accepted )
    {
        result = dialog->m_unit;
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

DialogAddUnit::DialogAddUnit( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::DialogAddUnit )
{
    m_ui->setupUi( this );

    for ( int i = 0; i < (int)Mission::getInstance()->getUnits().size(); i++ )
    {
        m_ui->comboBox->addItem( Mission::getInstance()->getUnits().at( i )->getName().c_str() );
    }

    m_ui->comboBox->setCurrentIndex( -1 );
}

////////////////////////////////////////////////////////////////////////////////

DialogAddUnit::~DialogAddUnit()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogAddUnit::on_comboBox_currentIndexChanged(int index)
{
    if ( index >= 0 && index < (int)Mission::getInstance()->getUnits().size() )
    {
        m_unit = Mission::getInstance()->getUnits().at( index )->getName().c_str();
    }
}
