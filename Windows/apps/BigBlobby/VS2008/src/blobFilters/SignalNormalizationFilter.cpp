////////////////////////////////////////////////////////////////////////////////
// SignalNormalizationFilter
//
// PURPOSE: Uses signal intensities (source minus background) from the 
//          calibration grid blobs to slightly amplify weaker regions of the 
//          camera image.  Edges, and especially corners, of the image tend to
//          have weaker signals, so a slight amplification of these weaker 
//          regions may be helpful in getting consistent blob detection.
//          Areas with a higher background also tend to have a smaller blob
//          (fingertip) signal after background subtraction.
//
// CREATED: 2/1/2015
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#include "blobFilters/SignalNormalizationFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobNormalize/FishEyeGrid.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>

using blobFilters::SignalNormalizationFilter;

SignalNormalizationFilter::SignalNormalizationFilter() :
  isActive_( false ),
  bufferWidth_( 0 ),
  bufferHeight_( 0 ),
  fishEyeGrid_( NULL ),
  temp32fMat_( NULL ),
  normalization32fMat_( NULL ),
  needToUpdateNormalization_( false )
{
}

SignalNormalizationFilter::~SignalNormalizationFilter()
{
    clear();
    delete fishEyeGrid_;
}

void SignalNormalizationFilter::clear()
{
    bufferWidth_ = 0;
    bufferHeight_ = 0;
    delete temp32fMat_;
    delete normalization32fMat_;
    temp32fMat_ = NULL;
    normalization32fMat_ = NULL;
}

void SignalNormalizationFilter::process( blobBuffers::GrayBuffer * srcImage,
                                         blobBuffers::GrayBuffer * dstImage )
{
    if( srcImage != NULL && srcImage->isSameWidthAndHeight( dstImage ) ) {
        int cols = srcImage->width(),
            rows = srcImage->height();

        if( bufferWidth_ != cols || bufferHeight_ != rows ) {
            initialize( cols, rows );
        }
        if( needToUpdateNormalization_ ) {
            updateNormalizationMatrix();
            needToUpdateNormalization_ = false;
        }
        cv::Mat srcM( rows, cols, CV_8U, srcImage->pixels(), cols );
        cv::Mat dstM( rows, cols, CV_8U, dstImage->pixels(), cols );

        if( isActive_ && fishEyeGrid_ != NULL ) {
            srcM.convertTo( *temp32fMat_, CV_32F, 1.0 / 255.0 );
            cv::multiply( *temp32fMat_, *normalization32fMat_, *temp32fMat_ );
            temp32fMat_->convertTo( dstM, CV_8U, 255.0 );
        }
        else {
            srcM.copyTo( dstM );
        }
    }
}

void SignalNormalizationFilter::initialize( int width, int height )
{
    clear();
    bufferWidth_ = width;
    bufferHeight_ = height;

    int rows = bufferHeight_,
        cols = bufferWidth_;
    temp32fMat_ = new cv::Mat( rows, cols, CV_32F );
    normalization32fMat_ = new cv::Mat( rows, cols, CV_32F );
}

void SignalNormalizationFilter::updateNormalizationMatrix()
{
    if( fishEyeGrid_ != NULL && normalization32fMat_ != NULL ) {
        int numRows = normalization32fMat_->rows,
            numCols = normalization32fMat_->cols;

        for( int row = 0; row < numRows; ++row ) {
            for( int col = 0; col < numCols; ++col ) {
                float x = (float)col,
                      y = (float)row;
                float intensityNorm = fishEyeGrid_->intensityNormalizationFactor( x, y );
                normalization32fMat_->at<float>( row, col ) = intensityNorm;
            }
        }
        //debugPrintFishEyeGrid();
    }
}

void SignalNormalizationFilter::setActive( bool active )
{
    isActive_ = active;
}

bool SignalNormalizationFilter::isActive()
{
    return isActive_;
}

void SignalNormalizationFilter::setFishEyeGrid( blobNormalize::FishEyeGrid * fishEyeGrid )
{
    if( fishEyeGrid != NULL ) {
        delete fishEyeGrid_;
        fishEyeGrid_ = fishEyeGrid;
        needToUpdateNormalization_ = true;
        //debugPrintFishEyeGridDimensions();
    }
}

void SignalNormalizationFilter::debugPrintFishEyeGridDimensions()
{
    if( fishEyeGrid_ != NULL ) {
        int rows = fishEyeGrid_->numRows(),
            cols = fishEyeGrid_->numCols();

        std::cout << "\nSignalNormalizationFilter::setFishEyeGrid() called...\n"
                  << "    (rows, cols) = (" << rows << ", " << cols << ")\n\n";
    }
}

void SignalNormalizationFilter::debugPrintFishEyeGrid()
{
    if( normalization32fMat_ != NULL ) {
        int numRows = normalization32fMat_->rows,
            numCols = normalization32fMat_->cols;
        std::cout << "\nSignalNormalizationFilter::debugPrintFishEyeGrid()...\n\n";

        for( int row = 0; row < numRows; ++row ) {
            for( int col = 0; col < numCols; ++col ) {
                float n = normalization32fMat_->at<float>( row, col );
                std::cout << std::fixed << std::setprecision( 2 ) << n << "  ";
            }
            std::cout << "\n";
        }
    }
}
