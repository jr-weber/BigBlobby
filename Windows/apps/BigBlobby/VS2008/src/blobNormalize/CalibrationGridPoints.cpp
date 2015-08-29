////////////////////////////////////////////////////////////////////////////////
// CalibrationGridPoints
//
// PURPOSE: Holds information on the calibration grid used to match camera 
//          space to screen space.  This class was written to make it easy 
//          to pass info from the BlobCalibration class to the ImageFilters 
//          class so that the BackgroundFilter can create a mask to eliminate 
//          areas outside of the calibration grid.
//
// CREATED: 6/6/2014
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
#include "blobNormalize/CalibrationGridPoints.h"
#include "blobNormalize/FishEyeGrid.h"
#include "blobBuffers/GrayBuffer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using blobNormalize::CalibrationGridPoints;

const int CalibrationGridPoints::DEFAULT_CAMERA_WIDTH = 360,
          CalibrationGridPoints::DEFAULT_CAMERA_HEIGHT = 240;
const float CalibrationGridPoints::MAX_CORRECTION_FACTOR = 2.0f;
const int CalibrationGridPoints::NUM_POINTS_FOR_AVG_MAX_INTENSITY = 5;
const float CalibrationGridPoints::MIN_SRC_MINUS_BK_NORMALIZATION = 1.0f,
            CalibrationGridPoints::MAX_SRC_MINUS_BK_NORMALIZATION = 2.0f;

CalibrationGridPoints::CalibrationGridPoints( int rows, int cols ) :
  numRows_( rows ),
  numCols_( cols ),
  gridPoints_( rows * cols ),
  avgMaxIntensity_( 0.0f ),
  camWidth_( DEFAULT_CAMERA_WIDTH ),
  camHeight_( DEFAULT_CAMERA_HEIGHT )
{
}

CalibrationGridPoints::CalibrationGridPoints( CalibrationGridPoints * other ) :
  numRows_( other->numRows_ ),
  numCols_( other->numCols_ ),
  gridPoints_( other->gridPoints_ ),
  avgMaxIntensity_( other->avgMaxIntensity_ ),
  camWidth_( other->camWidth_ ),
  camHeight_( other->camHeight_ )
{
}

CalibrationGridPoints::~CalibrationGridPoints()
{
}

/***************************************************************************//**
Returns the number of rows of points (vertices) in the calibration grid.  The 
number of rows of points is one greater than the number of rows of cells seen 
in a wireframe.
*******************************************************************************/
int CalibrationGridPoints::numRows()
{
    return numRows_;
}

/***************************************************************************//**
Returns the number of columns of points (vertices) in the calibration grid.  The
number of columns of points is one greater than the number of columns of cells
seen in a wireframe.
*******************************************************************************/
int CalibrationGridPoints::numCols()
{
    return numCols_;
}

/***************************************************************************//**
The number of points is the same as the number of columns multiplied by the
number of rows.
*******************************************************************************/
int CalibrationGridPoints::numPoints()
{
    return gridPoints_.size();
}

/***************************************************************************//**
Sets the (x, y) coordinates in pixels for a point at a row and column of the 
grid.
*******************************************************************************/
void CalibrationGridPoints::setPointXY( float x, float y, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        gridPoints_.at( index ).setXY( x, y );
    }
}

void CalibrationGridPoints::setSourceIntensity( float intensity, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        gridPoints_.at( index ).setSourceIntensity( intensity );
    }
}

void CalibrationGridPoints::setBackgroundIntensity( float intensity, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        gridPoints_.at( index ).setBackgroundIntensity( intensity );
    }
}

/***************************************************************************//**
Copies all data from the CalibrationGridPoints object given as an argument.
*******************************************************************************/
void CalibrationGridPoints::set( CalibrationGridPoints * other )
{
    numRows_ = other->numRows_;
    numCols_ = other->numCols_;
    gridPoints_ = other->gridPoints_;
    avgMaxIntensity_ = other->avgMaxIntensity_;
    camWidth_ = other->camWidth_;
    camHeight_ = other->camHeight_;
}

/***************************************************************************//**
Returns a copy of the grid point at the requested row and column.  A GridPoint 
has xy-coordinates in pixels and an associated signal intensity, which will 
usually be highest in the center of the calibration grid and lowest near the
edges and corners.
*******************************************************************************/
blobNormalize::GridPoint CalibrationGridPoints::gridPoint( int row, int col )
{
    GridPoint p;
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        p = gridPoints_[index];
    }
    return p;
}

/***************************************************************************//**
Increments x by deltaX, but checks that the new value of x is within bounds.
If the new value is less than 0, x is set to 0.  If the new value is greater 
than the camera width, x is set to the camera width.
*******************************************************************************/
void CalibrationGridPoints::incrementX( float deltaX, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        float x = gridPoints_[index].x() + deltaX,
              y = gridPoints_[index].y();

        if( x < 0.0f ) { 
            x = 0.0f; 
        }
        else if( x > camWidth_ ) {
            x = (float)camWidth_;
        }
        gridPoints_[index].setXY( x, y );
    }
}

/***************************************************************************//**
Increments y by deltaY, but checks that the new value of y is within bounds.
If the new value is less than 0, y is set to 0.  If the new value is greater 
than the camera height, y is set to the camera height.
*******************************************************************************/
void CalibrationGridPoints::incrementY( float deltaY, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        float x = gridPoints_[index].x(),
              y = gridPoints_[index].y() + deltaY;

        if( y < 0.0f ) { 
            y = 0.0f; 
        }
        else if( y > camHeight_ ) {
            y = (float)camHeight_;
        }
        gridPoints_[index].setXY( x, y );
    }
}

/***************************************************************************//**
Expects the camera image dimensions in pixels.  Usually this will be VGA 
(640 x 480) or quarter-VGA (320 x 240).
*******************************************************************************/
void CalibrationGridPoints::setCameraWidthAndHeight( int width, int height )
{
    camWidth_ = width;
    camHeight_ = height;
}

/***************************************************************************//**
The camera image width in pixels.
*******************************************************************************/
int CalibrationGridPoints::camWidth()
{
    return camWidth_;
}

/***************************************************************************//**
The camera image height in pixels.
*******************************************************************************/
int CalibrationGridPoints::camHeight()
{
    return camHeight_;
}

/***************************************************************************//**
Returns true if the integer is odd.
*******************************************************************************/
bool CalibrationGridPoints::isOdd( int n )
{
    return ((n % 2) == 1);
}

/***************************************************************************//**
Assumes that the sourceIntensities and the backgroundIntensities have already
been set.
*******************************************************************************/
void CalibrationGridPoints::calculateSrcMinusBkIntensities()
{
    int length = gridPoints_.size();

    for( int i = 0; i < length; ++i ) {
        blobNormalize::GridPoint & point = gridPoints_.at( i );
        point.calculateSrcMinusBackground();
    }
}

/***************************************************************************//**
Calculates a normalization (correction) factor for grid points that are below
the maximum signal intensity for a calibration blob.  Instead of the using the 
max signal for one point, the average of the top 5 signals is used.  Also, the
correction factor must be between the MIN_SRC_MINUS_BK_NORMALIZATION and the 
MAX_SRC_MINUS_BK_NORMALIZATION.
*******************************************************************************/
void CalibrationGridPoints::calculateIntensityNormalizations()
{
    avgMaxIntensity_ = calculateAverageMaxIntensity();
    int length = gridPoints_.size();

    for( int i = 0; i < length; ++i ) {
        blobNormalize::GridPoint & point = gridPoints_.at( i );
        float srcMinusBk = point.srcMinusBkIntensity(),
              normalization = MIN_SRC_MINUS_BK_NORMALIZATION;

        if( srcMinusBk > 1.0f && avgMaxIntensity_ > 1.0f ) {
            normalization = avgMaxIntensity_ / srcMinusBk;

            if( normalization < MIN_SRC_MINUS_BK_NORMALIZATION ) {
                normalization = MIN_SRC_MINUS_BK_NORMALIZATION;
            }
            else if( normalization > MAX_SRC_MINUS_BK_NORMALIZATION ) {
                normalization = MAX_SRC_MINUS_BK_NORMALIZATION;
            }
        }
        point.setIntensityNormalization( normalization );
    }
    //debugPrintCellIntensities();
}

/***************************************************************************//**
Takes the average of the highest N srcMinusBkIntensity values, where N is
NUM_POINTS_FOR_AVG_MAX_INTENSITY.
*******************************************************************************/
float CalibrationGridPoints::calculateAverageMaxIntensity()
{
    float avgMaxIntensity = 0.0f;
    int length = gridPoints_.size();
    std::vector<float> intensities;

    for( int i = 0; i < length; ++i ) {
        float srcMinusBk = gridPoints_.at( i ).srcMinusBkIntensity();
        intensities.push_back( srcMinusBk );
    }
    std::sort( intensities.begin(), intensities.end() );
    std::reverse( intensities.begin(), intensities.end() );
    float sum = 0.0f;
    int count = 0;

    for( int i = 0; i < length && i < NUM_POINTS_FOR_AVG_MAX_INTENSITY; ++i ) {
        sum += intensities.at( i );
        ++count;
    }
    if( count > 0 ) {
        avgMaxIntensity = sum / count;
    }
    return avgMaxIntensity;
}

void CalibrationGridPoints::updateBackgroundIntensities( blobBuffers::GrayBuffer * backgroundBuffer )
{
    //std::cout << "CalibrationGridPoints::updateBackgroundIntensities() called.\n";
    if( backgroundBuffer != NULL ) {
        int length = gridPoints_.size();

        for( int i = 0; i < length; ++i ) {
            blobNormalize::GridPoint & point = gridPoints_.at( i );
            int row = (int)(point.y() + 0.5f),
                col = (int)(point.x() + 0.5f);

            float bkIntensity = (float)backgroundBuffer->pixel( row, col );
            point.setBackgroundIntensity( bkIntensity );
        }
    }
}

/***************************************************************************//**
Intensities should have been calculated before this function is called.
*******************************************************************************/
blobNormalize::FishEyeGrid * CalibrationGridPoints::createFishEyeGrid()
{
    return new blobNormalize::FishEyeGrid( this );
}

/***************************************************************************//**
Prints to standard out for debugging purposes.
*******************************************************************************/
void CalibrationGridPoints::debugPrintCellIntensities()
{
    std::cout << "\nCalibrationGridPoints::debugPrintCellIntensities() called.\n\n";

    for( int row = 0; row < numRows_; ++row ) {
        std::cout << "row: " << row << "\n\n";

        for( int col = 0; col < numCols_; ++col ) {
            blobNormalize::GridPoint p = gridPoint( row, col );
            std::cout << std::fixed << std::setprecision( 0 ) << "(" 
                      << p.x() << ", " 
                      << p.y() << ")"
                      << std::setprecision( 2 )
                      << "; srcMinusBk = " << p.srcMinusBkIntensity()
                      << "; src = " << p.sourceIntensity()
                      << "; bk = " << p.backgroundIntensity()
                      << "; norm = " << p.intensityNormalization() << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
