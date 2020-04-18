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

#include "DialogAddObjective.h"
#include "ui_DialogAddObjective.h"

#include <editor/ObjectiveAirspeed.h>
#include <editor/ObjectiveAltitude.h>
#include <editor/ObjectiveDefend.h>
#include <editor/ObjectiveDestroy.h>
#include <editor/ObjectiveDistance.h>
#include <editor/ObjectiveHeading.h>
#include <editor/ObjectiveTimeout.h>
#include <editor/ObjectiveWaypoint.h>

////////////////////////////////////////////////////////////////////////////////

Objective* DialogAddObjective::getObjective( QWidget *parent )
{
    Objective *objective = 0;

    DialogAddObjective *dialog = new DialogAddObjective( parent );

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->m_objective )
        {
            objective = dialog->m_objective->clone();
        }
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return objective;
}

////////////////////////////////////////////////////////////////////////////////

DialogAddObjective::DialogAddObjective( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::DialogAddObjective ),

    m_objective ( 0 )
{
    m_ui->setupUi( this );

    m_ui->comboBox->setCurrentIndex( -1 );
}

////////////////////////////////////////////////////////////////////////////////

DialogAddObjective::~DialogAddObjective()
{
    if ( m_objective ) delete m_objective;
    m_objective = 0;

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogAddObjective::on_comboBox_currentIndexChanged(int index)
{
    if ( m_objective ) delete m_objective;
    m_objective = 0;

    switch ( index )
    {
        case 0: m_objective = new ObjectiveAirspeed(); break;
        case 1: m_objective = new ObjectiveAltitude(); break;
        case 2: m_objective = new ObjectiveDefend(); break;
        case 3: m_objective = new ObjectiveDestroy(); break;
        case 4: m_objective = new ObjectiveDistance(); break;
        case 5: m_objective = new ObjectiveHeading(); break;
        case 6: m_objective = new ObjectiveTimeout(); break;
        case 7: m_objective = new ObjectiveWaypoint(); break;
    }
}
