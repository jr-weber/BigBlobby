/* CalibrationUtils.cpp
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
#include "blobCalibration/CalibrationUtils.h"
#include <cmath>

const float CalibrationUtils::GRID_BORDER_FACTOR = 0.01f;

CalibrationUtils::CalibrationUtils() :
  screenBB(),
  goToNextStep_( false ),
  calibrationStep( 0 ),
  gridPoints( 0 ),
  gridCellsX( 0 ),
  gridCellsY( 0 ),
  isCalibrating_( false ),
  screenPoints_( NULL ),
  cameraPoints_( NULL ),
  cameraSourceIntensity_( NULL ),
  cameraBackgroundIntensity_( NULL ),
  triangles_( NULL ),
  gridIndices_( 0 ),
  camWidth_( 320 ),
  camHeight_( 240 ),
  bscreenPoints_( false ),
  bcameraPoints_( false ),
  xmlFilename_( "calibration.xml" ),
  maxBoxX_( 0 ),
  minBoxX_( 0 ),
  maxBoxY_( 0 ),
  minBoxY_( 0 ),
  isCalibrationIncomplete_( false )
{
}

CalibrationUtils::~CalibrationUtils()
{
    delete screenPoints_;
	delete cameraPoints_;
    delete cameraSourceIntensity_;
    delete cameraBackgroundIntensity_;
    delete triangles_;
}

void CalibrationUtils::setCamRes(int camWidth = 320, int camHeight = 240)
{
    //std::cout << "\n\nCalibrationUtils::setCamRes("
    //          << camWidth << ", " << camHeight << ")\n\n\n";
	camWidth_ = camWidth;
	camHeight_ = camHeight;
}

void CalibrationUtils::setXmlFilename(const std::string & filename)
{
    xmlFilename_ = filename;
}

std::string CalibrationUtils::getXmlFilename()
{
    return xmlFilename_;
}

//--------------------------------------------------------------
//	Load Settings from the config.xml file
//--------------------------------------------------------------
void CalibrationUtils::loadXMLSettings()
{
	goToNextStep_ = false;

    if( calibrationXML_.loadFile( xmlFilename_ ) ) {
        //std::cout << "Calibration was loaded from " << xmlFilename_ << "\n";
    } 
    else { // FAIL!
        std::cout << "The calibration file could not be found: " << xmlFilename_ << "\n";
        // GENERATE DEFAULT XML DATA WHICH WILL BE SAVED INTO THE CONFIG
    }
	bool bboxRoot = true;
	bool screenRoot = true;
	isCalibrating_ = false;
	calibrationStep = 0;

	//Set grid and init everything that relates to the grid
	gridCellsX = calibrationXML_.getValue( "SCREEN:GRIDMESH:GRIDX", 50 );
	gridCellsY = calibrationXML_.getValue( "SCREEN:GRIDMESH:GRIDY", 50 );
	setGrid( gridCellsX, gridCellsY );

	//Bounding Box Points
	if( bboxRoot ) {
	    vector2df ul( calibrationXML_.getValue( "SCREEN:BOUNDINGBOX:ulx", 0.000000 ),
                      calibrationXML_.getValue( "SCREEN:BOUNDINGBOX:uly", 0.000000 ) );
	    vector2df lr( calibrationXML_.getValue( "SCREEN:BOUNDINGBOX:lrx", 1.000000 ),
                      calibrationXML_.getValue( "SCREEN:BOUNDINGBOX:lry", 1.000000 ) );
		rect2df boundingbox( ul, lr );
		setScreenBBox( boundingbox );
	}
    else {
		setScreenScale( 1.0f );
    }
	//Calibration Points
	if( screenRoot ) {
		//lets see how many <STROKE> </STROKE> tags there are in the xml file
		int numDragTags = calibrationXML_.getNumTags( "SCREEN:POINT" );

		//if there is at least one <POINT> tag we can read the list of points
		if( numDragTags > 0 ) {
			//we push into the last POINT tag this temporarily treats the tag as the document root.
			calibrationXML_.pushTag( "SCREEN:POINT", numDragTags - 1 );

			//we see how many points we have stored in <POINT> tags
			int numPtTags = calibrationXML_.getNumTags( "POINT" );

			if( numPtTags > 0 ) {
				//We then read those x y values into our array
				for( int i = 0; i < numPtTags; i++ ) {
					//the last argument of getValue can be used to specify
					//which tag out of multiple tags you are refering to.
					float x = calibrationXML_.getValue( "POINT:X", 0.000000, i );
					float y = calibrationXML_.getValue( "POINT:Y", 0.000000, i );
					cameraPoints_[i] = vector2df( x, y );
                    cameraSourceIntensity_[i] = calibrationXML_.getValue( "POINT:SOURCE_INTENSITY", 0.0, i );
                    cameraBackgroundIntensity_[i] = calibrationXML_.getValue( "POINT:BACKGROUND_INTENSITY", 0.0, i );
					bscreenPoints_ = true;
					bcameraPoints_ = true;
				}
			}
			calibrationXML_.popTag(); //Set XML root back to highest level
		}
	}
	//Set the camera calibrated box.
	calculateBox();
    isCalibrationIncomplete_ = false;
    //debugPrintCameraPoints();
}

/*****************************************************************************
 * Start of Calibration Methods
 *****************************************************************************/
//Bounding Box Size
void CalibrationUtils::setScreenBBox(rect2df &box)
{
	screenBB = box;
	initScreenPoints();
}

void CalibrationUtils::setGrid( int x, int y )
{
    isCalibrationIncomplete_ = true;
	gridCellsX = x;
	gridCellsY = y;
	gridPoints = (gridCellsX + 1) * (gridCellsY + 1);
    gridIndices_ = gridCellsX * gridCellsY * 3 * 2;

    delete screenPoints_;
    delete cameraPoints_;
    delete cameraSourceIntensity_;
    delete cameraBackgroundIntensity_;
    delete triangles_;

	screenPoints_ = new vector2df[gridPoints];
	cameraPoints_ = new vector2df[gridPoints];
    cameraSourceIntensity_ = new float[gridPoints];
    cameraBackgroundIntensity_ = new float[gridPoints];
	triangles_ = new int[gridIndices_];
	initTriangles();

	if( bscreenPoints_ && bcameraPoints_ ) {
		initScreenPoints();
		initCameraPoints( camWidth_, camHeight_ );
	}
}

int CalibrationUtils::pointsPerGridRow()
{
    return gridCellsX + 1;
}

int CalibrationUtils::pointsPerGridCol()
{
    return gridCellsY + 1;
}

int CalibrationUtils::numberOfGridPoints()
{
    return gridPoints;
}

int CalibrationUtils::cameraWidth()
{
    return camWidth_;
}

int CalibrationUtils::cameraHeight()
{
    return camHeight_;
}

void CalibrationUtils::initTriangles()
{
	int t = 0;
	for(int j = 0; j < gridCellsY; j++)
	{
		for(int i = 0; i < gridCellsX; i++)
		{
			triangles_[t+0] = (i+0) + ((j+0) * (gridCellsX+1));
			triangles_[t+1] = (i+1) + ((j+0) * (gridCellsX+1));
			triangles_[t+2] = (i+0) + ((j+1) * (gridCellsX+1));

			t += 3;

			triangles_[t+0] = (i+1) + ((j+0) * (gridCellsX+1));
			triangles_[t+1] = (i+1) + ((j+1) * (gridCellsX+1));
			triangles_[t+2] = (i+0) + ((j+1) * (gridCellsX+1));

			t += 3;
		}
	}
}

//Initialize Points
void CalibrationUtils::initScreenPoints()
{
	int p = 0;

	vector2df xd(screenBB.lowerRightCorner.X-screenBB.upperLeftCorner.X,0.0f);
	vector2df yd(0.0f, screenBB.lowerRightCorner.Y-screenBB.upperLeftCorner.Y);

	xd /= (float) gridCellsX;
	yd /= (float) gridCellsY;

	for(int j = 0; j <= gridCellsY; j++)
	{
		for(int i = 0; i <= gridCellsX; i++)
		{
			screenPoints_[p] = screenBB.upperLeftCorner + xd*i + yd*j;
			//printf("(%d, %d) = (%f, %f)\n", i, j, screenPoints_[p].X, screenPoints_[p].Y);
			p++;
		}
	}
}

void CalibrationUtils::initCameraPoints(int camWidth, int camHeight)
{
	int p = 0;

	for(int j = 0; j <= gridCellsY; ++j ) {
		for(int i = 0; i <= gridCellsX; ++i ) {
			cameraPoints_[p] = vector2df((i * camWidth) / (float)gridCellsX, (j * camHeight) / (float)gridCellsY);
            cameraSourceIntensity_[p] = 0.0f;
            cameraBackgroundIntensity_[p] = 0.0f;
			p++;
		}
	}
}

void CalibrationUtils::setScreenScale(float s)
{
	// legacy
	float offset = (1.0f - s) * 0.5f;
	screenBB = rect2df(vector2df(offset,offset),vector2df(1.0f-offset,1.0f-offset));
	initScreenPoints();
}

float CalibrationUtils::getScreenScale()
{
	// legacy, take the minimum scale value that fits inside the bounding box
	float minValL = MIN(screenBB.lowerRightCorner.X,screenBB.lowerRightCorner.Y);
	minValL = 1.0f - minValL;
	float minValU = MAX(screenBB.upperLeftCorner.X,screenBB.upperLeftCorner.Y);
	float minVal = MIN(minValL,minValU);
	return 1.0f - (2.0f * minVal);
}

void CalibrationUtils::cameraToScreenPosition(float &x, float &y)
{
	cameraToScreenSpace(x, y);
}

void CalibrationUtils::transformDimension(float &width, float &height)
{
	//Transform width/height
    float halfX = width * 0.5f;
    float halfY = height * 0.5f;

	//Take all blobs as if they're from the center of calibrated region
	float centerX = ((maxBoxX_ - minBoxX_)/2) + minBoxX_;
	float centerY = ((maxBoxY_ - minBoxY_)/2) + minBoxY_;

	//Calculate x/y position of upper left and lower right x/y positions
    float ulX = centerX - halfX;
    float ulY = centerY - halfY;
    float lrX = centerX + halfX;
    float lrY = centerY + halfY;

	//Transform these x/y positions to screen space
	cameraToScreenPosition(ulX, ulY);
	cameraToScreenPosition(lrX, lrY);

	//Calculate new height/width
	width = std::fabs(lrX - ulX);
	height = std::fabs(ulY - lrY);
}

void CalibrationUtils::calculateBox()
{
	//reset variables
	maxBoxX_ = 0;
	minBoxX_ = camWidth_;
	maxBoxY_ = 0;
	minBoxY_ = camHeight_;

	// Calculate the max/min points based on cameraPoints_
	for( int i = 0; i < gridPoints; ++i ) {
		if( cameraPoints_[i].X > maxBoxX_ )			maxBoxX_ = cameraPoints_[i].X;
		else if( cameraPoints_[i].X < minBoxX_ )	minBoxX_ = cameraPoints_[i].X;
		if( cameraPoints_[i].Y > maxBoxY_ )			maxBoxY_ = cameraPoints_[i].Y;
		else if( cameraPoints_[i].Y < minBoxY_ )	minBoxY_ = cameraPoints_[i].Y;
	}
}

vector2df * CalibrationUtils::getScreenPoints() 
{ 
    return screenPoints_; 
}

vector2df * CalibrationUtils::getCameraPoints() 
{ 
    return cameraPoints_; 
}

float * CalibrationUtils::cameraSourceIntensities()
{
    return cameraSourceIntensity_;
}

float * CalibrationUtils::cameraBackgroundIntensities()
{
    return cameraBackgroundIntensity_;
}

// Transforms a camera space coordinate into a screen space coord
void CalibrationUtils::cameraToScreenSpace(float &x, float &y)
{
	vector2df pt(x, y);
	int t = findTriangleWithin(pt);
	if(t != -1)
	{
		vector2df A = cameraPoints_[triangles_[t+0]];
		vector2df B = cameraPoints_[triangles_[t+1]];
		vector2df C = cameraPoints_[triangles_[t+2]];
		float total_area = (A.X - B.X) * (A.Y - C.Y) - (A.Y - B.Y) * (A.X - C.X);

		// pt,B,C
		float area_A = (pt.X - B.X) * (pt.Y - C.Y) - (pt.Y - B.Y) * (pt.X - C.X);

		// A,pt,C
		float area_B = (A.X - pt.X) * (A.Y - C.Y) - (A.Y - pt.Y) * (A.X - C.X);

		float bary_A = area_A / total_area;
		float bary_B = area_B / total_area;
		float bary_C = 1.0f - bary_A - bary_B;	// bary_A + bary_B + bary_C = 1

		vector2df sA = screenPoints_[triangles_[t+0]];
		vector2df sB = screenPoints_[triangles_[t+1]];
		vector2df sC = screenPoints_[triangles_[t+2]];

		vector2df transformedPos;

		transformedPos = (sA*bary_A) + (sB*bary_B) + (sC*bary_C);

		x = transformedPos.X;
		y = transformedPos.Y;
		return;
	}
	x = 0;
	y = 0;
	// FIXME: what to do in the case that it's outside the mesh?
}

bool CalibrationUtils::isPointInTriangle(vector2df p, vector2df a, vector2df b, vector2df c)
{
	if (vector2df::isOnSameSide(p,a, b,c) && vector2df::isOnSameSide(p,b, a,c) && vector2df::isOnSameSide(p, c, a, b))
		return true;
    else
		return false;
}

int CalibrationUtils::findTriangleWithin(vector2df pt)
{
	for(int t = 0; t < gridIndices_; t += 3)
	{
		if(isPointInTriangle(pt, cameraPoints_[triangles_[t]], cameraPoints_[triangles_[t+1]], cameraPoints_[triangles_[t+2]]) )
			return t;
	}
	return -1;
}

void CalibrationUtils::setCalibrating( bool b )
{
    isCalibrating_ = b;
}

bool CalibrationUtils::isCalibrating()
{
    return isCalibrating_;
}

void CalibrationUtils::setGoToNextStep( bool b )
{
    goToNextStep_ = b;
}

bool CalibrationUtils::goToNextStep()
{
    return goToNextStep_;
}

void CalibrationUtils::beginCalibration()
{
	 isCalibrating_ = true;
	 calibrationStep = 0;
}

void CalibrationUtils::nextCalibrationStep()
{
	if( isCalibrating_ ) {
		calibrationStep++;

        if( calibrationStep == 1 ) {
            // Remember that a calibration has been started, but not finished.
            isCalibrationIncomplete_ = true;
        }

		if( calibrationStep >= gridPoints ) {
			finishLastCalibrationStep();
		}
	}
}

void CalibrationUtils::finishLastCalibrationStep()
{
    isCalibrating_ = false;
    calibrationStep = 0;
    expandOuterEdgeOfCalibrationGrid();
	calculateBox();
    saveCalibration();
    isCalibrationIncomplete_ = false;
}

bool CalibrationUtils::isCalibrationIncomplete()
{
    return isCalibrationIncomplete_;
}

void CalibrationUtils::revertCalibrationStep()
{
	if( isCalibrating_ ) {
		calibrationStep--;

		if( calibrationStep < 0 ) {
			calibrationStep = 0;
		}
	}
}

void CalibrationUtils::incrementGridCellsX()
{
    ++gridCellsX;

    if( gridCellsX > 13 ) {
        gridCellsX = 13; 
    }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::decrementGridCellsX()
{
    --gridCellsX;

    if( gridCellsX < 1 ) {
        gridCellsX = 1; 
    }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}


void CalibrationUtils::setGridCellsX( int x )
{
    gridCellsX = x;

    if( gridCellsX > 13 ) { 
        gridCellsX = 13; 
    }
    else if( gridCellsX < 1 ) {
        gridCellsX = 1;
    }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::incrementGridCellsY()
{
    ++gridCellsY;

    if( gridCellsY > 13 ) {
        gridCellsY = 13; 
    }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::decrementGridCellsY()
{
    --gridCellsY;

    if( gridCellsY < 1 ) {
        gridCellsY = 1; 
    }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}


void CalibrationUtils::setGridCellsY( int y )
{
    gridCellsY = y;

    if( gridCellsY > 13 ) { 
        gridCellsY = 13; 
    }
    else if( gridCellsY < 1 ) {
        gridCellsY = 1;
    }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxToTheRight()
{
    moveBoundingBoxLeftEdgeToTheRight();
    moveBoundingBoxRightEdgeToTheRight();
}

void CalibrationUtils::moveBoundingBoxLeftEdgeToTheRight()
{
    screenBB.upperLeftCorner.X += .001;

    if( screenBB.upperLeftCorner.X > 1 )  { screenBB.upperLeftCorner.X = 1; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxRightEdgeToTheRight()
{
    screenBB.lowerRightCorner.X += .001;

    if( screenBB.lowerRightCorner.X > 1) { screenBB.lowerRightCorner.X = 1; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxToTheLeft()
{
    moveBoundingBoxLeftEdgeToTheLeft();
    moveBoundingBoxRightEdgeToTheLeft();
}

void CalibrationUtils::moveBoundingBoxLeftEdgeToTheLeft()
{
    screenBB.upperLeftCorner.X -= .001;

    if( screenBB.upperLeftCorner.X < 0 ) { screenBB.upperLeftCorner.X = 0; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxRightEdgeToTheLeft()
{
    screenBB.lowerRightCorner.X -= .001;

    if( screenBB.lowerRightCorner.X < 0 ) { screenBB.lowerRightCorner.X = 0; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxDownward()
{
    moveBoundingBoxUpperEdgeDownward();
    moveBoundingBoxLowerEdgeDownward();
}

void CalibrationUtils::moveBoundingBoxUpperEdgeDownward()
{
    screenBB.upperLeftCorner.Y += .001;

    if( screenBB.upperLeftCorner.Y > 1 ) { screenBB.upperLeftCorner.Y = 1; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxLowerEdgeDownward()
{
    screenBB.lowerRightCorner.Y += .001;

    if( screenBB.lowerRightCorner.Y > 1 ) { screenBB.lowerRightCorner.Y = 1; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxUpward()
{
    moveBoundingBoxUpperEdgeUpward();
    moveBoundingBoxLowerEdgeUpward();
}

void CalibrationUtils::moveBoundingBoxUpperEdgeUpward()
{
    screenBB.upperLeftCorner.Y -= .001;

    if( screenBB.upperLeftCorner.Y < 0 ) { screenBB.upperLeftCorner.Y = 0; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

void CalibrationUtils::moveBoundingBoxLowerEdgeUpward()
{
    screenBB.lowerRightCorner.Y -= .001;

    if( screenBB.lowerRightCorner.Y < 0 ) { screenBB.lowerRightCorner.Y = 0; }
    setGrid( gridCellsX, gridCellsY );
    calibrationStep = 0;
}

/***************************************************************************//**
Slightly expands the outside edges of the calibration grid by adding or 
subtracting a few pixels.  The number of pixels to expand by is calculated by 
taking the camera image height and multiplying by GRID_BORDER_FACTOR.  So if
quarter VGA is used (320 x 240 pixels) and GRID_BORDER_FACTOR is set to 0.01,
a value of 2.4 pixels is used (1% of the camera image height).  The reason for 
this minor adjustment is that I have always noticed that with multitouch apps
like Finger Paint or Fire Fingers, no matter how carefully I calibrate, the 
calibration grid always seems just a little too small.  This issue can be 
tested by slowly moving your fingertip from outside the projection area of 
your touch table towards the edge of the projection area and seeing if the 
touch is detected as soon as your fingertip crosses the edge.  Without the  
minor adjustment performed by this function, there was almost always a slight 
gap, but the gap goes away when this minor adjustment is performed at the 
very end of a calibration.
*******************************************************************************/
void CalibrationUtils::expandOuterEdgeOfCalibrationGrid()
{
    expandFirstAndLastRow();
    expandFirstAndLastCol();
}

/***************************************************************************//**
This helper function for expandOuterEdgeOfCalibrationGrid() subtracts a few 
pixels from the y-value of the first row of cameraPoints_ and adds a few pixels
to the y-value of the last row.
*******************************************************************************/
void CalibrationUtils::expandFirstAndLastRow()
{
    //printf( "\n(first row y, last row y)\n\n" );
    float pixelBorder = camHeight_ * GRID_BORDER_FACTOR;
    int rows = pointsPerGridCol(),
        cols = pointsPerGridRow(),
        firstRowIndex = 0,
        lastRowIndex = cols * (rows - 1);

    for( int col = 0; col < cols; ++col, ++firstRowIndex, ++lastRowIndex ) {
        float firstRowY = cameraPoints_[firstRowIndex].Y,
              lastRowY = cameraPoints_[lastRowIndex].Y;
        //printf( "Before: (%.6f, %.6f)\n", firstRowY, lastRowY );

        firstRowY -= pixelBorder;
        lastRowY += pixelBorder;
        if( firstRowY > 0 ) { cameraPoints_[firstRowIndex].Y = firstRowY; }
        if( lastRowY < camHeight_ ) { cameraPoints_[lastRowIndex].Y = lastRowY; }
        //printf( "After:  (%.6f, %.6f)\n\n", firstRowY, lastRowY );
    }
}

/***************************************************************************//**
This helper function for expandOuterEdgeOfCalibrationGrid() subtracts a few 
pixels from the x-value of the first column of cameraPoints_ and adds a few 
pixels to the y-value of the last column.
*******************************************************************************/
void CalibrationUtils::expandFirstAndLastCol()
{
    //printf( "\n(first col x, last col x)\n\n" );
    float pixelBorder = camHeight_ * GRID_BORDER_FACTOR;
    int rows = pointsPerGridCol(),
        cols = pointsPerGridRow(),
        firstColStartIndex = 0,
        lastColStartIndex = cols - 1;

    for( int row = 0; row < rows; ++row ) {
        int firstColIndex = firstColStartIndex + (row * cols),
            lastColIndex = lastColStartIndex + (row * cols);
        float firstColX = cameraPoints_[firstColIndex].X,
              lastColX = cameraPoints_[lastColIndex].X;
        //printf( "(%.6f, %.6f)\n", firstColX, lastColX );

        firstColX -= pixelBorder;
        lastColX += pixelBorder;
        if( firstColX > 0 ) { cameraPoints_[firstColIndex].X = firstColX; }
        if( lastColX < camWidth_ ) { cameraPoints_[lastColIndex].X = lastColX; }
        //printf( "After:  (%.6f, %.6f)\n\n", firstColX, lastColX );
    }
}

//Save Calibration Points to file when finished calibrating.
void CalibrationUtils::saveCalibration()
{
    //lets see how many <STROKE> </STROKE> tags there are in the xml file
	int numDragTags = calibrationXML_.getNumTags("SCREEN:POINT");

	//if there is at least one <POINT> tag we can read the list of points
	if(numDragTags > 0)
	{
		//we push into the last POINT tag this temporarirly treats the tag as the document root.
		calibrationXML_.pushTag("SCREEN:POINT", numDragTags-1);

		calibrationXML_.clear();

		//Save the Grid Mesh X/Y
		calibrationXML_.setValue("GRIDMESH:GRIDX", gridCellsX);
	    calibrationXML_.setValue("GRIDMESH:GRIDY", gridCellsY);

		//Save the Bounding Box
		calibrationXML_.setValue("BOUNDINGBOX:ulx", screenBB.upperLeftCorner.X);
		calibrationXML_.setValue("BOUNDINGBOX:uly", screenBB.upperLeftCorner.Y);
		calibrationXML_.setValue("BOUNDINGBOX:lrx", screenBB.lowerRightCorner.X);
		calibrationXML_.setValue("BOUNDINGBOX:lry", screenBB.lowerRightCorner.Y);

		//Save all the Calibration Points
		if(gridPoints > 0)
		{
			//We then read those x y values into our array
			for(int i = 0; i < gridPoints; i++)
			{
				//the last argument of getValue can be used to specify
				//which tag out of multiple tags you are referring to.
				calibrationXML_.setValue("POINT:X", cameraPoints_[i].X, i);
				calibrationXML_.setValue("POINT:Y", cameraPoints_[i].Y, i);
                calibrationXML_.setValue("POINT:SOURCE_INTENSITY", cameraSourceIntensity_[i], i);
                calibrationXML_.setValue("POINT:BACKGROUND_INTENSITY", cameraBackgroundIntensity_[i], i);
			}
		}
		calibrationXML_.popTag(); //Set XML root back to highest level
	}
	//calibrationXML_.saveFile("calibration.xml");
    calibrationXML_.saveFile( xmlFilename_ );
    //debugPrintCameraPoints();
}

rect2df CalibrationUtils::getScreenBBox() 
{ 
    return screenBB; 
}

void CalibrationUtils::debugPrintCameraPoints()
{
    int rows = pointsPerGridCol(),
        cols = pointsPerGridRow(),
        point = 0;

    printf( "\nCalibrationUtils::debugPrintCameraPoints():\n" );

    for( int row = 0; row < rows; ++row ) {
        printf( "row = %i\n", row );

        for( int col = 0; col < cols; ++col ) {
            if( point < gridPoints ) {
                printf( "%d: (x, y, src, bk) = (%.6f, %.6f, %.2f, %.2f, %.2f)\n", 
                        col, 
                        cameraPoints_[point].X, 
                        cameraPoints_[point].Y, 
                        cameraSourceIntensity_[point],
                        cameraBackgroundIntensity_[point] );
                ++point;
            }
        }
        printf( "\n" );
    }
    printf( "\n" );
}
