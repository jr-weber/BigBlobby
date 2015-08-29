////////////////////////////////////////////////////////////////////////////////
// FishEyeGrid
//
// PURPOSE: Provides a look-up table for adjusting blob intensity based on the 
//          "fish eye" effect seen with some camera lenses.
//
// CREATED: 6/14/2014
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
#include "blobNormalize/FishEyeGrid.h"
#include "blobNormalize/CalibrationGridPoints.h"
#include <iostream>
#include <iomanip>

using blobNormalize::FishEyeGrid;
using blobNormalize::CalibrationGridPoints;

FishEyeGrid::FishEyeGrid( CalibrationGridPoints * calGridPoints ) :
  gridPoints_(),
  gridStepsX_(),
  gridStepsY_(),
  numRows_( 0 ),
  numCols_( 0 ),
  camWidth_( calGridPoints->camWidth() ),
  camHeight_( calGridPoints->camHeight() )
{
    calculateGridStepsX( calGridPoints );
    calculateGridStepsY( calGridPoints );
    calculateGridXY();
    copyIntensityInfo( calGridPoints );
    fixBorderGridPoints();
    //debugPrintGrid();
}

FishEyeGrid::FishEyeGrid( blobNormalize::FishEyeGrid * other ) :
  gridPoints_( other->gridPoints_ ),
  gridStepsX_( other->gridStepsX_ ),
  gridStepsY_( other->gridStepsY_ ),
  numRows_( other->numRows_ ),
  numCols_( other->numCols_ ),
  camWidth_( other->camWidth_ ),
  camHeight_( other->camHeight_ )
{
    
}

FishEyeGrid::~FishEyeGrid()
{
}

void FishEyeGrid::calculateGridStepsX( CalibrationGridPoints * calGridPoints )
{
    gridStepsX_.push_back( 0.0f );

    for( int col = 0; col < calGridPoints->numCols(); ++col ) {
        float sum = 0.0f;
        int count = 0;
         
        for( int row = 0; row < calGridPoints->numRows(); ++row ) {
            sum += calGridPoints->gridPoint( row, col ).x();
            ++count;
        }
        if( count > 0 ) {
            float x = sum / count;
            gridStepsX_.push_back( x );
        }
    }
    gridStepsX_.push_back( (float)camWidth_ );
    numCols_ = gridStepsX_.size();
}

void FishEyeGrid::calculateGridStepsY( CalibrationGridPoints * calGridPoints )
{
    gridStepsY_.push_back( 0.0f );

    for( int row = 0; row < calGridPoints->numRows(); ++row ) {
         float sum = 0.0f;
         int count = 0;
         
         for( int col = 0; col < calGridPoints->numCols(); ++col ) {
             sum += calGridPoints->gridPoint( row, col ).y();
             ++count;
         }
         if( count > 0 ) {
             float y = sum / count;
             gridStepsY_.push_back( y );
         }
    }
    gridStepsY_.push_back( (float)camHeight_ );
    numRows_ = gridStepsY_.size();
}

void FishEyeGrid::calculateGridXY()
{
    gridPoints_.resize( numRows_ * numCols_ );
    int count = 0;

    for( int row = 0; row < numRows_; ++row ) {
        float y = gridStepsY_.at( row );

        for( int col = 0; col < numCols_; ++col ) {
            float x = gridStepsX_.at( col );
            
            gridPoints_.at( count ).setXY( x, y );
            ++count;
        }
    }
}

void FishEyeGrid::copyIntensityInfo( CalibrationGridPoints * calGridPoints )
{
    for( int row = 0; row < calGridPoints->numRows(); ++ row ) {
        int rowPlusOne = row + 1;

        for( int col = 0; col < calGridPoints->numCols(); ++ col ) {
            int colPlusOne = col + 1;
            GridPoint p = calGridPoints->gridPoint( row, col );
            setPointSrcMinusBkIntensity( p.srcMinusBkIntensity(), rowPlusOne, colPlusOne );
            setPointSourceIntensity( p.sourceIntensity(), rowPlusOne, colPlusOne );
            setPointBackgroundIntensity( p.backgroundIntensity(), rowPlusOne, colPlusOne );
            setIntensityNormalization( p.intensityNormalization(), rowPlusOne, colPlusOne );
        }
    }
}

void FishEyeGrid::fixBorderGridPoints()
{
    if( numRows_ > 2 && numCols_ > 2 ) {
        fixRowZero();
        fixLastRow();
        fixFirstColumn();
        fixLastColumn();
    }
}

void FishEyeGrid::fixRowZero()
{
    int lastColIndex = numCols_ - 1;

    for( int col = 1; col < lastColIndex; ++col ) {
        GridPoint p = gridPoint( 1, col );
        setPointSrcMinusBkIntensity( p.srcMinusBkIntensity(), 0, col );
        setPointSourceIntensity( p.sourceIntensity(), 0, col );
        setPointBackgroundIntensity( p.backgroundIntensity(), 0, col );
        setIntensityNormalization( p.intensityNormalization(), 0, col );
    }
}

void FishEyeGrid::fixLastRow()
{
    int lastRowIndex = numRows_ - 1,
        nextToLastRowIndex = lastRowIndex - 1,
        lastColIndex = numCols_ - 1;

    for( int col = 1; col < lastColIndex; ++col ) {
        GridPoint p = gridPoint( nextToLastRowIndex, col );
        setPointSrcMinusBkIntensity( p.srcMinusBkIntensity(), lastRowIndex, col );
        setPointSourceIntensity( p.sourceIntensity(), lastRowIndex, col );
        setPointBackgroundIntensity( p.backgroundIntensity(), lastRowIndex, col );
        setIntensityNormalization( p.intensityNormalization(), lastRowIndex, col );
    }
}

void FishEyeGrid::fixFirstColumn()
{
    for( int row = 0; row < numRows_; ++row ) {
        GridPoint p = gridPoint( row, 1 );
        setPointSrcMinusBkIntensity( p.srcMinusBkIntensity(), row, 0 );
        setPointSourceIntensity( p.sourceIntensity(), row, 0 );
        setPointBackgroundIntensity( p.backgroundIntensity(), row, 0 );
        setIntensityNormalization( p.intensityNormalization(), row, 0 );
    }
}

void FishEyeGrid::fixLastColumn()
{
    int lastColIndex = numCols_ - 1,
        nextToLastColIndex = lastColIndex - 1;

    for( int row = 0; row < numRows_; ++row ) {
        GridPoint p = gridPoint( row, nextToLastColIndex );
        setPointSrcMinusBkIntensity( p.srcMinusBkIntensity(), row, lastColIndex );
        setPointSourceIntensity( p.sourceIntensity(), row, lastColIndex );
        setPointBackgroundIntensity( p.backgroundIntensity(), row, lastColIndex );
        setIntensityNormalization( p.intensityNormalization(), row, lastColIndex );
    }
}

int FishEyeGrid::numRows()
{
    return numRows_;
}

int FishEyeGrid::numCols()
{
    return numCols_;
}

int FishEyeGrid::numPoints()
{
    return gridPoints_.size();
}

void FishEyeGrid::setPointSrcMinusBkIntensity( float intensity, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        gridPoints_.at( index ).setSrcMinusBkIntensity( intensity );
    }
}

void FishEyeGrid::setPointSourceIntensity( float intensity, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        gridPoints_.at( index ).setSourceIntensity( intensity );
    }
}

void FishEyeGrid::setPointBackgroundIntensity( float intensity, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        gridPoints_.at( index ).setBackgroundIntensity( intensity );
    }
}

void FishEyeGrid::setIntensityNormalization( float intensity, int row, int col )
{
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >= 0 && index < size ) {
        gridPoints_.at( index ).setIntensityNormalization( intensity );
    }
}

/***************************************************************************//**
Returns a copy of the GridPoint.
*******************************************************************************/
blobNormalize::GridPoint FishEyeGrid::gridPoint( int row, int col )
{
    GridPoint p;
    int index = (row * numCols_) + col;
    int size = gridPoints_.size();

    if( index >=0 && index < size ) {
        p = gridPoints_.at( index );
    }
    return p;
}

/***************************************************************************//**
Before this function is called, x should have been determined to have a value 
of at least gridStepsX_[col], but less than gridStepsX_[col+1].  This function
will determine the relative distance of y between gridStepsX_[col] and 
gridStepsX_[col], but will never return a value less than 0 or greater than 1.
*******************************************************************************/
float FishEyeGrid::calculateDeltaX( float x, int col )
{
    float startX = gridStepsX_.at( col ),
          endX = gridStepsX_.at( col + 1 ),
          deltaX = (x - startX) / (endX - startX);

    if( deltaX < 0 ) { 
        deltaX = 0.0f; 
    }
    else if( deltaX > 1.0f ) {
        deltaX = 1.0f;
    }
    return deltaX;
}

/***************************************************************************//**
Before this function is called, y should have been determined to have a value 
of at least gridStepsY_[row], but less than gridStepsY_[row+1].  This function
will determine the relative distance of y between gridStepsY_[row] and 
gridStepsY_[row], but will never return a value less than 0 or greater than 1.
*******************************************************************************/
float FishEyeGrid::calculateDeltaY( float y, int row )
{
    float startY = gridStepsY_.at( row ),
          endY = gridStepsY_.at( row + 1 ),
          deltaY = (y - startY) / (endY - startY);

    if( deltaY < 0 ) { 
        deltaY = 0.0f; 
    }
    else if( deltaY > 1.0f ) {
        deltaY = 1.0f;
    }
    return deltaY;
}

/***************************************************************************//**
Returns a cell row number that the y-value falls in.
*******************************************************************************/
int FishEyeGrid::cellRow( float y )
{
    int lastRowIndex = numRows_ - 1;

    for( int row = 0; row < lastRowIndex; ++row ) {
        if( y <= gridStepsY_.at( row + 1 ) ) {
            return row;
        }
    }
    return lastRowIndex - 1;
}

/***************************************************************************//**
Returns a cell column number that the x-value falls in.
*******************************************************************************/
int FishEyeGrid::cellCol( float x )
{
    int lastColIndex = numCols_ - 1;

    for( int col = 0; col < lastColIndex; ++col ) {
        if( x <= gridStepsX_.at( col + 1 ) ) {
            return col;
        }
    }
    return lastColIndex - 1;
}

/***************************************************************************//**
When an (x, y) position for an image is plugged in, this function returns a 
normalization factor for the blob intensity.  The normalization factor should 
be a value between 1.0 and 2.0, inclusive.  This factor is intended to boost 
the signal strength in regions of the calibration grid that are weaker because 
of too much background noise (so the "source minus background" signal is 
weaker than other areas of the grid).
*******************************************************************************/
float FishEyeGrid::intensityNormalizationFactor( float x, float y )
{
    int row = cellRow( y ),
        col = cellCol( x );
    float deltaX = calculateDeltaX( x, col ),
          deltaY = calculateDeltaY( y, row );
    return intensityNormalizationFactor( row, col, deltaX, deltaY );
}

float FishEyeGrid::intensityNormalizationFactor( int row, int col, float deltaX, float deltaY )
{
    float p0 = gridPoint( row, col ).intensityNormalization(),
          p1 = gridPoint( row, col + 1 ).intensityNormalization(),
          p2 = gridPoint( row + 1, col ).intensityNormalization(),
          p3 = gridPoint( row + 1, col + 1 ).intensityNormalization(),
          lowerEdge = p0 + deltaX * (p1 - p0),
          higherEdge = p2 + deltaX * (p3 - p2),
          solution = lowerEdge + deltaY * (higherEdge - lowerEdge);

    return solution;
}

/***************************************************************************//**
Prints grid to standard out for debugging purposes.
*******************************************************************************/
void FishEyeGrid::debugPrintGrid()
{
    std::cout << "\nFishEyeGrid::debugPrintGrid()" << "\n\n";

    for( int row = 0; row < numRows_; ++row ) {
        std::cout << "row = " << row << "\n";

        for( int col = 0; col < numCols_; ++col ) {
            blobNormalize::GridPoint p = gridPoint( row, col );
            std::cout << std::fixed << std::setprecision( 1 )
                      << col << ":  (" << p.x() << ", " << p.y() << "); "
                      << std::setprecision( 2 ) 
                      << "srcMinusBk = " << p.srcMinusBkIntensity() << "; "
                      << "src = " << p.sourceIntensity() << "; "
                      << "bk = " << p.backgroundIntensity() << "; "
                      << "norm = " << p.intensityNormalization() << "\n";
        }
    }
    std::cout << "\n";
}
