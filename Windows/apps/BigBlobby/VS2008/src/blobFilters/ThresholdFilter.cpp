////////////////////////////////////////////////////////////////////////////////
// ThresholdFilter
//
// PURPOSE: Knows how to apply a threshold value to an image.
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
#include "blobFilters/ThresholdFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using blobFilters::ThresholdFilter;

ThresholdFilter::ThresholdFilter() :
  threshold_( 50 ),
  isActive_( true )
{
}

ThresholdFilter::~ThresholdFilter()
{
}

void ThresholdFilter::process( blobBuffers::GrayBuffer * srcImage,
                               blobBuffers::GrayBuffer * dstImage )
{
    if( srcImage != NULL && srcImage->isSameWidthAndHeight( dstImage ) ) {
        int cols = srcImage->width(),
            rows = srcImage->height();

        cv::Mat srcM( rows, cols, CV_8U, srcImage->pixels(), cols );
        cv::Mat dstM( rows, cols, CV_8U, dstImage->pixels(), cols );

        if( isActive_ && threshold_ > 0 ) {
            cv::threshold( srcM, dstM, threshold_, 255.0, cv::THRESH_BINARY );

            //int blockSize = 71;
            //cv::adaptiveThreshold( srcM, dstM, 255.0, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blockSize, -threshold_ );
        }
        else {
            srcM.copyTo( dstM );
        }
    }
}

void ThresholdFilter::setThreshold( int level )
{
    // A level of zero results in way too many false blobs, so don't allow.
    if( level < 1 ) {
        level = 1;
    }
    else if( level > 255 ) { // Over 255 is off scale for one-byte grayscale.
        level = 255;
    }
    else {
        threshold_ = level;
    }
}

int ThresholdFilter::threshold()
{
    return threshold_;
}

void ThresholdFilter::setActive( bool active )
{
    isActive_ = active;
}

bool ThresholdFilter::isActive()
{
    return isActive_;
}
