////////////////////////////////////////////////////////////////////////////////
// HighpassFilter
//
// PURPOSE: Knows how to subtract a smoothed (blurred) image from the original
//          image.
//
// CREATED: 5/20/2014
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
#include "blobFilters/HighpassFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using blobFilters::HighpassFilter;

HighpassFilter::HighpassFilter() :
  bufferWidth_( 0 ),
  bufferHeight_( 0 ),
  blurredImage_( NULL ),
  blurredImageM_( NULL ),
  blurSize_( 0 ),
  isActive_( false ),
  showSubtractionImage_( false )
{
}

HighpassFilter::~HighpassFilter()
{
    clear();
}

void HighpassFilter::clear()
{
    bufferWidth_ = 0;
    bufferHeight_ = 0;
    delete blurredImage_;
    delete blurredImageM_;
    blurredImage_ = NULL;
    blurredImageM_ = NULL;
}

void HighpassFilter::process( blobBuffers::GrayBuffer * srcImage,
                              blobBuffers::GrayBuffer * dstImage )
{
    if( srcImage != NULL && srcImage->isSameWidthAndHeight( dstImage ) ) {
        int rows = srcImage->height(),
            cols = srcImage->width();

        if( bufferWidth_ != cols || bufferHeight_ != rows ) {
            initialize( cols, rows );
        }
        cv::Mat srcM( rows, cols, CV_8U, srcImage->pixels(), cols );
        cv::Mat dstM( rows, cols, CV_8U, dstImage->pixels(), cols );

        if( isActive_ && blurSize_ > 0 ) {
            int oddBlurSize = (blurSize_ * 2) + 1; // Always use odd number.
            cv::Size kernelSize( oddBlurSize, oddBlurSize );
            cv::blur( srcM, *blurredImageM_, kernelSize );
            cv::subtract( srcM, *blurredImageM_, dstM );
        }
        else {
            srcM.copyTo( dstM );
        }
    }
}

blobBuffers::GrayBuffer * HighpassFilter::blurredImage()
{
    return blurredImage_;
}

void HighpassFilter::initialize( int width, int height )
{
    clear();
    bufferWidth_ = width;
    bufferHeight_ = height;
    blurredImage_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );

    int rows = bufferHeight_,
        cols = bufferWidth_;
    blurredImageM_ = new cv::Mat( rows, cols, CV_8U, blurredImage_->pixels(), cols );
}

void HighpassFilter::setBlurSize( int blurSize )
{
    blurSize_ = blurSize;
}

int HighpassFilter::blurSize()
{
    return blurSize_;
}

void HighpassFilter::setActive( bool active )
{
    isActive_ = active;
}

bool HighpassFilter::isActive()
{
    return isActive_;
}

void HighpassFilter::showSubtractionImage( bool show )
{
    showSubtractionImage_ = show;
}

bool HighpassFilter::showSubtractionImage()
{
    return showSubtractionImage_;
}
