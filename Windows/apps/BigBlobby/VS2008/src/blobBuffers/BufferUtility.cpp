////////////////////////////////////////////////////////////////////////////////
// BufferUtility
//
// PURPOSE: Knows how to copy an image while flipping it about the horizontal
//          and/or vertical axis.
//
// CREATED: 5/22/2014
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
#include "blobBuffers/BufferUtility.h"
#include "blobBuffers/GrayBuffer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using blobBuffers::BufferUtility;

const int BufferUtility::GRAY_CHANNEL = 1,
          BufferUtility::RGB_CHANNELS = 3,
          BufferUtility::FLIP_ON_BOTH_AXES = -1,
          BufferUtility::FLIP_VERTICAL = 0,
          BufferUtility::FLIP_HORIZONTAL = 1;

BufferUtility::BufferUtility() :
  flipVertical_( false ),
  flipHorizontal_( false )
{
}

BufferUtility::~BufferUtility()
{
}

void BufferUtility::process( unsigned char * pixels,
                             int width, 
                             int height,
                             int channels,
                             blobBuffers::GrayBuffer * dstImage )
{
    if(  width > 0 && height > 0 && dstImage != NULL ) {
        if( channels == GRAY_CHANNEL ) {
            copyGrayImage( pixels, width, height, dstImage );
        }
        else if( channels == RGB_CHANNELS ) {
            copyRgbImage( pixels, width, height, dstImage );
        }
    }
}

void BufferUtility::copyGrayImage( unsigned char * pixels, int width, int height,
                                   blobBuffers::GrayBuffer * dstImage )
{
    dstImage->copyPixels( pixels, width, height );
    flipIfRequested( dstImage );
}

void BufferUtility::copyRgbImage( unsigned char * pixels, int width, int height,
                                  blobBuffers::GrayBuffer * dstImage )
{
    cv::Mat srcM( height, width, CV_8UC3, pixels, width * 3 );
    cv::Mat dstM( height, width, CV_8U, dstImage->pixels(), width );
    cv::cvtColor( srcM, dstM, CV_RGB2GRAY, 1 );
    flipIfRequested( dstImage );
}

void BufferUtility::flipIfRequested( blobBuffers::GrayBuffer * buffer )
{
    if( (flipVertical_ || flipHorizontal_) && buffer != NULL ) {
        int width = buffer->width(),
            height = buffer->height();
        cv::Mat dstM( height, width, CV_8U, buffer->pixels(), width );

        if( flipVertical_ && flipHorizontal_ ) {
            cv::flip( dstM, dstM, FLIP_ON_BOTH_AXES );
        }
        else if( flipVertical_ ) {
            cv::flip( dstM, dstM, FLIP_VERTICAL );
        }
        else {
            cv::flip( dstM, dstM, FLIP_HORIZONTAL );
        }
    }
}

void BufferUtility::setFlipVertical( bool flip )
{
    flipVertical_ = flip;
}

bool BufferUtility::flipVertical()
{
    return flipVertical_;
}

void BufferUtility::setFlipHorizontal( bool flip )
{
    flipHorizontal_ = flip;
}

bool BufferUtility::flipHorizontal()
{
    return flipHorizontal_;
}
