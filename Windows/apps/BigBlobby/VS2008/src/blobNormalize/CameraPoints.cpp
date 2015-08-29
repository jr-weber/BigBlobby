////////////////////////////////////////////////////////////////////////////////
// CameraPoints
//
// PURPOSE: Holds a vector of CameraPoint objects.
//
// CREATED: 8/23/2014
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
#include "blobNormalize/CameraPoints.h"
#include "blobExceptions/CalibrationException.h"
#include "blobTracker/BlobTracker.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using blobNormalize::CameraPoints;
using blobNormalize::CameraPoint;
using blobExceptions::CalibrationException;

struct SortByAscendingY {
    bool operator() ( CameraPoint p0,CameraPoint p1 ){ 
        return (p0.y() < p1.y());
    }
} sortByAscendingY;

struct SortByAscendingX {
    bool operator() ( CameraPoint p0,CameraPoint p1 ) { 
        return (p0.x() < p1.x());
    }
} sortByAscendingX;

CameraPoints::CameraPoints() :
  points_(),
  pointsPerRow_( 0 ),
  pointsPerCol_( 0 ),
  numGridPoints_( 0 )
{
}

CameraPoints::~CameraPoints()
{
}

void CameraPoints::clear()
{
    pointsPerRow_ = 0;
	pointsPerCol_ = 0;
	numGridPoints_ = 0;
    points_.clear();
}

void CameraPoints::setGrid( CalibrationUtils * calibrationUtils, 
                            blobTracker::BlobTracker * tracker )
{
    clear();

    int pointsPerRow = calibrationUtils->pointsPerGridRow(),
        pointsPerCol = calibrationUtils->pointsPerGridCol();

    if( pointsPerRow > 1 && pointsPerCol > 1 ) {
        pointsPerRow_ = pointsPerRow;
	    pointsPerCol_ = pointsPerCol;
	    numGridPoints_ = pointsPerRow_ * pointsPerCol_;
        points_.reserve( numGridPoints_ );
    }
    //debugPrintBlobs( tracker );
    addPoints( tracker ); // Can throw exception.
}

void CameraPoints::addPoints( blobTracker::BlobTracker * tracker )
{
    std::map<int, blobTracker::Blob> * trackedBlobs = tracker->getTrackedBlobs();
    std::map<int, blobTracker::Blob>::iterator iter;
    validateSize( trackedBlobs->size() ); // Can throw exception.

    for( iter = trackedBlobs->begin(); iter != trackedBlobs->end(); ++iter ) {
        points_.push_back( 
            blobNormalize::CameraPoint( iter->second.cameraCentroid.x, 
                                          iter->second.cameraCentroid.y, 
                                          iter->second.sourceIntensity,
                                          iter->second.backgroundIntensity ) );
    }
}

void CameraPoints::validateSize( int numBlobs )
{
    if( numBlobs != numGridPoints_ ) { 
        std::string msg = "ERROR: Unexpected number of blobs: " + intToStr( numBlobs );
        msg += " (expected " + intToStr( numGridPoints_ ) + ")";
        throw CalibrationException( "FastCalibration::processBlobs",  msg );
    }
}

std::string CameraPoints::intToStr( int n )
{
    std::stringstream out;
    out << n;
    return out.str();
}

void CameraPoints::sortIntoRowsAndColumns()
{
    sortPointsByAscendingY();
    sortEachRowByAscendingX();
}

void CameraPoints::sortPointsByAscendingY()
{
    std::sort( points_.begin(), points_.end(), sortByAscendingY );
}

void CameraPoints::sortEachRowByAscendingX()
{
    //std::cout << "\nCameraPoints::sortEachRowByAscendingX() called.\n\n";
    int rows = pointsPerCol_,
        cols = pointsPerRow_;

    for( int row = 0; row < rows; ++row ) {
        int rowStart = cols * row,
            rowEndPlusOne = rowStart + cols;

        //std::cout << "(rowStart, rowEndPlusOne) = (" << rowStart << ", " << rowEndPlusOne << ")\n";
        std::sort( points_.begin() + rowStart, 
                   points_.begin() + rowEndPlusOne, 
                   sortByAscendingX );
    }
}

std::vector<blobNormalize::CameraPoint> & CameraPoints::points()
{
    return points_;
}

// Not currently called, but might be useful in a later version.
CameraPoint CameraPoints::nearestPoint( CameraPoint & p )
{
    CameraPoint nearestP = p;
    float minDistance = 1000000.0f;
    int size = points_.size(),
        indexOfClosestPoint = -1;

    for( int i = 0; i < size; ++i ) {
        float d = distance( p, points_.at( i ) );

        if( d < minDistance )  {
            indexOfClosestPoint = i;
            minDistance = d;
        }
    }
    if( indexOfClosestPoint > -1 ) { 
        nearestP = points_.at( indexOfClosestPoint );
        points_.erase( points_.begin() + indexOfClosestPoint );
    }
    return nearestP;
}

float CameraPoints::distance( CameraPoint & p0, CameraPoint & p1 )
{
    float xDiff = p0.x() - p1.x(),
          yDiff = p0.y() - p1.y();

    return sqrt( xDiff * xDiff + yDiff * yDiff );
}

void CameraPoints::debugPrintPoints()
{
    std::cout << "\nFastCalibration::debugPrintPoints():\n";
    int size = points_.size();

    for( int i = 0; i < size; ++i ) {
        blobNormalize::CameraPoint & cameraPoint = points_.at( i );

        if( (i % pointsPerRow_) == 0 ) { std::cout << "\n"; }
        std::cout << i << " (x, y, srcMinusBk, src, bk ) = "
                  << std::fixed << std::setprecision( 1 ) << "(" 
                  << cameraPoint.x() << ", " 
                  << cameraPoint.y() << ", " 
                  << cameraPoint.sourceIntensity() << ", "
                  << cameraPoint.backgroundIntensity() << ")\n";
	}
    std::cout << "\n";
}

void CameraPoints::debugPrintBlobs( blobTracker::BlobTracker * tracker )
{
    std::map<int, blobTracker::Blob> * trackedBlobs = tracker->getTrackedBlobs();
    std::map<int, blobTracker::Blob>::iterator iter;
    int count = 0;
    std::cout << "\nUnsortedCameraPoints::debugPrintBlobs():\n\n";

    for( iter = trackedBlobs->begin(); iter != trackedBlobs->end(); ++iter, ++count ) {
        float x = iter->second.cameraCentroid.x,
              y = iter->second.cameraCentroid.y,
              src = iter->second.sourceIntensity,
              bk = iter->second.backgroundIntensity;

        std::cout << "blob " << count 
                  << " (x, y, srcMinusBk, src, bk) = (" 
                  << x << ", " 
                  << y << ", " 
                  << src << ", "
                  << bk << ")\n";
    }
    std::cout << "\n";
}
