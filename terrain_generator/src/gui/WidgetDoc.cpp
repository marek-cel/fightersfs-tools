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

#include <iostream>

#include "WidgetDoc.h"
#include "ui_WidgetDoc.h"

#include <gui/DialogEditFileList.h>

////////////////////////////////////////////////////////////////////////////////

WidgetDoc::WidgetDoc( QWidget *parent ) :
    QWidget ( parent ),
    m_ui ( new Ui::WidgetDoc ),

    m_doc ( new Document() ),

    m_terrain ( 0 )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

WidgetDoc::~WidgetDoc()
{
    if ( m_doc ) delete m_doc; m_doc = 0;

    //////////////////////////////////
    if ( m_ui ) delete m_ui; m_ui = 0;
    //////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::newEmpty()
{
    m_doc->newEmpty();

    m_terrain = m_doc->getTerrain();

    updateGUI();
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetDoc::readFile( QString file )
{
    bool result = m_doc->readFile( file );

    m_terrain = m_doc->getTerrain();

    if ( result ) updateGUI();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetDoc::saveFile( QString file )
{
    return m_doc->saveFile( file );
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetDoc::exportAs( QString file )
{
    return m_doc->exportAs( file );
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetDoc::generateElevation( QString file )
{
    return m_doc->generateElevation( file );
}

////////////////////////////////////////////////////////////////////////////////

osg::Node* WidgetDoc::getRootNode()
{
    return m_doc->getRootNode();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::updateGUI()
{
    m_elevation.clear();
    m_exclusion.clear();
    m_landUrban.clear();
    m_landTrees.clear();
    m_landCrops.clear();
    m_landWater.clear();

    m_ui->spinBoxLat0->setValue( osg::RadiansToDegrees( m_terrain->getOrigLat() ) );
    m_ui->spinBoxLon0->setValue( osg::RadiansToDegrees( m_terrain->getOrigLon() ) );

    m_ui->spinBoxTiles->setValue( m_terrain->getTiles() );
    m_ui->spinBoxScale->setValue( m_terrain->getScale() );

    ListGeoTIFF::ListNames elevation_temp = m_terrain->getListElevation();
    ListGeoTIFF::ListNames exclusion_temp = m_terrain->getListExclusion();
    ListGeoTIFF::ListNames landCrops_temp = m_terrain->getListLandCrops();
    ListGeoTIFF::ListNames landTrees_temp = m_terrain->getListLandTrees();
    ListGeoTIFF::ListNames landUrban_temp = m_terrain->getListLandUrban();
    ListGeoTIFF::ListNames landWater_temp = m_terrain->getListLandWater();
    std::vector< std::string > detailed_temp = m_terrain->getListDetailed();

    for ( unsigned int i = 0; i < elevation_temp.size(); i++ )
    {
        m_elevation.push_back( elevation_temp[ i ].c_str() );
    }

    for ( unsigned int i = 0; i < exclusion_temp.size(); i++ )
    {
        m_exclusion.push_back( exclusion_temp[ i ].c_str() );
    }

    for ( unsigned int i = 0; i < landCrops_temp.size(); i++ )
    {
        m_landCrops.push_back( landCrops_temp[ i ].c_str() );
    }

    for ( unsigned int i = 0; i < landTrees_temp.size(); i++ )
    {
        m_landTrees.push_back( landTrees_temp[ i ].c_str() );
    }

    for ( unsigned int i = 0; i < landUrban_temp.size(); i++ )
    {
        m_landUrban.push_back( landUrban_temp[ i ].c_str() );
    }

    for ( unsigned int i = 0; i < landWater_temp.size(); i++ )
    {
        m_landWater.push_back( landWater_temp[ i ].c_str() );
    }

    for ( unsigned int i = 0; i < detailed_temp.size(); i++ )
    {
        m_detailed.push_back( detailed_temp[ i ].c_str() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_spinBoxLat0_valueChanged( double arg1 )
{
    m_terrain->setOrigLat( osg::DegreesToRadians( arg1 ) );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_spinBoxLon0_valueChanged( double arg1 )
{
    m_terrain->setOrigLon( osg::DegreesToRadians( arg1 ) );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_spinBoxTiles_valueChanged( int arg1 )
{
    m_terrain->setTiles( (unsigned short)arg1 );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_spinBoxScale_valueChanged( double arg1 )
{
    m_terrain->setScale( arg1 );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonElevation_clicked()
{
    if ( QDialog::Accepted == DialogEditFileList::editFileList( m_elevation, this ) )
    {
        ListGeoTIFF::ListNames temp;

        for ( int i = 0; i < m_elevation.size(); i++ )
        {
            temp.push_back( m_elevation.at( i ).toStdString() );
        }

        m_terrain->setListElevation( temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonExclusion_clicked()
{
    if ( QDialog::Accepted == DialogEditFileList::editFileList( m_exclusion, this ) )
    {
        ListGeoTIFF::ListNames temp;

        for ( int i = 0; i < m_exclusion.size(); i++ )
        {
            temp.push_back( m_exclusion.at( i ).toStdString() );
        }

        m_terrain->setListExclusion( temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonLandCrops_clicked()
{
    if ( QDialog::Accepted == DialogEditFileList::editFileList( m_landCrops, this ) )
    {
        ListGeoTIFF::ListNames temp;

        for ( int i = 0; i < m_landCrops.size(); i++ )
        {
            temp.push_back( m_landCrops.at( i ).toStdString() );
        }

        m_terrain->setListLandCrops( temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonLandTrees_clicked()
{
    if ( QDialog::Accepted == DialogEditFileList::editFileList( m_landTrees, this ) )
    {
        ListGeoTIFF::ListNames temp;

        for ( int i = 0; i < m_landTrees.size(); i++ )
        {
            temp.push_back( m_landTrees.at( i ).toStdString() );
        }

        m_terrain->setListLandTrees( temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonLandUrban_clicked()
{
    if ( QDialog::Accepted == DialogEditFileList::editFileList( m_landUrban, this ) )
    {
        ListGeoTIFF::ListNames temp;

        for ( int i = 0; i < m_landUrban.size(); i++ )
        {
            temp.push_back( m_landUrban.at( i ).toStdString() );
        }

        m_terrain->setListLandUrban( temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonLandWater_clicked()
{
    if ( QDialog::Accepted == DialogEditFileList::editFileList( m_landWater, this ) )
    {
        ListGeoTIFF::ListNames temp;

        for ( int i = 0; i < m_landWater.size(); i++ )
        {
            temp.push_back( m_landWater.at( i ).toStdString() );
        }

        m_terrain->setListLandWater( temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonDetailed_clicked()
{
    if ( QDialog::Accepted == DialogEditFileList::editFileList( m_detailed, this ) )
    {
        std::vector< std::string > temp;

        for ( int i = 0; i < m_detailed.size(); i++ )
        {
            temp.push_back( m_detailed.at( i ).toStdString() );
        }

        m_terrain->setListDetailed( temp );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetDoc::on_pushButtonExportTiles_clicked()
{
    m_terrain->exportTiles();
}
