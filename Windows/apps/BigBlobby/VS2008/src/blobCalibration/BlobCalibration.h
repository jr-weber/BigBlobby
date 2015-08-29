////////////////////////////////////////////////////////////////////////////////
// BlobCalibration.h
//
// PURPOSE: Uses CCV code to calibrate the xy-coordinate system used by a 
//          multi-touch surface.
//
// CREATED: 5/26/2011
// 
// This class is modified from the original Calibration class of CCV version 1.3
// (files Calibration.h and Calibration.cpp).  In particular, a FastCalibration
// helper class has been added for allowing the use of a clear plexiglass board
// with circular stickers to be used for calibration, instead of one finger at
// a time.  A CalibrationGridPoints and a FishEyeGrid class have also been 
// added for allowing an intensity normalization to be done by the
// blobFilters::SignalNormalizationFilter class.
//     -J.R.Weber <joe.weber77@gmail.com>
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2009 NUI Group/Inc. All rights reserved.
Web: http://nuigroup.com
  
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are 
met:
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
#ifndef BLOBCALIBRATION_BLOBCALIBRATION_H
#define BLOBCALIBRATION_BLOBCALIBRATION_H

#include "blobCalibration/CalibrationUtils.h"
#include "blobTracker/BlobTracker.h"
#include "blobTouchEvents/TouchListener.h"
#include "blobEnums/KeyEnum.h"
#include "blobIO/DebugPrint.h"
#include "ofMain.h"
#include "ofxXmlSettings.h" // LOAD CONFIG.XML

namespace blobNormalize { class CalibrationGridPoints; }
namespace blobNormalize { class FishEyeGrid; }
namespace blobCalibration { class FastCalibration; }
namespace blobBuffers { class GrayBuffer; }

namespace blobCalibration 
{
    /***********************************************************************//**
     Uses CCV code to calibrate the xy-coordinate system used by a multi-touch 
     surface.  The blobGui::CalibrationGLWidget class only provides a drawing
     surface.  The BlobCalibration class and its primary helper class, the 
     CalibrationUtils class of CCV, do the actual calibration and drawing using
     OpenGL.  Although this class is mainly CCV code, it knows about the 
     blobEnums::KeyEnum class that is used for passing info on key events that
     ultimately are coming from Qt GUI classes.
     <br />

     BlobCalibration is a modified version of the CCV Calibration class found
     in files Calibration.h and Calibration.cpp.  
     ***************************************************************************/
    class BlobCalibration : public blobTouchEvents::TouchListener
    {
    public:
        static const std::string CALIBRATION_FILE;

        BlobCalibration();
        ~BlobCalibration();
        void setup( int videoWidth, int videoHeight, blobTracker::BlobTracker * trackerIn );
        void setCalibrating( bool b );
        bool isCalibrating();
        int numberOfCalibrationGridPoints();
        void doCalibration( int screenWidth, int screenHeight ); 
        void drawCalibrationWireframe( float videoWidth, float videoHeight );
        void drawCalibrationPointsWithIntensities( float videoWidth, float videoHeight );
        void checkForIncompleteCalibration();
        void updateCalGridPoints();
        blobNormalize::CalibrationGridPoints * copyOfGridPoints();
        blobNormalize::FishEyeGrid * copyOfFishEyeGrid();
        void updateCalGridPointsBackground( blobBuffers::GrayBuffer * backgroundBuffer );

        CalibrationUtils * calibrationUtils();
        int numPointsPerRow();
        int numRows();
        void setNumPointsPerRow( int numPoints );
        void setNumRows( int rows );

        // Touch Events
        virtual void RAWTouchDown( blobTracker::Blob blob );
        virtual void RAWTouchMoved( blobTracker::Blob blob );
        virtual void RAWTouchUp( blobTracker::Blob blob );
        virtual void RAWTouchHeld( blobTracker::Blob blob );
            
        // Key Events
        void keyPressed( blobEnums::KeyEnum::Enum key );
        void keyReleased( blobEnums::KeyEnum::Enum key );

    private:
        void drawBlackScreen();
        void drawCalibrationPoints();
        void drawCircleAroundActivePoint();
        void drawCircleLoader( double xctr, double yctr, double radius, 
                               double startAngle, double endAngle );
        void drawBoundingBox();
        void drawCalibrationBlobs();
        void drawCalibrationInstructions();
        void drawStartCalibrationInstructions();
        void drawRegularCalibrationInstructions();
        void drawFastCalibrationInstructions();

        void captureFastCalibrationPanel();
        std::string fastCalibrationSuccessMsg( int numPoints );

        void enterOrExitRegularCalibrationMode();
        void enterOrExitFastCalibrationMode();
        void revertCalibrationStep();
        void toggleShowTargets();
        void processKeyASDW( blobEnums::KeyEnum::Enum key );
        void processArrowKey( blobEnums::KeyEnum::Enum key );
        void moveBoundingBoxRight();
        void moveBoundingBoxLeft();
        void moveBoundingBoxDown();
        void moveBoundingBoxUp();

        void drawWireframeQuadStrips();
        blobNormalize::CalibrationGridPoints * createCalibrationGridPoints();

        std::string intToStr( int n );
        void debugPrintKeyPressed( blobEnums::KeyEnum::Enum key );
        void debugPrintKeyReleased( blobEnums::KeyEnum::Enum key );
        void debugPrintCameraDimensions( float videoWidth, float videoHeight );
        void debugPrintGridPoints( blobNormalize::CalibrationGridPoints * gridPoints );

        bool bCalibrating_,
             bW_,
             bS_,
             bA_,
             bD_,
             bShowTargets_;
        int monitorWidth_,
            monitorHeight_;
        float arcAngle_,
              targetColor_;

        // Fonts
        ofTrueTypeFont verdana_;
        ofTrueTypeFont calibrationText_;

        // Draw Particle Image
        ofImage calibrationParticle_;

        // Blob Tracker
        blobTracker::BlobTracker * tracker_;
        CalibrationUtils calibrationUtils_;
        ofxXmlSettings calibrationXML_;
        blobCalibration::FastCalibration * fastCalibration_;
        blobNormalize::CalibrationGridPoints * calGridPoints_;
        blobNormalize::FishEyeGrid * fishEyeGrid_;
        bool doFastCalibration_;
        std::string fastCalMessage_;
        blobIO::DebugPrint debug_;
    };
}

#endif
