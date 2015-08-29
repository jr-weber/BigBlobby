/* CalibrationUtils.h
 *  
 *  Created on 2/2/09.
 *  Copyright 2009 NUI Group. All rights reserved.
 *
 *  This class is slightly modified from the original CalibrationUtils class of 
 *  CCV version 1.3 (files CalibrationUtils.h and Calibration.cpp).
 *      -J.R.Weber <joe.weber77@gmail.com>
 */
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
#ifndef CALIBRATIONUTILS_H
#define CALIBRATIONUTILS_H

//LOAD CONFIG.XML
#include "ofxXmlSettings.h"
//Used other calibration
#include "blobCalibration/rect2d.h"
#include "blobCalibration/vector2d.h"

class CalibrationUtils
{
	public:
        static const float GRID_BORDER_FACTOR;

		CalibrationUtils();	//constructor
		~CalibrationUtils();//destructor

		void setCamRes(int camWidth, int camHeight);
        void setXmlFilename(const std::string & filename);
        std::string getXmlFilename();
		void loadXMLSettings();// Load Settings
		void saveCalibration();// Save Settings
		//Calibration Methods
		void setScreenScale(float s);
		void setScreenBBox(rect2df & bbox);
		void setGrid(int x, int y);
        int pointsPerGridRow();
        int pointsPerGridCol();
        int numberOfGridPoints();
        int cameraWidth();
        int cameraHeight();
		void initTriangles();
		void cameraToScreenPosition(float &x, float &y);
		void cameraToScreenSpace(float &x, float &y);
		void transformDimension(float &width, float &height);
		void initScreenPoints();
		void initCameraPoints(int camWidth, int camHeight);
		void calculateBox(); //calculates min/max calibration rect

		vector2df * getScreenPoints();
		vector2df * getCameraPoints();
        float * cameraSourceIntensities();
        float * cameraBackgroundIntensities();

        void setCalibrating( bool b );
        bool isCalibrating();
        void setGoToNextStep( bool b );
        bool goToNextStep();
	    void beginCalibration();
		void nextCalibrationStep();
        void finishLastCalibrationStep();
		void revertCalibrationStep();
        bool isCalibrationIncomplete();

        void incrementGridCellsX();
        void decrementGridCellsX();
        void incrementGridCellsY();
        void decrementGridCellsY();
        void setGridCellsX( int x );
        void setGridCellsY( int y );

        void moveBoundingBoxToTheRight();
        void moveBoundingBoxLeftEdgeToTheRight();
        void moveBoundingBoxRightEdgeToTheRight();
        void moveBoundingBoxToTheLeft();
        void moveBoundingBoxLeftEdgeToTheLeft();
        void moveBoundingBoxRightEdgeToTheLeft();
        void moveBoundingBoxDownward();
        void moveBoundingBoxUpperEdgeDownward();
        void moveBoundingBoxLowerEdgeDownward();
        void moveBoundingBoxLowerEdgeUpward();
        void moveBoundingBoxUpperEdgeUpward();
        void moveBoundingBoxUpward();

		rect2df getScreenBBox();
		bool isPointInTriangle(vector2df p, vector2df a, vector2df b, vector2df c);

		// returns -1 if none found..
		int findTriangleWithin(vector2df pt);		
		float getScreenScale();

        void debugPrintCameraPoints();

		rect2df screenBB;
		int  calibrationStep;
		int  gridPoints;
		int  gridCellsX;
		int  gridCellsY;

	private:
        void expandOuterEdgeOfCalibrationGrid();
        void expandFirstAndLastRow();
        void expandFirstAndLastCol();

		bool isCalibrating_;
		bool goToNextStep_;
        vector2df * screenPoints_;
		vector2df * cameraPoints_;
        float * cameraSourceIntensity_;
        float * cameraBackgroundIntensity_;
		int * triangles_;
		int gridIndices_;
		int	camWidth_;
		int camHeight_;
		bool bscreenPoints_;
		bool bcameraPoints_;
		ofxXmlSettings calibrationXML_;
        std::string xmlFilename_;
        float maxBoxX_,  //calibration rectangle min/max
		      minBoxX_,
		      maxBoxY_,
		      minBoxY_;
        bool isCalibrationIncomplete_;
};

#endif
