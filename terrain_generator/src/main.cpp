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

#include <QApplication>

#include <defs.h>
#include <gui/MainWindow.h>

#include <TexturesGenerator.h>

#include <gen/Common.h>

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    setlocale( LC_ALL, "C" );

#   ifdef _LINUX_
    setenv( "LC_NUMERIC", "en_US", 1 );
#   endif

    Common::getInstance()->init();

    QApplication *app = new QApplication( argc, argv );

    app->setApplicationName( GEN_APP_NAME );
    app->setApplicationVersion( GEN_APP_VER );
    app->setOrganizationDomain( GEN_ORG_DOMAIN );
    app->setOrganizationName( GEN_ORG_NAME );

//    TexturesGenerator tgen;
//    tgen.generateTextures();

    MainWindow *win = new MainWindow();
    win->show();
    
    int result = app->exec();

    if ( win ) delete win;
    win = 0;

    if ( app ) delete app;
    app = 0;

    return result;
}
