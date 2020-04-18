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
#ifndef WIDGETDOC_H
#define WIDGETDOC_H

////////////////////////////////////////////////////////////////////////////////

#include <QTreeWidgetItem>
#include <QWidget>

#include <gen/Document.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class WidgetDoc;
}

////////////////////////////////////////////////////////////////////////////////

class WidgetDoc : public QWidget
{
    Q_OBJECT

public:

    explicit WidgetDoc( QWidget *parent = 0 );

    ~WidgetDoc();

    void newEmpty();

    bool readFile( QString file );
    bool saveFile( QString file );
    bool exportAs( QString file );
    bool generateElevation( QString file );

    osg::Node* getRootNode();

signals:

    void changed();

private:

    Ui::WidgetDoc *m_ui;

    Document *m_doc;

    Terrain *m_terrain;

    QVector< QString > m_elevation;
    QVector< QString > m_exclusion;
    QVector< QString > m_landCrops;
    QVector< QString > m_landTrees;
    QVector< QString > m_landUrban;
    QVector< QString > m_landWater;
    QVector< QString > m_detailed;

    void updateGUI();

private slots:

    void on_spinBoxLat0_valueChanged( double arg1 );
    void on_spinBoxLon0_valueChanged( double arg1 );

    void on_spinBoxTiles_valueChanged( int arg1 );
    void on_spinBoxScale_valueChanged( double arg1 );

    void on_pushButtonElevation_clicked();
    void on_pushButtonExclusion_clicked();
    void on_pushButtonLandCrops_clicked();
    void on_pushButtonLandTrees_clicked();
    void on_pushButtonLandUrban_clicked();
    void on_pushButtonLandWater_clicked();
    void on_pushButtonDetailed_clicked();

    void on_pushButtonExportTiles_clicked();
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETDOC_H
