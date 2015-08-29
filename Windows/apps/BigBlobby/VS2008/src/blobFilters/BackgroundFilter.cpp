////////////////////////////////////////////////////////////////////////////////
// BackgroundFilter
//
// PURPOSE: Knows how to subtract a background from an image an image.
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
#include "blobFilters/BackgroundFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobBuffers/GrayBufferShort.h"
#include "blobUtils/BackgroundTimer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using blobFilters::BackgroundFilter;

BackgroundFilter::BackgroundFilter() :
  bufferWidth_( 0 ),
  bufferHeight_( 0 ),
  srcCopy_( NULL ),
  backgroundImage_( NULL ),
  backgroundImageShort_( NULL ),
  backgroundImageShortTemp_( NULL ),

  srcCopyM_( NULL ),
  backgroundM_( NULL ),
  backgroundShortM_( NULL ),
  backgroundShortTempM_( NULL ),

  learnRate_( 0.0f ),
  frameCountDown_( 0 ),
  saveNextFrame_( false ),
  isAutoBackgroundActive_( false ),
  useDarkBlobs_( false ),
  useBlur_( true ),

  periodicBackgroundTimer_( new blobUtils::BackgroundTimer() ),
  periodicBackgroundPercent_( 0.0f ),
  periodicBackgroundFraction_( 0.0f )
{
}

BackgroundFilter::~BackgroundFilter()
{
    clear();
    delete periodicBackgroundTimer_;
}

void BackgroundFilter::clear()
{
    bufferWidth_ = 0;
    bufferHeight_ = 0;
    clearMatrices();
    clearBuffers();
}

void BackgroundFilter::clearMatrices()
{
    delete srcCopyM_;
    delete backgroundM_;
    delete backgroundShortM_;
    delete backgroundShortTempM_;

    srcCopyM_ = NULL;
    backgroundM_ = NULL;
    backgroundShortM_ = NULL;
    backgroundShortTempM_ = NULL;
}

void BackgroundFilter::clearBuffers()
{
    delete srcCopy_;
    delete backgroundImage_;
    delete backgroundImageShort_;
    delete backgroundImageShortTemp_;

    srcCopy_ = NULL;
    backgroundImage_ = NULL;
    backgroundImageShort_ = NULL;
    backgroundImageShortTemp_ = NULL;
}

blobBuffers::GrayBuffer * BackgroundFilter::backgroundImage()
{
    return backgroundImage_;
}

void BackgroundFilter::process( blobBuffers::GrayBuffer * srcImage,
                                blobBuffers::GrayBuffer * dstImage )
{
    if( srcImage != NULL && srcImage->isSameWidthAndHeight( dstImage ) ) {
        int cols = srcImage->width(),
            rows = srcImage->height();
        cv::Mat srcM( rows, cols, CV_8U, srcImage->pixels(), cols );
        cv::Mat dstM( rows, cols, CV_8U, dstImage->pixels(), cols );

        if( bufferWidth_ != cols || bufferHeight_ != rows ) {
            initialize( cols, rows );
            saveNextFrame_ = true;
        }
        if( isAutoBackgroundActive_ ) {
            updateWeightedBackground( srcM, rows, cols );
        }
        if( frameCountDown_ > 0 ) {
            --frameCountDown_;
            saveNextFrame_ = true;
        }
        if( saveNextFrame_ ) {
            updateBackground( srcM, rows, cols );
            saveNextFrame_ = false;
        }
        if( useDarkBlobs_ ) {
            cv::subtract( *backgroundM_, srcM, dstM );
        }
        else {
            cv::subtract( srcM, *backgroundM_, dstM );
        }
        srcCopy_->copyPixels( srcImage ); // Save copy for periodic background.
    }
}

void BackgroundFilter::updateWeightedBackground( cv::Mat & srcM,  int rows, int cols )
{
    srcM.convertTo( *backgroundShortTempM_, CV_16U, (65535.0f / 255.0f), 0 );
    cv::addWeighted( *backgroundShortTempM_, learnRate_, 
                     *backgroundShortM_, 1.0f - learnRate_,
                     0, *backgroundShortM_ );
    backgroundShortM_->convertTo( *backgroundM_, CV_8U, (255.0f / 65535.0f), 0 );
}

void BackgroundFilter::updateBackground( cv::Mat & srcM, int rows, int cols )
{
    srcM.copyTo( *backgroundM_ );
    srcM.convertTo( *backgroundShortM_, CV_16U, (65535.0f / 255.0f), 0 );
}

bool BackgroundFilter::isWithinBufferSize( const std::vector<cv::Point> & quad )
{
    int w = bufferWidth_,
        h = bufferHeight_,
        numPoints = (int)quad.size();

    for( int i = 0; i < numPoints; ++i ) {
        cv::Point p = quad.at( i );
        if( p.x < 0 || p.x >= w || p.y < 0 || p.y >= h ) {
            return false;
        }
    }
    return true;
}

void BackgroundFilter::initialize( int width, int height )
{
    clear();
    bufferWidth_ = width;
    bufferHeight_ = height;
    initializeBuffers();
    initializeMatrices();
}

void BackgroundFilter::initializeBuffers()
{
    srcCopy_ =  new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    backgroundImage_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    backgroundImageShort_ = new blobBuffers::GrayBufferShort( bufferWidth_, bufferHeight_ );
    backgroundImageShortTemp_ = new blobBuffers::GrayBufferShort( bufferWidth_, bufferHeight_ );
}

void BackgroundFilter::initializeMatrices()
{
    int rows = bufferHeight_,
        cols = bufferWidth_;

    srcCopyM_ = new cv::Mat( rows, cols, CV_8U, srcCopy_->pixels(), cols );
    backgroundM_ = new cv::Mat( rows, cols, CV_8U, backgroundImage_->pixels(), cols );
    backgroundShortM_ = new cv::Mat( rows, cols, CV_16U, backgroundImageShort_->pixels(), cols * 2 );
    backgroundShortTempM_ = new cv::Mat( rows, cols, CV_16U, backgroundImageShortTemp_->pixels(), cols * 2 );
}

void BackgroundFilter::saveNextFrameAsBackground()
{
    saveNextFrame_ = true;
}

void BackgroundFilter::saveFramesAsBackground( int count )
{
    frameCountDown_ = count;
}

void BackgroundFilter::setAutoBackgroundActive( bool b )
{
    isAutoBackgroundActive_ = b;
}

bool BackgroundFilter::isAutoBackgroundActive()
{
    return isAutoBackgroundActive_;
}

void BackgroundFilter::setBackgroundLearnRate( float rate )
{
    learnRate_ = rate;
}

void BackgroundFilter::useDarkBlobs( bool b )
{
    useDarkBlobs_ = b;
}

bool BackgroundFilter::useDarkBlobs()
{
    return useDarkBlobs_;
}

void BackgroundFilter::setPeriodicBackgroundActive( bool b )
{
    periodicBackgroundTimer_->setTimerActive( b );
}

bool BackgroundFilter::isPeriodicBackgroundActive()
{
    return periodicBackgroundTimer_->isTimerActive();
}

void BackgroundFilter::setPeriodicBackgroundSeconds( float t )
{
    periodicBackgroundTimer_->setIntervalInSeconds( t );
}

float BackgroundFilter::getPeriodicBackgroundSeconds()
{
    return (float)periodicBackgroundTimer_->intervalInSeconds();
}

void BackgroundFilter::setPeriodicBackgroundPercent( float percent )
{
    if( percent > 100 )    { periodicBackgroundPercent_ = 100.0f; }
    else if( percent < 0 ) { periodicBackgroundPercent_ = 0; }
    else                   { periodicBackgroundPercent_ = percent; }

    periodicBackgroundFraction_ = periodicBackgroundPercent_ / 100.0f;
}

float BackgroundFilter::getPeriodicBackgroundPercent()
{
    return periodicBackgroundPercent_;
}

void BackgroundFilter::checkPeriodicBackground()
{
    if( periodicBackgroundTimer_->needToUpdate() ) {
        // Use weighted average of last source image and background image.
        if( bufferWidth_ > 0 && bufferHeight_ > 0 && srcCopyM_ != NULL && backgroundM_ != NULL ) {
            float alpha = periodicBackgroundFraction_,
                  beta = 1.0f - alpha;
            cv::addWeighted( *srcCopyM_, alpha, *backgroundM_, beta, 0.0, *backgroundM_ );
            //std::cout << "(alpha, beta) = (" << alpha << ", " << beta << ")\n";
        }
    }
}
