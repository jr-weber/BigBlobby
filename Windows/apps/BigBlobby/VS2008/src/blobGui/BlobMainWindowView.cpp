////////////////////////////////////////////////////////////////////////////////
// BlobMainWindowView
//
// PURPOSE: This helper class for BlobMainWindow has functions for controlling
//          the size of the Source, Tracked, and filter images.
//
// CREATED:  6/20/2013
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2013, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer as
the first lines of this file unmodified.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License, version 2.1.
You should have received a copy of the GNU Lesser General Public License along
with this program.  If not, go to 

http://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html 

or write to 
 
Free Software  Foundation, Inc.
59 Temple Place, Suite 330
Boston, MA  02111-1307  USA
 
THIS SOFTWARE IS PROVIDED BY NUI GROUP ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL BEN WOODHOUSE BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "blobGui/BlobMainWindowView.h"
#include "blobGui/BlobMainWindow.h"
#include "blobGui/SourceGLWidget.h"
#include "blobGui/FilterGLWidget.h"
#include "ui_BlobMainWindow.h"
#include <QApplication>
#include <QDesktopWidget>

using blobGui::BlobMainWindowView;
using blobGui::BlobMainWindow;
using Ui::BlobMainWindowUi;

const double BlobMainWindowView::MIN_ASPECT_RATIO = (4.0 / 3.0);
const int BlobMainWindowView::TOP_MARGIN = 40,
          BlobMainWindowView::SIDE_MARGIN = 10,
          BlobMainWindowView::MIN_FRAME_GEO_WIDTH = 200,
          BlobMainWindowView::MIN_FRAME_GEO_HEIGHT = 185,
          BlobMainWindowView::MAX_WIDGET_DIMENSION = 16777215; // Qt Designer max size

/***************************************************************************//**

*******************************************************************************/
BlobMainWindowView::BlobMainWindowView( BlobMainWindow * blobMainWindow,
                                        BlobMainWindowUi * ui ) :
  blobMainWindow_( blobMainWindow ),
  mainUi_( ui ),
  initialAspectRatio_( 0.0 ),
  initialFrameX_( 0 ),
  initialFrameY_( 0 ),
  initialMainWindowWidth_( 0 ),
  initialMainWindowHeight_( 0 ),
  initialMainPanelWidth_( 0 ),
  initialMainPanelHeight_( 0 ),
  initialMainPanelMinWidth_( 0 ),
  initialMainPanelMinHeight_( 0 ),
  initialImageWidth_( 0 ),
  initialImageHeight_( 0 ),
  debug_( false )
{
    calculateDefaultSizeAndCentering();
    resizeAndCenterToDefault();
    memorizeMainWindowDimensions();
    debugPrintInitialDimensions();
}

BlobMainWindowView::~BlobMainWindowView()
{
}

/***************************************************************************//**
This helper function for the constructor looks at the aspect ratio of the 
BlobMainWindow and then adjusts it size to fit into the available geometry of 
the screen.
*******************************************************************************/
void BlobMainWindowView::calculateDefaultSizeAndCentering()
{
    QRect screenGeoRect = QApplication::desktop()->availableGeometry( blobMainWindow_ ),
          frameGeoRect = blobMainWindow_->frameGeometry(),
          mainWindowGeoRect = blobMainWindow_->geometry();
    debugPrintGeo( "\nScreenGeo", screenGeoRect );
    debugPrintGeo( "FrameGeo", frameGeoRect );
    debugPrintGeo( "MainWindowGeo", mainWindowGeoRect );
    int screenWidth = screenGeoRect.width(),
        screenHeight = screenGeoRect.height(),
        frameWidth = frameGeoRect.width(),
        frameHeight = frameGeoRect.height(),
        mainWidth = mainWindowGeoRect.width(),
        mainHeight = mainWindowGeoRect.height(),
        frameWidthDiff = frameWidth - mainWidth,
        frameHeightDiff = frameHeight - mainHeight;

    initialAspectRatio_ = frameWidth / (double)frameHeight;

    if( initialAspectRatio_ < MIN_ASPECT_RATIO ) {
        initialAspectRatio_ = MIN_ASPECT_RATIO;
    }
    int initialFrameHeight = screenHeight - TOP_MARGIN,
        initialFrameWidth = (int)(initialFrameHeight * initialAspectRatio_);

    if( (initialFrameWidth + SIDE_MARGIN) > screenWidth ) {
        initialFrameWidth = screenWidth - SIDE_MARGIN;
        initialFrameHeight = (int)(initialFrameWidth / initialAspectRatio_);
    }
    initialFrameX_ = (screenWidth - initialFrameWidth) / 2;
    initialFrameY_ = (screenHeight - initialFrameHeight) / 2;
    initialMainWindowWidth_ = initialFrameWidth - frameWidthDiff;
    initialMainWindowHeight_ = initialFrameHeight - frameHeightDiff;
}

void BlobMainWindowView::resizeAndCenterToDefault()
{
    blobMainWindow_->resize( initialMainWindowWidth_, initialMainWindowHeight_ );
    blobMainWindow_->move( initialFrameX_, initialFrameY_ );
    emit zoomMainPanelImagesCalled( 100 );
}

void BlobMainWindowView::memorizeMainWindowDimensions()
{
    int menubarHeight = mainUi_->menubar->height();
    initialMainPanelWidth_ = initialMainWindowWidth_;
    initialMainPanelHeight_ = initialMainWindowHeight_ - menubarHeight;
    initialMainPanelMinWidth_ = mainUi_->widget_mainPanel->minimumWidth();
    initialMainPanelMinHeight_ = mainUi_->widget_mainPanel->minimumHeight();

    int widthDiff = initialMainPanelWidth_ - initialMainPanelMinWidth_,
        heightDiff = initialMainPanelHeight_ - initialMainPanelMinHeight_;
    initialImageWidth_ = mainUi_->frame_sourceImage->width() - 2  + widthDiff / 2;
    initialImageHeight_ = mainUi_->frame_sourceImage->height() - 2 + heightDiff / 2;
}

void BlobMainWindowView::setViewToMinimalWindow()
{
    blobMainWindow_->resize( MIN_FRAME_GEO_WIDTH, MIN_FRAME_GEO_HEIGHT );
}

/***************************************************************************//**
Resets the main window to the size it opens to when the program starts.
*******************************************************************************/
void BlobMainWindowView::setViewToDefault()
{
    mainUi_->widget_mainPanel->setMinimumSize( initialMainPanelMinWidth_, initialMainPanelMinHeight_ );
    mainUi_->widget_mainPanel->setMaximumSize( MAX_WIDGET_DIMENSION, MAX_WIDGET_DIMENSION );
    resizeAndCenterToDefault();
}

/***************************************************************************//**
Sets the panel images to 100%, where 100% is defined as the size of the images 
when the program first opened.
*******************************************************************************/
void BlobMainWindowView::zoomMainPanelImages100()
{
    zoomMainPanelImagesPercent( 100 );
    emit zoomMainPanelImagesCalled( 100 );
}

/***************************************************************************//**
Sets the panel images to 200%, where 100% is defined as the size of the images 
when the program first opened.
*******************************************************************************/
void BlobMainWindowView::zoomMainPanelImages200()
{
    zoomMainPanelImagesPercent( 200 );
    emit zoomMainPanelImagesCalled( 200 );
}

/***************************************************************************//**
Sets the panel images to 400%, where 100% is defined as the size of the images 
when the program first opened.
*******************************************************************************/
void BlobMainWindowView::zoomMainPanelImages400()
{
    zoomMainPanelImagesPercent( 400 );
    emit zoomMainPanelImagesCalled( 400 );
}

/***************************************************************************//**
Sets the panel images to the requested size, where 100% is defined as the size 
of the images when the program first opened.
*******************************************************************************/
void BlobMainWindowView::zoomMainPanelImagesPercent( int percent )
{
    std::cout << "zoom % = " << percent << "\n";
    scaleImages( percent / 100.0 );
}

void BlobMainWindowView::scaleImages( double scaleFactor )
{
    if( scaleFactor < 1.0 ) {
        scaleFactor = 1.0;
    }
    int imageWidth = (int)(initialImageWidth_ * scaleFactor),
        imageHeight = (int)(initialImageHeight_ * scaleFactor),
        widthIncrease = imageWidth - initialImageWidth_,
        heightIncrease = imageHeight - initialImageHeight_,
        mainPanelWidth = initialMainPanelWidth_ + (2 * widthIncrease),
        mainPanelHeight = initialMainPanelHeight_ + (2 * heightIncrease);

    mainUi_->widget_mainPanel->setFixedSize( mainPanelWidth, mainPanelHeight );
}

void BlobMainWindowView::debugPrintGeo( const std::string & name, const QRect & rect )
{
    debug_.printLn( name + " geometry: (x, y) = (" 
                  + debug_.intToStr( rect.x() ) + ", " 
                  + debug_.intToStr( rect.y() ) + ") and (width x height) = (" 
                  + debug_.intToStr( rect.width() ) + " x " 
                  + debug_.intToStr( rect.height() ) + ")" );
}

void BlobMainWindowView::debugPrintInitialDimensions()
{
    debug_.printLn( 
        "initialFrameX_ = " + debug_.intToStr( initialFrameX_ ) + "\n"
      + "initialFrameY_ = " + debug_.intToStr( initialFrameY_ ) + "\n"
      + "initialMainWindowWidth_  = " + debug_.intToStr( initialMainWindowWidth_ ) + "\n"
      + "initialMainWindowHeight_ = " + debug_.intToStr( initialMainWindowHeight_ ) + "\n\n"

      + "initialMainPanelWidth_  = " + debug_.intToStr( initialMainPanelWidth_ ) + "\n"
      + "initialMainPanelHeight_ = " + debug_.intToStr( initialMainPanelHeight_ ) + "\n"
      + "initialMainPanelMinWidth_  = " + debug_.intToStr( initialMainPanelMinWidth_ ) + "\n"
      + "initialMainPanelMinHeight_ = " + debug_.intToStr( initialMainPanelMinHeight_ ) + "\n\n"

      + "initialImageWidth_  = " + debug_.intToStr( initialImageWidth_ ) + "\n"
      + "initialImageHeight_ = " + debug_.intToStr( initialImageHeight_ ) + "\n" );
}
