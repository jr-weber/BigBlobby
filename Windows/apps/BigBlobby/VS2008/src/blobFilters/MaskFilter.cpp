////////////////////////////////////////////////////////////////////////////////
// MaskFilter
//
// PURPOSE: Generates a mask to block out image regions outside of the 
//          calibration grid.
//
// CREATED: 12/5/2014
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
#include "blobFilters/MaskFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobNormalize/CalibrationGridPoints.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using blobFilters::MaskFilter;

MaskFilter::MaskFilter() :
  isActive_( false ),
  bufferWidth_( 0 ),
  bufferHeight_( 0 ),
  calibrationMask_( NULL ),
  calibrationMaskM_( NULL ),
  calGridPoints_( NULL ),
  maskPoints_( NULL ),
  widthPadding_( 0 ),
  heightPadding_( 0 ),
  needToUpdateMask_( false )
{
}

MaskFilter::~MaskFilter()
{
    clear();
    delete calGridPoints_;
    delete maskPoints_;
}

void MaskFilter::clear()
{
    bufferWidth_ = 0;
    bufferHeight_ = 0;
    delete calibrationMask_;
    delete calibrationMaskM_;
    calibrationMask_ = NULL;
    calibrationMaskM_ = NULL;
}

void MaskFilter::process( blobBuffers::GrayBuffer * srcImage,
                          blobBuffers::GrayBuffer * dstImage )
{
    if( srcImage != NULL && srcImage->isSameWidthAndHeight( dstImage ) ) {
        int cols = srcImage->width(),
            rows = srcImage->height();

        if( bufferWidth_ != cols || bufferHeight_ != rows ) {
            initialize( cols, rows );
        }
        if( needToUpdateMask_ ) {
            updateCalibrationGridMask();
            needToUpdateMask_ = false;
        }
        cv::Mat srcM( rows, cols, CV_8U, srcImage->pixels(), cols );
        cv::Mat dstM( rows, cols, CV_8U, dstImage->pixels(), cols );

        if( isActive_ && maskPoints_ != NULL ) {
            cv::multiply( srcM, *calibrationMaskM_, dstM );
        }
        else {
            srcM.copyTo( dstM );
        }
    }
}

void MaskFilter::initialize( int width, int height )
{
    clear();
    bufferWidth_ = width;
    bufferHeight_ = height;
    calibrationMask_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );

    int rows = bufferHeight_,
        cols = bufferWidth_;
    calibrationMaskM_ = new cv::Mat( rows, cols, CV_8U, calibrationMask_->pixels(), cols );
}

void MaskFilter::setActive( bool active )
{
    isActive_ = active;
}

bool MaskFilter::isActive()
{
    return isActive_;
}

void MaskFilter::setWidthPadding( int pixels )
{
    widthPadding_ = (pixels >= 0) ? pixels : 0;
    needToUpdateMask_ = true;
    //std::cout << "widthPadding_ = " << widthPadding_ << "\n";
}

int MaskFilter::widthPadding()
{
    return widthPadding_;
}

void MaskFilter::setHeightPadding( int pixels )
{
    heightPadding_ = (pixels >= 0) ? pixels : 0;
    needToUpdateMask_ = true;
    //std::cout << "heightPadding_ = " << heightPadding_ << "\n";
}

int MaskFilter::heightPadding()
{
    return heightPadding_;
}

void MaskFilter::setCalibrationGridPoints( blobNormalize::CalibrationGridPoints * calGridPoints )
{
    if( calGridPoints != NULL ) {
        delete calGridPoints_;
        delete maskPoints_;
        calGridPoints_ = calGridPoints;
        maskPoints_ = new blobNormalize::CalibrationGridPoints( calGridPoints_ );
        needToUpdateMask_ = true;
    }
}

void MaskFilter::updateCalibrationGridMask()
{
    if( calGridPoints_ != NULL && maskPoints_ != NULL ) {
        expandOuterEdgeOfMaskPoints();
        generateMask();
    }
}

void MaskFilter::expandOuterEdgeOfMaskPoints()
{
    maskPoints_->set( calGridPoints_ );
    expandHeightOfUnmaskedArea();
    expandWidthOfUnmaskedArea();
}

void MaskFilter::expandHeightOfUnmaskedArea()
{
    float deltaY = (float)heightPadding_;
    int rows = maskPoints_->numRows(),
        cols = maskPoints_->numCols();

    if( rows > 1 ) {
        int firstRow = 0,
            lastRow = rows - 1;

        for( int col = 0; col < cols; ++col ) {
            maskPoints_->incrementY( -deltaY, firstRow, col );
            maskPoints_->incrementY( deltaY, lastRow, col );
        }
    }
}

void MaskFilter::expandWidthOfUnmaskedArea()
{
    float deltaX = (float)widthPadding_;
    int rows = maskPoints_->numRows(),
        cols = maskPoints_->numCols();

    if( cols > 1 ) {
        int firstCol = 0,
            lastCol = cols - 1;

        for( int row = 0; row < rows; ++row ) {
            maskPoints_->incrementX( -deltaX, row, firstCol );
            maskPoints_->incrementX( deltaX, row, lastCol );
        }
    }
}

void MaskFilter::generateMask()
{
    if( calibrationMask_ != NULL ) {
        calibrationMask_->zeroAllPixels();
        int lastRowIndex = maskPoints_->numRows() - 1,
            lastColIndex = maskPoints_->numCols() - 1;

        for( int row = 0; row < lastRowIndex; ++row ) {
            for( int col = 0; col < lastColIndex; ++col ) {
                fillConvexPoly( row, col );
            }
        }
    }
}

void MaskFilter::fillConvexPoly( int row, int col )
{
    int rowPlusOne = row + 1,
        colPlusOne = col + 1;
    blobNormalize::GridPoint p0 = maskPoints_->gridPoint( row, col ),
                               p1 = maskPoints_->gridPoint( rowPlusOne, col ),
                               p2 = maskPoints_->gridPoint( rowPlusOne, colPlusOne ),
                               p3 = maskPoints_->gridPoint( row, colPlusOne );
    std::vector<cv::Point> quad;
    quad.push_back( cv::Point( (int)p0.x(), (int)p0.y() ) );
    quad.push_back( cv::Point( (int)p1.x(), (int)p1.y() ) );
    quad.push_back( cv::Point( (int)p2.x(), (int)p2.y() ) );
    quad.push_back( cv::Point( (int)p3.x(), (int)p3.y() ) );

    if( isWithinBufferSize( quad ) ) {
        cv::fillConvexPoly( *calibrationMaskM_, quad, cv::Scalar(1), 8, 0 );
    }
}

bool MaskFilter::isWithinBufferSize( const std::vector<cv::Point> & quad )
{
    int w = bufferWidth_,
        h = bufferHeight_,
        numPoints = (int)quad.size();

    for( int i = 0; i < numPoints; ++i ) {
        cv::Point p = quad.at( i );
        if( p.x < 0 || p.x > w || p.y < 0 || p.y > h ) {
            return false;
        }
    }
    return true;
}
