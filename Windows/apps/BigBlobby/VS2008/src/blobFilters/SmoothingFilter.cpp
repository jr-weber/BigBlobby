////////////////////////////////////////////////////////////////////////////////
// SmoothingFilter
//
// PURPOSE: Knows how to smooth (blur) an image.
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
#include "blobFilters/SmoothingFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using blobFilters::SmoothingFilter;

SmoothingFilter::SmoothingFilter() :
  requestedBlurSize_( 0 ),
  blurSize_( 0 ),
  gaussianSigma_( 2.0 ),
  isActive_( false ),
  useGaussianBlur_( false )
{
}

SmoothingFilter::~SmoothingFilter()
{
}

void SmoothingFilter::process( blobBuffers::GrayBuffer * srcImage,
                               blobBuffers::GrayBuffer * dstImage )
{
    if( srcImage != NULL && srcImage->isSameWidthAndHeight( dstImage ) ) {
        int rows = srcImage->height(),
            cols = srcImage->width();
        cv::Mat srcM( rows, cols, CV_8U, srcImage->pixels(), cols );
        cv::Mat dstM( rows, cols, CV_8U, dstImage->pixels(), cols );

        if( isActive_ && blurSize_ > 0 ) {
            if( useGaussianBlur_ ) {
                useGaussianBlur( srcM, dstM );
            }
            else {
                useNormalizedBoxFilterBlur( srcM, dstM );
            }
        }
        else {
            srcM.copyTo( dstM );
        }
    }
}

void SmoothingFilter::useGaussianBlur( cv::Mat & srcM, cv::Mat & dstM )
{
    cv::Size kernelSize( blurSize_, blurSize_ );
    cv::GaussianBlur( srcM, dstM, kernelSize, gaussianSigma_ );
}

void SmoothingFilter::useNormalizedBoxFilterBlur( cv::Mat & srcM, cv::Mat & dstM )
{
    cv::Size kernelSize( blurSize_, blurSize_ );
    cv::blur( srcM, dstM, kernelSize );
}

void SmoothingFilter::setBlurSize( int blurSize )
{
    // The blur size for the box filter must be an odd number (or zero).
    requestedBlurSize_ = blurSize;
    blurSize_ = (blurSize > 0) ? (blurSize * 2 + 1) : 0;
}

int SmoothingFilter::blurSize()
{
    return requestedBlurSize_;
}

void SmoothingFilter::setActive( bool active )
{
    isActive_ = active;
}

bool SmoothingFilter::isActive()
{
    return isActive_;
}

void SmoothingFilter::useGaussianBlur( bool b )
{
    useGaussianBlur_ = b;
}

void SmoothingFilter::setGaussianBlurSigma( double sigma )
{
    if( sigma < 0.1 ) {
        gaussianSigma_ = 0.1;
    }
    else if( sigma > 4.0 ) {
        gaussianSigma_ = 4.0;
    }
    else {
        gaussianSigma_ = sigma;
    }
}

bool SmoothingFilter::useGaussianBlur()
{
    return useGaussianBlur_;
}

double SmoothingFilter::gaussianBlurSigma()
{
    return gaussianSigma_;
}
