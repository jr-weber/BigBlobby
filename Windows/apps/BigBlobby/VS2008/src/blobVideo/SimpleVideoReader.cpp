////////////////////////////////////////////////////////////////////////////////
// SimpleVideoReader
//
// PURPOSE: Uses the OpenCV VideoCapture class to read a video from a file.
//
// CREATED: 7/6/2014
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
#include "blobVideo/SimpleVideoReader.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobUtils/StopWatch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using blobVideo::SimpleVideoReader;

SimpleVideoReader::SimpleVideoReader() :
  stopWatch_( new blobUtils::StopWatch() ),
  cvVideoCapture_( NULL ),
  rgbPixels_( NULL ),
  bufferWidth_( 0 ),
  bufferHeight_( 0 ),
  bytesPerRow_( 0 ),
  totalBytes_( 0 ),
  fps_( 0.0 ),
  minFrameTime_( 0.0 ),
  isFrameNew_( false )
{
    
}

SimpleVideoReader::~SimpleVideoReader()
{
    delete cvVideoCapture_;
    delete stopWatch_;
    delete rgbPixels_;
}

bool SimpleVideoReader::init( const std::string & filename )
{
    close();
    cvVideoCapture_ = new cv::VideoCapture( filename );
    bool isFileOpen = cvVideoCapture_->isOpened();

    if( isFileOpen ) {
        bufferWidth_ = (int)cvVideoCapture_->get( CV_CAP_PROP_FRAME_WIDTH );
        bufferHeight_ = (int)cvVideoCapture_->get( CV_CAP_PROP_FRAME_HEIGHT );
        bytesPerRow_ = bufferWidth_ * 3;
        totalBytes_ = bytesPerRow_ * bufferHeight_;
        fps_ = cvVideoCapture_->get( CV_CAP_PROP_FPS );
        minFrameTime_ = (fps_ > 0) ? (1.0 / fps_) : 0.0;
        initBuffer();
        //debugPrintVideoInfo( filename );
    }
    else {
        printInitErrorMessage( filename );
    }
    return isFileOpen;
}

void SimpleVideoReader::initBuffer()
{
    rgbPixels_ = new unsigned char[totalBytes_];
    std::memset( rgbPixels_, 0, totalBytes_ );
    isFrameNew_ = false;
}

void SimpleVideoReader::printInitErrorMessage( const std::string & filename )
{
    std::cout << "\vSimpleVideoReader::init() called.\n"
              << "Video file could not be opened:\n" << filename << "\n\n";
}

int SimpleVideoReader::width()
{
    return bufferWidth_;
}

int SimpleVideoReader::height()
{
    return bufferHeight_;
}

void SimpleVideoReader::updateFrame()
{
    isFrameNew_ = false;

    if( rgbPixels_ != NULL ) {
        if( stopWatch_->elapsed() >= minFrameTime_ ) {
            stopWatch_->start();
            cv::Mat rgbPixelsMat( bufferHeight_, bufferWidth_, CV_8UC3, rgbPixels_, bufferWidth_ * 3 );

            if( cvVideoCapture_ != NULL && cvVideoCapture_->isOpened() ) {
                isFrameNew_ = cvVideoCapture_->read( rgbPixelsMat );

                if( rgbPixelsMat.empty() ) {
                    cvVideoCapture_->set( CV_CAP_PROP_POS_FRAMES, 0 );
                    isFrameNew_ = false;
                }
            }
        }
    }
}

bool SimpleVideoReader::isFrameNew()
{
    return isFrameNew_;
}

unsigned char * SimpleVideoReader::getPixels()
{
    isFrameNew_ = false;
    return rgbPixels_;
}

void SimpleVideoReader::close()
{
    //std::cout << "\nSimpleVideoReader::close() called.\n\n";
    isFrameNew_ = false;
    bufferWidth_ = 0;
    bufferHeight_ = 0;
    bytesPerRow_ = 0;
    totalBytes_ = 0;
    fps_ = 0;
    minFrameTime_ = 0;
    delete cvVideoCapture_;
    cvVideoCapture_ = NULL;
}

void SimpleVideoReader::debugPrintVideoInfo( const std::string & filename )
{
    std::cout << "SimpleVideoReader::init() called.\n"
              << "Video file opened:\n" << filename << "\n"
              << "width = " << bufferWidth_ << "\n"
              << "height = " << bufferHeight_ << "\n"
              << "fps = " << fps_ << "\n\n";
}
