////////////////////////////////////////////////////////////////////////////////
// SimpleVideoWriter
//
// PURPOSE: Writes grayscale images to a video file that can be used for 
//          testing BigBlobby.
//
// CREATED: 7/2/2014
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2014, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#include "blobVideo/SimpleVideoWriter.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobUtils/FunctionTimer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using blobVideo::SimpleVideoWriter;

const int SimpleVideoWriter::RGB_BYTES = 3;

SimpleVideoWriter::SimpleVideoWriter() :
  processTimer_( new blobUtils::FunctionTimer() ),
  cvVideoWriter_( NULL ),
  rgbPixels_( NULL ),
  bufferWidth_( 0 ),
  bufferHeight_( 0 ),
  flipVertical_( false ),
  flipHorizontal_( false )
{
    
}

SimpleVideoWriter::~SimpleVideoWriter()
{
    delete cvVideoWriter_;
    delete processTimer_;
    delete rgbPixels_;
}

bool SimpleVideoWriter::initGrayscale( const std::string & filename, 
                                       int videoWidth, 
                                       int videoHeight, 
                                       int videoRate /* = 20 */ )
{
    initBuffer( videoWidth, videoHeight );
    //int fourCC = -1; // Opens a dialog box.
    // "DIB " = Device Independent Bitmap (no compression)
    int fourCC = CV_FOURCC( 'C', 'V', 'I', 'D' ); // Cinepak Codec by Radius
    cvVideoWriter_ = new cv::VideoWriter( filename,
                                          fourCC,
                                          videoRate,
                                          cv::Size( bufferWidth_, bufferHeight_ ),
                                          true );
    return cvVideoWriter_->isOpened();
}

void SimpleVideoWriter::initBuffer( int bufferWidth, int bufferHeight )
{
    bufferWidth_ = bufferWidth;
    bufferHeight_ = bufferHeight;
    delete rgbPixels_;

    int numBytes = bufferWidth_ * bufferHeight_ * RGB_BYTES;
    rgbPixels_ = new unsigned char[numBytes];
    std::memset( rgbPixels_, 0, numBytes );
}

bool SimpleVideoWriter::initColor( const std::string & filename, 
                                   int videoWidth, 
                                   int videoHeight, 
                                   int videoRate /* = 20 */ )
{
    bufferWidth_ = videoWidth;
    bufferHeight_ = videoHeight;
    int fourCC = CV_FOURCC( 'C', 'V', 'I', 'D' ); // Cinepak Codec by Radius
    cvVideoWriter_ = new cv::VideoWriter( filename,
                                          fourCC,
                                          videoRate,
                                          cv::Size( bufferWidth_, bufferHeight_ ),
                                          true );
    return cvVideoWriter_->isOpened();
}

void SimpleVideoWriter::flipImageVertical( bool b )
{
    flipVertical_ = b;
}

void SimpleVideoWriter::flipImageHorizontal( bool b )
{
    flipHorizontal_ = b;
}

/***************************************************************************//**
Will return false if the width and height do not match the values plugged in to 
the initGrayscale() function.
*******************************************************************************/
bool SimpleVideoWriter::processGrayscaleImage( blobBuffers::GrayBuffer * srcImage )
{
    processTimer_->startTiming();
    bool success = false;

    if( cvVideoWriter_ != NULL && srcImage != NULL ) {
        int rows = srcImage->height(),
            cols = srcImage->width();

        if(  cols == bufferWidth_ && rows == bufferHeight_ ) {
            cv::Mat srcM( rows, cols, CV_8U, srcImage->pixels(), cols );
            cv::Mat dstM( rows, cols, CV_8UC3, rgbPixels_, cols * RGB_BYTES );
            cv::cvtColor( srcM, dstM, CV_GRAY2RGB );
            flipIfRequested( dstM );
            cvVideoWriter_->write( dstM );
            success = true;
        }
    }
    processTimer_->stopTiming();
    return success;
}

/***************************************************************************//**
Will return false if the width and height do not match the values plugged in to 
the initColor() function.
*******************************************************************************/
bool SimpleVideoWriter::processRgbImage( unsigned char * pixels, int width, int height, int bytesPerRow )
{
    processTimer_->startTiming();
    bool success = false;

    if( cvVideoWriter_ != NULL && width == bufferWidth_ && height == bufferHeight_ ) {
        int rows = height,
            cols = width;

        if(  cols == bufferWidth_ && rows == bufferHeight_ ) {
            cv::Mat mat( rows, cols, CV_8UC3, pixels, bytesPerRow );
            flipIfRequested( mat );
            cvVideoWriter_->write( mat );
            success = true;
        }
    }
    processTimer_->stopTiming();
    return success;
}

void SimpleVideoWriter::flipIfRequested( cv::Mat & m )
{
    if( flipVertical_ && flipHorizontal_ ) { cv::flip( m, m, -1 ); }
    else if( flipVertical_ )               { cv::flip( m, m,  0 ); }
    else if( flipHorizontal_ )             { cv::flip( m, m,  1 ); }
}

void SimpleVideoWriter::clear()
{
    bufferWidth_ = 0;
    bufferHeight_ = 0;
    delete rgbPixels_;
    delete cvVideoWriter_;
    rgbPixels_ = NULL;
    cvVideoWriter_ = NULL;
}

double SimpleVideoWriter::calculateAverageTime()
{
    return processTimer_->calculateAverageTime();
}
