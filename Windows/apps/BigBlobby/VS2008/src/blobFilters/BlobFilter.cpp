////////////////////////////////////////////////////////////////////////////////
// BlobFilter
//
// PURPOSE: This superclass for image filters holds a function timer object and
//          provides a virtual process( src, dst ) function that all 
//          subclasses must implement.
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
#include "blobFilters/BlobFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobUtils/FunctionTimer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using blobFilters::BlobFilter;

BlobFilter::BlobFilter() :
  processTimer_( new blobUtils::FunctionTimer() ),
  isAmplifyActive_( false ),
  amplifyLevel_( 0 ),
  amplifyScale_( 0.0 )
{
}

BlobFilter::~BlobFilter()
{
    delete processTimer_;
}

void BlobFilter::processImage( blobBuffers::GrayBuffer * src, blobBuffers::GrayBuffer * dst )
{
    processTimer_->startTiming();
    process( src, dst );
    amplify( dst );
    processTimer_->stopTiming();
}

void BlobFilter::amplify( blobBuffers::GrayBuffer * grayBuffer )
{
    if( isAmplifyActive_ && amplifyScale_ > 0 && grayBuffer != NULL ) {
        int cols = grayBuffer->width(),
            rows = grayBuffer->height();
        cv::Mat matrix( rows, cols, CV_8U, grayBuffer->pixels(), cols );
        cv::multiply( matrix, matrix, matrix, amplifyScale_ );
    }
}

double BlobFilter::calculateAverageTime()
{
    return processTimer_->calculateAverageTime();
}

void BlobFilter::setAmplifyActive( bool active )
{
    isAmplifyActive_ = active;
}

bool BlobFilter::isAmplifyActive()
{
    return isAmplifyActive_;
}

void BlobFilter::setAmplifyLevel( int level )
{
    amplifyLevel_ = level;
    amplifyScale_ = amplifyLevel_ / 128.0f;
}

int BlobFilter::amplifyLevel()
{
    return amplifyLevel_;
}
