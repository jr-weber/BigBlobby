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
#ifndef BLOBNORMALIZE_FISHEYEGRID_H
#define BLOBNORMALIZE_FISHEYEGRID_H

#include "blobNormalize/GridPoint.h"
#include <vector>

namespace blobNormalize { class CalibrationGridPoints; }

namespace blobNormalize 
{
    /***********************************************************************//**
    
    ***************************************************************************/
    class FishEyeGrid
    {
    public:
        FishEyeGrid( blobNormalize::CalibrationGridPoints * calGridPoints );
        FishEyeGrid( blobNormalize::FishEyeGrid * other );
        ~FishEyeGrid();

        int numRows();
        int numCols();
        int numPoints();
        void setPointSrcMinusBkIntensity( float intensity, int row, int col );
        void setPointSourceIntensity( float intensity, int row, int col );
        void setPointBackgroundIntensity( float intensity, int row, int col );
        void setIntensityNormalization( float intensity, int row, int col );
        blobNormalize::GridPoint gridPoint( int row, int col );
        float intensityNormalizationFactor( float x, float y );
        void debugPrintGrid();

    private:
        void calculateGridStepsX( blobNormalize::CalibrationGridPoints * calGridPoints );
        void calculateGridStepsY( blobNormalize::CalibrationGridPoints * calGridPoints );
        void calculateGridXY();
        void copyIntensityInfo( blobNormalize::CalibrationGridPoints * calGridPoints );
        void fixBorderGridPoints();
        void fixRowZero();
        void fixLastRow();
        void fixFirstColumn();
        void fixLastColumn();
        int cellRow( float y );
        int cellCol( float x );
        float calculateDeltaX( float x, int col );
        float calculateDeltaY( float x, int row );
        float intensityNormalizationFactor( int row, int col, float deltaX, float deltaY );

        std::vector<blobNormalize::GridPoint> gridPoints_;
        std::vector<float> gridStepsX_,
                           gridStepsY_;
        int numRows_,
            numCols_,
            camWidth_,
            camHeight_;
    };
}

#endif