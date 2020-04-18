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

#include "DialogEditMessage.h"
#include "ui_DialogEditMessage.h"

////////////////////////////////////////////////////////////////////////////////

DialogEditMessage::DialogEditMessage( QWidget *parent ) :
    QDialog( parent ),
    m_ui( new Ui::DialogEditMessage )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DialogEditMessage::~DialogEditMessage()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditMessage::setMessage( Message *message )
{
    m_message = message;

    if ( m_message )
    {
        m_ui->checkBoxOverlay->setChecked( m_message->getOverlay() );

        m_ui->spinBoxMsgDelay->setValue( m_message->getDelay() );
        m_ui->spinBoxMsgDuration->setValue( m_message->getDuration() );

        Message::Lines lines = m_message->getLines();

        m_ui->lineEditMsgLine_1->setText( "" );
        m_ui->lineEditMsgLine_2->setText( "" );

        if ( lines.size() > 0 ) m_ui->lineEditMsgLine_1->setText( lines.at( 0 ).get().c_str() );
        if ( lines.size() > 1 ) m_ui->lineEditMsgLine_2->setText( lines.at( 1 ).get().c_str() );

        if ( Mission::getInstance()->getTutorial() )
        {
            m_ui->groupBoxTutorial->setEnabled( true );

            m_ui->checkBoxPointerCustom->setChecked( m_message->getPointerCustom() );

            m_ui->spinBoxPointerX   ->setValue( m_message->getPointerX()   );
            m_ui->spinBoxPointerY   ->setValue( m_message->getPointerY()   );
            m_ui->spinBoxPointerPhi ->setValue( m_message->getPointerPhi() );

            m_ui->checkBoxPointerTarget  ->setChecked( m_message->getPointerTarget()  );
            m_ui->checkBoxPointerRpmDec  ->setChecked( m_message->getPointerRpmDec()  );
            m_ui->checkBoxPointerRpmInc  ->setChecked( m_message->getPointerRpmInc()  );
            m_ui->checkBoxPointerTrigger ->setChecked( m_message->getPointerTrigger() );

            m_ui->comboBoxTutorialTip->setCurrentIndex( m_message->getTutorialTip() );
        }
        else
        {
            m_ui->groupBoxTutorial->setEnabled( false );

            m_ui->checkBoxPointerCustom->setChecked( false );

            m_ui->spinBoxPointerX   ->setValue( 0.0 );
            m_ui->spinBoxPointerY   ->setValue( 0.0 );
            m_ui->spinBoxPointerPhi ->setValue( 0.0 );

            m_ui->checkBoxPointerTarget  ->setChecked( false );
            m_ui->checkBoxPointerTarget  ->setChecked( false );
            m_ui->checkBoxPointerRpmDec  ->setChecked( false );
            m_ui->checkBoxPointerRpmInc  ->setChecked( false );
            m_ui->checkBoxPointerTrigger ->setChecked( false );

            m_ui->comboBoxTutorialTip->setCurrentIndex( 0 );
        }
    }
    else
    {
        m_ui->checkBoxOverlay->setChecked( false );

        m_ui->spinBoxMsgDelay->setValue( 0 );
        m_ui->spinBoxMsgDuration->setValue( 0 );

        m_ui->lineEditMsgLine_1->setText( "" );
        m_ui->lineEditMsgLine_2->setText( "" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEditMessage::accept()
{
    if ( m_message )
    {
        m_message->setOverlay( m_ui->checkBoxOverlay->isChecked() );

        m_message->setDelay( m_ui->spinBoxMsgDelay->value() );
        m_message->setDuration( m_ui->spinBoxMsgDuration->value() );

        Message::Lines lines = m_message->getLines();

        if ( lines.size() > 0 )
        {
            lines.at( 0 ).set( m_ui->lineEditMsgLine_1->text().toStdString() );
        }
        else
        {
            Text text;
            text.set( m_ui->lineEditMsgLine_1->text().toStdString() );

            lines.push_back( text );
        }

        if ( lines.size() > 1 )
        {
            lines.at( 1 ).set( m_ui->lineEditMsgLine_2->text().toStdString() );
        }
        else
        {
            Text text;
            text.set( m_ui->lineEditMsgLine_2->text().toStdString() );

            lines.push_back( text );
        }

        m_message->setLines( lines );

        if ( Mission::getInstance()->getTutorial() )
        {
            m_message->setPointerCustom( m_ui->checkBoxPointerCustom->isChecked() );

            m_message->setPointerX( m_ui->spinBoxPointerX->value() );
            m_message->setPointerY( m_ui->spinBoxPointerY->value() );
            m_message->setPointerPhi( m_ui->spinBoxPointerPhi->value() );

            m_message->setPointerTarget( m_ui->checkBoxPointerTarget->isChecked() );
            m_message->setPointerRpmDec( m_ui->checkBoxPointerRpmDec->isChecked() );
            m_message->setPointerRpmInc( m_ui->checkBoxPointerRpmInc->isChecked() );
            m_message->setPointerTrigger( m_ui->checkBoxPointerTrigger->isChecked() );

            m_message->setTutorialTip( m_ui->comboBoxTutorialTip->currentIndex() );
        }
    }

    //////////////////
    QDialog::accept();
    //////////////////
}
