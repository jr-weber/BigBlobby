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
#ifndef BLOBNORMALIZE_CALIBRATIONGRIDPOINTS_H
#define BLOBNORMALIZE_CALIBRATIONGRIDPOINTS_H

#include "blobNormalize/GridPoint.h"
#include <vector>

namespace blobNormalize { class FishEyeGrid; }
namespace blobBuffers { class GrayBuffer; }

namespace blobNormalize 
{
    /***********************************************************************//**
    Holds information on the calibration grid used to match camera 
    space to screen space.  This class was written to make it easy 
    to pass info from the BlobCalibration class to the ImageFilters 
    class so that the BackgroundFilter can create a mask to eliminate 
    areas outside of the calibration grid.
    ***************************************************************************/
    class CalibrationGridPoints
    {
    public:
        static const int DEFAULT_CAMERA_WIDTH,
                         DEFAULT_CAMERA_HEIGHT;
        static const float MAX_CORRECTION_FACTOR;
        static const int NUM_POINTS_FOR_AVG_MAX_INTENSITY;
        static const float MIN_SRC_MINUS_BK_NORMALIZATION,
                           MAX_SRC_MINUS_BK_NORMALIZATION;

        CalibrationGridPoints( int rows, int cols );
        CalibrationGridPoints( CalibrationGridPoints * other );
        ~CalibrationGridPoints();

        int numRows();
        int numCols();
        int numPoints();
        void setPointXY( float x, float y, int row, int col );
        void setSourceIntensity( float intensity, int row, int col );
        void setBackgroundIntensity( float intensity, int row, int col );
        blobNormalize::GridPoint gridPoint( int row, int col );

        void set( CalibrationGridPoints * other );
        void incrementX( float deltaX, int row, int col );
        void incrementY( float deltaY, int row, int col );

        void setCameraWidthAndHeight( int width, int height );
        int camWidth();
        int camHeight();

        void calculateSrcMinusBkIntensities();
        void calculateIntensityNormalizations();
        void updateBackgroundIntensities( blobBuffers::GrayBuffer * backgroundBuffer );

        blobNormalize::FishEyeGrid * createFishEyeGrid();

    private:
        bool isOdd( int n );
        float calculateAverageMaxIntensity();
        void debugPrintCellIntensities();

        int numRows_,
            numCols_;
        std::vector<blobNormalize::GridPoint> gridPoints_;
        float avgMaxIntensity_;
        int camWidth_,
            camHeight_;
    };
}

#endif
