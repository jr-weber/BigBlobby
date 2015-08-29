////////////////////////////////////////////////////////////////////////////////
// BlobCalibration.cpp
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
#include "blobCalibration/BlobCalibration.h"
#include "blobNormalize/CalibrationGridPoints.h"
#include "blobNormalize/FishEyeGrid.h"
#include "blobCalibration/FastCalibration.h"
#include "blobExceptions/CalibrationException.h"
#include "blobTouchEvents/TouchEvents.h"
#include <iostream>

using blobCalibration::BlobCalibration;
using blobEnums::KeyEnum;
using blobExceptions::CalibrationException;

/** A successful calibration will be saved to this file. */
const std::string BlobCalibration::CALIBRATION_FILE = "data/settings/calibration/calibration.xml";

/***************************************************************************//**
After initializing data members and creating helper objects, this class will
add itself as a TouchEvents listener.
*******************************************************************************/
BlobCalibration::BlobCalibration() :
  bCalibrating_( false ),
  bW_( false ),
  bS_( false ),
  bA_( false ),
  bD_( false ),
  bShowTargets_( true ),
  monitorWidth_( 800 ),
  monitorHeight_( 600 ),
  arcAngle_( 0 ),
  targetColor_( 0xFF0000 ),
  verdana_(),
  calibrationText_(),
  calibrationParticle_(),
  tracker_( NULL ),
  calibrationUtils_(),
  calibrationXML_(),
  fastCalibration_( new blobCalibration::FastCalibration() ),
  calGridPoints_( NULL ),
  fishEyeGrid_( NULL ),
  doFastCalibration_( false ),
  fastCalMessage_( "" ),
  debug_( false )
{
    blobTouchEvents::TouchEvents::instance().addRAWListener( this );
}

BlobCalibration::~BlobCalibration()
{
    delete fishEyeGrid_;
    delete calGridPoints_;
    delete fastCalibration_;
}

/***************************************************************************//**
The setup function is run once to perform initializations in the application.

The name of the calibration XML file to read settings from is in the constant 
CALIBRATION_FILE.  The CalibrationUtils class will remember the name of the
XML file because it will be needed again for saving the results of a calibration
as soon as a calibration routine is completed successfully.
*******************************************************************************/
void BlobCalibration::setup( int videoWidth, int videoHeight, 
                             blobTracker::BlobTracker * trackerIn )
{
    std::string banner( 80, '*' );
    debug_.printLn( "\n" + banner );
    debug_.printLn( "BlobCalibration::setup() called..." );
    debug_.printLn( "    videoWidth = " + debug_.intToStr( videoWidth ) );
    debug_.printLn( "    videoHeight = " + debug_.intToStr( videoHeight ) );
    debug_.printLn( "    CALIBRATION_FILE = " + CALIBRATION_FILE );
    debug_.printLn( banner + "\n" );

    calibrationParticle_.loadImage( "images/particle.png" );
    calibrationParticle_.setUseTexture( true );
    verdana_.loadFont( "verdana.ttf", 7, true, true );
    calibrationText_.loadFont( "verdana.ttf", 14, true, true );
    calibrationUtils_.setCamRes( videoWidth, videoHeight );
    calibrationUtils_.setXmlFilename( CALIBRATION_FILE );
    calibrationUtils_.loadXMLSettings();
    updateCalGridPoints();
    tracker_ = trackerIn;
    tracker_->passInCalibration( &calibrationUtils_ );
    debug_.printLn( "\nCalibration is now setup!\n" );
}

/***************************************************************************//**
If the old calibration grid (camera points and grid points) was thrown out, but 
then the user did not follow through by finishing a new calibration, the grids 
will be left in an odd state.  If that situation is detected, the 
CalibrationUtils object will be called on  to load the last completed
calibration from the calibrations.xml file.
*******************************************************************************/
void BlobCalibration::checkForIncompleteCalibration()
{
    if( calibrationUtils_.isCalibrationIncomplete() ) {
        calibrationUtils_.loadXMLSettings();
    }
}

void BlobCalibration::updateCalGridPoints()
{
    delete calGridPoints_;
    delete fishEyeGrid_;
    calGridPoints_ = createCalibrationGridPoints();
    fishEyeGrid_ = calGridPoints_->createFishEyeGrid();
    //debugPrintGridPoints( calGridPoints_ );
}

/***************************************************************************//**
Called on by CoreVision when a calibration is started.
*******************************************************************************/
void BlobCalibration::setCalibrating( bool b )
{
    bCalibrating_ = b;
}

/***************************************************************************//**
Returns true if a calibration is in progress.
*******************************************************************************/
bool BlobCalibration::isCalibrating()
{
    return bCalibrating_;
}

/***************************************************************************//**
Returns the current size of the calibration grid held by the CalibrationUtils
object.
*******************************************************************************/
int BlobCalibration::numberOfCalibrationGridPoints()
{
    return calibrationUtils_.numberOfGridPoints();
}

/***************************************************************************//**
Starts the calibration routine by displaying the full screen showing the
calibration instructions.
*******************************************************************************/
void BlobCalibration::doCalibration( int screenWidth, int screenHeight )
{
    monitorWidth_ = screenWidth;
    monitorHeight_ = screenHeight;

    if( doFastCalibration_ ) {
        captureFastCalibrationPanel();
        doFastCalibration_ = false;
    }
    drawBlackScreen();
    drawCalibrationPoints();
    drawBoundingBox();

    // Draw blobs in calibration to see where you are touching.
    if( bShowTargets_ && !calibrationUtils_.isCalibrating() && !fastCalibration_->isCalibrating() ) {
        drawCalibrationBlobs();
    }
    drawCalibrationInstructions();
}

/***************************************************************************//**
Captures camera points and then transfers data to CalibrationUtils.
*******************************************************************************/
void BlobCalibration::captureFastCalibrationPanel()
{
    try {
        fastCalMessage_ = "";
        fastCalibration_->captureCalibrationPanel( tracker_, &calibrationUtils_ );
        int numPoints = fastCalibration_->copyCameraPointsToCalUtils( &calibrationUtils_ );
        fastCalMessage_ = fastCalibrationSuccessMsg( numPoints );
    }
    catch( CalibrationException e ) {
        fastCalMessage_ = e.getMessage() + "\n\n";
    }
}

std::string BlobCalibration::fastCalibrationSuccessMsg( int numPoints )
{
    std::string msg = 
        "FAST CALIBRATION: Processed " + intToStr( numPoints ) + " blobs.\n"
        "To check the calibration grid, press [x] to exit calibration mode, and then \n"
        "select the \"Show Calibration Wireframe\" option from the Blobs menu. \n\n";

    return msg;
}

/***************************************************************************//**
Draws black filled rectangle to fill the monitor area.
*******************************************************************************/
void BlobCalibration::drawBlackScreen()
{
    ofSetColor( 0x000000 );
    ofFill();
    ofRect( 0, 0, monitorWidth_, monitorHeight_ );
}

/***************************************************************************//**
Draws plus signs for calibration points and draws circles around active point 
if a calibration is in progress. 
*******************************************************************************/
void BlobCalibration::drawCalibrationPoints()
{
    // This all has to do with getting the angle for loading circle.
    arcAngle_ = 0;
    std::map<int, blobTracker::Blob> * trackedBlobs = tracker_->getTrackedBlobs();
    std::map<int, blobTracker::Blob>::iterator iter;

    for( iter = trackedBlobs->begin(); iter != trackedBlobs->end(); ++iter ) {
        if( iter->second.sitting > arcAngle_ ) {
            arcAngle_ = iter->second.sitting;
        }
    } // end loading circle angle
    //Get the screen points so we can make a grid
    vector2df * screenpts = calibrationUtils_.getScreenPoints();

    //For each grid point
    for( int i = 0; i < calibrationUtils_.gridPoints; i++ ) {
        // If calibrating, draw a red circle around the active point.
        if( calibrationUtils_.calibrationStep == i && calibrationUtils_.isCalibrating() ) {
            glPushMatrix();
            glTranslatef( screenpts[i].X * monitorWidth_, screenpts[i].Y * monitorHeight_, 0.0f );
            drawCircleAroundActivePoint();
            glPopMatrix();
        }
        // Make Plus Sign.
        ofSetColor( 0x226600 ); 
        ofRect( screenpts[i].X * monitorWidth_ - 2, screenpts[i].Y * monitorHeight_ - 18, 4, 36 ); //Horizontal Plus
        ofRect( screenpts[i].X * monitorWidth_ - 18, screenpts[i].Y * monitorHeight_ - 2, 36, 4 ); //Vertical Plus
    }
}

/***************************************************************************//**

*******************************************************************************/
void BlobCalibration::drawCircleAroundActivePoint()
{
    ofFill();
    ofSetColor( targetColor_ );
    ofCircle( 0.f, 0.f, 40 ); // Draws red target circle.
    ofSetColor( 0x00A4FF );
    drawCircleLoader( 0.0f, 0.0f, 40, 0, arcAngle_ * 360 ); // Draws loading circle.
    ofSetColor( 0x000000 );
    ofCircle( 0.f, 0.f, 25 ); // Draws black circle so it cuts out center.
}

/***************************************************************************//**

*******************************************************************************/
void BlobCalibration::drawCircleLoader( double xctr, double yctr, double radius, 
                                        double startAngle, double endAngle )
{
    double vectorX1, vectorY1;  // vector to a point on circle from its center
    double vectorX0, vectorY0;  // previous version of vectorX1,Y1;
    double angle, ang0, ang1;   // Angle in radians from circle start point.

    ang0 = startAngle * (3.14/180.0);	// convert degrees to radians
    ang1 = endAngle * (3.14/180.0);
    glBegin( GL_TRIANGLES );		// Tell OpenGL to draw a series of triangles
    // Start at the circle's rightmost point.
    vectorX1 = xctr + radius*cos( ang0 );
    vectorY1 = yctr + radius*sin( ang0 );

    // step through all other points on circle;
    for( angle=ang0 + 3.14/180.0; angle < ang1 + 3.14/180.0; angle += 3.14/180.0 )
    {								// (add to ang1 to ensure arcs can close)
        vectorX0 = vectorX1;		// save previous point's position,
        vectorY0 = vectorY1;
        vectorX1 = xctr + radius*cos( angle ); // find a new point on the circle,
        vectorY1 = yctr + radius*sin( angle );
        glVertex2d( xctr, yctr );		      // plot the points of a triangle (CCW order)
        glVertex2d( vectorX0,vectorY0 );	  // center-->old pt-->new pt.
        glVertex2d( vectorX1, vectorY1 );
    }
    glEnd();    // finished drawing triangles.
    glFlush();  // Finish any pending drawing commands

    vectorY1 = yctr;  // Set starting point
    vectorX1 = xctr;
}

/***************************************************************************//**
Draws large bounding box around calibration grid.
*******************************************************************************/
void BlobCalibration::drawBoundingBox()
{
    ofSetColor( 0xFFFFFF );
    ofNoFill();
    ofRect( calibrationUtils_.screenBB.upperLeftCorner.X * monitorWidth_, 
            calibrationUtils_.screenBB.upperLeftCorner.Y * monitorHeight_,
            calibrationUtils_.screenBB.getWidth() * monitorWidth_, 
            calibrationUtils_.screenBB.getHeight() * monitorHeight_ );
}

/*******************************************************************************
Draws cross-hairs to show blobs on the screen with the calibration instructions.
*******************************************************************************/
void BlobCalibration::drawCalibrationBlobs()
{
    std::map<int, blobTracker::Blob> * trackedBlobs = tracker_->getTrackedBlobs();
    std::map<int, blobTracker::Blob>::iterator iter;

    for( iter = trackedBlobs->begin(); iter != trackedBlobs->end(); ++iter ) {		
        blobTracker::Blob drawBlob;
        drawBlob = iter->second;

        //transform height/width to calibrated space
        drawBlob.boundingRect.width *= calibrationUtils_.screenBB.getWidth() * monitorWidth_ * 4;
        drawBlob.boundingRect.height *= calibrationUtils_.screenBB.getHeight() * monitorHeight_ * 4 ;

        //Draw Fuzzy Circles
        ofEnableAlphaBlending();
        ofImage tempCalibrationParticle;
        tempCalibrationParticle.clone( calibrationParticle_ );
        ofSetColor( 0x00CCFF ); // light blue fuzzy circles
        float fuzzyScale = 0.7f,
              fuzzyWidth = drawBlob.boundingRect.width * fuzzyScale,
              fuzzyHeight = drawBlob.boundingRect.height * fuzzyScale;
        tempCalibrationParticle.draw( drawBlob.centroid.x * monitorWidth_ - fuzzyWidth * 0.5f, 
                                      drawBlob.centroid.y * monitorHeight_ - fuzzyHeight * 0.5f,
                                      fuzzyWidth, 
                                      fuzzyHeight );
        ofDisableAlphaBlending();

        // Draw Blob Targets.
        ofSetColor( 0xFFFFFF ); // white cross-hairs
        glLineWidth( 5 );
        glPushMatrix();
        //	glLoadIdentity();
        glTranslatef( drawBlob.centroid.x * monitorWidth_,
                      ((drawBlob.centroid.y * monitorHeight_)),
                      0 );
        //  ofEllipse(0, 0, drawBlob.boundingRect.width/2, drawBlob.boundingRect.height/2);
        float lineScale = 0.15f,
              lineHalfWidth = drawBlob.boundingRect.width * lineScale,
              lineHalfHeight = drawBlob.boundingRect.height * lineScale;
        ofLine( -lineHalfWidth, 0,  lineHalfWidth, 0 );
        ofLine( 0, -lineHalfHeight, 0,  lineHalfHeight );
        glPopMatrix();
        //set line width back to normal
        glLineWidth(1);
    }
}

/***************************************************************************//**
Draws calibration instructions for the start-calibration screen (where bounding
box can be adjusted) or the continue-calibration screen (where red circles are
touched).
*******************************************************************************/
void BlobCalibration::drawCalibrationInstructions()
{
    if( calibrationUtils_.isCalibrating() ){
        drawRegularCalibrationInstructions();
    }
    else if( fastCalibration_->isCalibrating() ) {
        drawFastCalibrationInstructions();
    }
    else {
        drawStartCalibrationInstructions();
    }
}

/***************************************************************************//**
Writes full set of calibration instructions to the center of the screen.
*******************************************************************************/
void BlobCalibration::drawStartCalibrationInstructions()
{
    ofSetColor( 0xFF00FF );
    char reportStr[10240];
    calibrationText_.setLineHeight( 20.0f );
    int pointsPerRow = calibrationUtils_.pointsPerGridRow(),
        pointsPerCol = calibrationUtils_.pointsPerGridCol(),
        totalPoints = pointsPerRow * pointsPerCol;

    sprintf( reportStr, 

        "CALIBRATION\n\n"
        "-Press [C] to start regular Calibration routine (touching one point at a time).\n"
        "-Press [F] to start Fast calibration routine (using a calibration panel).\n"
        "-Press [X] to eXit calibration and return to the main screen.\n"
        "-Press [B] to recapture Background.\n"
        "-Press [T] to Toggle blob targets (cross-hairs).\n"
        "-Press [G] to open a Grid Properties box to adjust the size and alignment\n"
        "            of the calibration grid (or use keyboard shortcuts right below).\n\n"

        "GRID SIZE: %i points per row times %i rows = %i total points\n\n"
        "-Press [+]/[-] to increase/decrease the number of points per row.\n"
        "-Press [Shift][+]/[-] to increase/decrease the number of rows.\n\n"
        
        "ALIGNING BOUNDING BOX TO PROJECTION SCREEN:\n\n"
        "-Use arrow keys to move the white bounding box.\n"
        "-Press and hold [W], [A], [S], [D] (top, left, bottom, right) and\n"
        " arrow keys to adjust each side.",

        pointsPerRow, pointsPerCol, totalPoints );

    int x =  monitorWidth_ / 2 - calibrationText_.stringWidth( reportStr ) / 2,
        y =  monitorHeight_ / 2 - calibrationText_.stringHeight( reportStr ) / 2;
    if( x < 3 ) { x = 3; }
    if( y < 3 ) { y = 3; }
    calibrationText_.drawString( reportStr, x, y );
}

/***************************************************************************//**
Writes instructions for touching calibration circles.
*******************************************************************************/
void BlobCalibration::drawRegularCalibrationInstructions()
{
    ofSetColor( 0xFF00FF );
    char reportStr[10240];
    calibrationText_.setLineHeight( 20.0f );

    sprintf( reportStr, "\nCALIBRATING:\n\n"
        "-To calibrate, touch and hold current circle target until the circle turns white.\n"
        "-Press [B] to recapture Background (if there's false blobs).\n"
        "-Press [R] to Return to previous point(s).\n"
        "-Press [C] to go back to previous Calibration screen.\n"
        "-Press [X] to eXit calibration mode.\n" );

    int x =  monitorWidth_ / 2 - calibrationText_.stringWidth( reportStr ) / 2,
        y =  monitorHeight_ / 2 - calibrationText_.stringHeight( reportStr ) / 2;
    if( x < 3 ) { x = 3; }
    if( y < 3 ) { y = 3; }
    calibrationText_.drawString( reportStr, x, y );
}

/***************************************************************************//**
Writes instructions for fast calibration with calibration board.
*******************************************************************************/
void BlobCalibration::drawFastCalibrationInstructions()
{
    ofSetColor( 0xFF00FF );
    char reportStr[10240];
    calibrationText_.setLineHeight( 20.0f );

    sprintf( reportStr, 
        "%s"
        "\nFAST CALIBRATION REQUIREMENTS: \n\n"
        "You need a clear plexiglass panel with small circular stickers spaced in a grid\n"
        "pattern.  If your table came with a calibration panel, it would likely be stored\n"
        "inside the table door.  Otherwise, see directions further below for creating one.\n\n"

        "FAST CALIBRATION PROCEDURE: \n\n"
        "Carefully align the calibration panel's circular stickers with the plus signs on\n"
        "this grid. If the number of points do not match, press [F] to return to the\n"
        "previous calibration screen and change the points in a row or column. \n\n"

        "-Press [S] to automatically calibrate and Save. \n"
        "-Press [F] to go back to the screen before Fast calibration. \n"
        "-Press [X] to eXit calibration mode. \n\n"

        "CREATING A CALIBRATION PANEL: \n\n"
        "1. You will need a clear plexiglass sheet with a height and width at least an\n"
        "   inch or so greater than this grid area.  A fairly thin sheet (0.1 inches thick)\n"
        "   is fine.  Many home improvement stores sell such an item.\n"
        "2. You will need circular paper stickers about 3/4 of an inch in diameter.  They\n"
        "   must reflect infrared light, so use white or brightly colored stickers.  Many\n"
        "   office supply stores will have something suitable.\n"
        "3. Set the plexiglass sheet down on top of this grid.  You might want to tape\n"
        "   the corners down so that the plexiglass doesn't move while you are adding\n"
        "   the stickers.  Carefully center a sticker above each plus sign on the grid.",
        fastCalMessage_.c_str() );

    int x =  monitorWidth_ / 2 - calibrationText_.stringWidth( reportStr ) / 2,
        y =  monitorHeight_ / 2 - calibrationText_.stringHeight( reportStr ) / 2;
    if( x < 3 ) { x = 3; }
    if( y < 3 ) { y = 3; }
    calibrationText_.drawString( reportStr, x, y );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                 Touch Events
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Used to detect if a finger is removed from a calibration point.
*******************************************************************************/
void BlobCalibration::RAWTouchUp( blobTracker::Blob blob )
{
    //If Calibrating, register the calibration point on blobOff
    if( calibrationUtils_.isCalibrating() ) {

        if( calibrationUtils_.goToNextStep() ) {
            calibrationUtils_.nextCalibrationStep();
            calibrationUtils_.setGoToNextStep( false );
            targetColor_ = 0xFF0000;
        }
    }
}

/***************************************************************************//**
Used to detect if a finger is being held on a calibration point.
*******************************************************************************/
void BlobCalibration::RAWTouchHeld( blobTracker::Blob blob ) 
{
    //If Calibrating, register the calibration point on blobOff
    if( calibrationUtils_.isCalibrating() ) {
        vector2df * cameraPoints = calibrationUtils_.getCameraPoints();
        float * sourceIntensities = calibrationUtils_.cameraSourceIntensities();
        float * backgroundIntensities = calibrationUtils_.cameraBackgroundIntensities();
        int step = calibrationUtils_.calibrationStep;

        cameraPoints[step] =  vector2df( blob.centroid.x, blob.centroid.y );
        sourceIntensities[step] = blob.sourceIntensity;
        backgroundIntensities[step] = blob.backgroundIntensity;

        calibrationUtils_.setGoToNextStep( true );
        targetColor_ = 0xFFFFFF;
    }
}

/***************************************************************************//**
Not currently used.
*******************************************************************************/
void BlobCalibration::RAWTouchMoved( blobTracker::Blob blob )
{
}

/***************************************************************************//**
Not currently used.
*******************************************************************************/
void BlobCalibration::RAWTouchDown( blobTracker::Blob blob )
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                   Key events
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Responds to the arrow keys or alphabetic keys that are pressed in response to 
the instructions on the first screen of the calibration process.
*******************************************************************************/
void BlobCalibration::keyPressed( KeyEnum::Enum key )
{
    debugPrintKeyPressed( key );

    if( bCalibrating_ ) {
        switch( key ) {
            case KeyEnum::KEY_c:  enterOrExitRegularCalibrationMode();  break;
            case KeyEnum::KEY_f:  enterOrExitFastCalibrationMode();     break;
            case KeyEnum::KEY_r:  revertCalibrationStep();              break;
            case KeyEnum::KEY_t:  toggleShowTargets();                  break;

            case KeyEnum::KEY_a:
            case KeyEnum::KEY_d:
            case KeyEnum::KEY_w:
            case KeyEnum::KEY_s:  processKeyASDW( key );  break;
            
            case KeyEnum::KEY_ARROW_RIGHT:
            case KeyEnum::KEY_ARROW_LEFT:
            case KeyEnum::KEY_ARROW_DOWN:
            case KeyEnum::KEY_ARROW_UP:  processArrowKey( key );  break;
            
            case KeyEnum::KEY_EQUAL_SIGN:  calibrationUtils_.incrementGridCellsX();  break;
            case KeyEnum::KEY_MINUS:       calibrationUtils_.decrementGridCellsX();  break;
            case KeyEnum::KEY_PLUS:        calibrationUtils_.incrementGridCellsY();  break;
            case KeyEnum::KEY_UNDERSCORE:  calibrationUtils_.decrementGridCellsY();  break;
        }
    }
}

void BlobCalibration::setNumPointsPerRow( int numPoints )
{
    // The number of cells is one less than the number of points.
    calibrationUtils_.setGridCellsX( numPoints - 1 );
}

void BlobCalibration::setNumRows( int rows )
{
    // The number of cells is one less than the number of rows.
    calibrationUtils_.setGridCellsY( rows - 1 );
}

int BlobCalibration::numPointsPerRow()
{
    return calibrationUtils_.pointsPerGridRow();
}

int BlobCalibration::numRows()
{
    return calibrationUtils_.pointsPerGridCol();
}

CalibrationUtils * BlobCalibration::calibrationUtils()
{
    return &calibrationUtils_;
}

/***************************************************************************//**
Responds to the release of keys or that are pressed in response to the 
instructions on the first screen of the calibration process.
*******************************************************************************/
void BlobCalibration::keyReleased( KeyEnum::Enum key )
{
    debugPrintKeyReleased( key );

    if( bCalibrating_ ) {
        switch( key ) {
            case KeyEnum::KEY_w:  bW_ = false;  break;
            case KeyEnum::KEY_s:  bS_ = false;  break;
            case KeyEnum::KEY_a:  bA_ = false;  break;
            case KeyEnum::KEY_d:  bD_ = false;  break;

            case KeyEnum::KEY_x:
                calibrationUtils_.setCalibrating( false );
                fastCalibration_->stopCalibration();
                break;
        }
    }
}

void BlobCalibration::enterOrExitRegularCalibrationMode()
{
    if( !fastCalibration_->isCalibrating() ) {
        if( calibrationUtils_.isCalibrating() ) {
            calibrationUtils_.setCalibrating( false );
            debug_.printLn( "Calibration stopped." );
        } 
        else {
            calibrationUtils_.beginCalibration();
            debug_.printLn( "Calibration started." );
        }
    }
}

void BlobCalibration::enterOrExitFastCalibrationMode()
{
    if( !calibrationUtils_.isCalibrating() ) {
        if( fastCalibration_->isCalibrating() ) {
            fastCalibration_->stopCalibration();
        }
        else {
            fastCalibration_->startCalibration();
        }
    }
}

void BlobCalibration::revertCalibrationStep()
{
    if( calibrationUtils_.isCalibrating() ) {
        calibrationUtils_.revertCalibrationStep();
    }
}

void BlobCalibration::toggleShowTargets()
{
    bShowTargets_ = !bShowTargets_;
}

/***************************************************************************//**
If the initial calibration screen is open, the a, d, w, or s key is pressed, 
then an arrow key can be used to move just one edge of the bounding box.  If,
instead, the Fast Calibration screen is shown, the current array of blobs will
be captured for processing by the FastCalibration object.
*******************************************************************************/
void BlobCalibration::processKeyASDW( KeyEnum::Enum key )
{
    if( !calibrationUtils_.isCalibrating() && !fastCalibration_->isCalibrating() ) {
        switch( key ) {
            case KeyEnum::KEY_a:  bA_ = true;  break;  //Left
            case KeyEnum::KEY_d:  bD_ = true;  break;  //Right
            case KeyEnum::KEY_w:  bW_ = true;  break;  //Right
            case KeyEnum::KEY_s:  bS_ = true;  break;  //Right
        }
    }
    else if( fastCalibration_->isCalibrating() && key == KeyEnum::KEY_s ) {
        doFastCalibration_ = true;
    }
}

/***************************************************************************//**
If the initial calibration screen is open, the arrow keys can be used to 
move the bounding box or, in combination with the a, s, d, or w key, move 
just one edge of the bounding box.
*******************************************************************************/
void BlobCalibration::processArrowKey( KeyEnum::Enum key )
{
    if( !calibrationUtils_.isCalibrating() && !fastCalibration_->isCalibrating() ) {
        switch( key ) {
            case KeyEnum::KEY_ARROW_RIGHT:  moveBoundingBoxRight();  break;
            case KeyEnum::KEY_ARROW_LEFT:   moveBoundingBoxLeft();   break;
            case KeyEnum::KEY_ARROW_DOWN:   moveBoundingBoxDown();   break;
            case KeyEnum::KEY_ARROW_UP:     moveBoundingBoxUp();     break;
        }
    }
}

void BlobCalibration::moveBoundingBoxRight()
{
    if(      bA_ ) { calibrationUtils_.moveBoundingBoxLeftEdgeToTheRight();  }
    else if( bD_ ) { calibrationUtils_.moveBoundingBoxRightEdgeToTheRight(); }
    else           { calibrationUtils_.moveBoundingBoxToTheRight();          }
}

void BlobCalibration::moveBoundingBoxLeft()
{
    if(      bA_ ) { calibrationUtils_.moveBoundingBoxLeftEdgeToTheLeft();  }
    else if( bD_ ) { calibrationUtils_.moveBoundingBoxRightEdgeToTheLeft(); } 
    else           { calibrationUtils_.moveBoundingBoxToTheLeft();          }
}

void BlobCalibration::moveBoundingBoxDown()
{
    if(      bW_ ) { calibrationUtils_.moveBoundingBoxUpperEdgeDownward(); }
    else if( bS_ ) { calibrationUtils_.moveBoundingBoxLowerEdgeDownward(); }
    else           { calibrationUtils_.moveBoundingBoxDownward();          }
}

void BlobCalibration::moveBoundingBoxUp()
{
    if(      bW_ ) { calibrationUtils_.moveBoundingBoxUpperEdgeUpward(); }
    else if( bS_ ) { calibrationUtils_.moveBoundingBoxLowerEdgeUpward(); }
    else           { calibrationUtils_.moveBoundingBoxUpward(); }
}

/***************************************************************************//**
Draws the current calibration grid as a wireframe.  Note that if the 
calibration.xml file read in at startup was made with a 320 x 240 camera 
resolution, but the DefaultBlobParams.xml file is now changed to 640 x 480, the 
calibration is no longer valid (it will occupy only the top left one-fourth of 
the screen).  When changing camera size, the calibration must be redone.
*******************************************************************************/
void BlobCalibration::drawCalibrationWireframe( float videoWidth, float videoHeight )
{
    //debugPrintCameraDimensions( videoWidth, videoHeight );
    glLineWidth( 2.0f );
    glColor3f( 0.0f, 1.0f, 0.0f );
    glPolygonMode( GL_FRONT, GL_LINE );
	glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    drawWireframeQuadStrips();
    
    glColor3f( 1.0f, 1.0f, 1.0f );
    glPolygonMode( GL_FRONT, GL_FILL );
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );
}

/***************************************************************************//**
Draws the calibration grid as quad strips.
*******************************************************************************/
void BlobCalibration::drawWireframeQuadStrips()
{
    if( calGridPoints_ != NULL ) {
        //debugPrintGridPoints( calGridPoints_ );
        int lastRowIndex = calGridPoints_->numRows() - 1,
            numCols = calGridPoints_->numCols();

        for( int row = 0; row < lastRowIndex; ++row ) {
            int rowPlusOne = row + 1;
            glBegin( GL_QUAD_STRIP );
            
            for( int col = 0; col < numCols; ++col ) {
                blobNormalize::GridPoint p0 = calGridPoints_->gridPoint( row, col ),
                                           p1 = calGridPoints_->gridPoint( rowPlusOne, col );
                glVertex2f( p0.x(), p0.y() );
                glVertex2f( p1.x(), p1.y() );
            }
            glEnd();
        }
    }
}

void BlobCalibration::drawCalibrationPointsWithIntensities( float videoWidth, float videoHeight )
{
    if( calGridPoints_ != NULL ) {
        int numRows = calGridPoints_->numRows(),
            numCols = calGridPoints_->numCols();

        glColor3f( 1.0f, 0.0f, 1.0f );
        glPointSize( 6.0f );
        glEnable( GL_POINT_SMOOTH );
        glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
        glEnable( GL_BLEND );
	    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        for( int row = 0; row < numRows; ++row ) {
            for( int col = 0; col < numCols; ++col ) {
                blobNormalize::GridPoint p = calGridPoints_->gridPoint( row, col );
                
                glBegin(GL_POINTS);
                    glVertex2f( p.x(), p.y() );
                glEnd();
                char intensityStr[100];
                sprintf( intensityStr, "%i", (int)p.srcMinusBkIntensity() );
                verdana_.drawString( intensityStr, (int)p.x(), (int)p.y() - 4 );
            }
        }
        ofSetColor( 0xFFFFFF );
        glDisable( GL_BLEND );
    }
}

blobNormalize::CalibrationGridPoints * BlobCalibration::copyOfGridPoints()
{
    blobNormalize::CalibrationGridPoints * gridPoints = NULL;

    if( calGridPoints_ != NULL ) {
        gridPoints = new blobNormalize::CalibrationGridPoints( calGridPoints_ );
    }
    return gridPoints;
}

blobNormalize::FishEyeGrid * BlobCalibration::copyOfFishEyeGrid()
{
    blobNormalize::FishEyeGrid * fishEyeGrid = NULL;

    if( fishEyeGrid_ != NULL ) {
        fishEyeGrid = new blobNormalize::FishEyeGrid( fishEyeGrid_ );
    }
    return fishEyeGrid;
}

void BlobCalibration::updateCalGridPointsBackground( blobBuffers::GrayBuffer * backgroundBuffer )
{
    if( backgroundBuffer != NULL && calGridPoints_ != NULL ) {
        calGridPoints_->updateBackgroundIntensities( backgroundBuffer );
        calGridPoints_->calculateSrcMinusBkIntensities();
        calGridPoints_->calculateIntensityNormalizations();

        delete fishEyeGrid_;
        fishEyeGrid_ = new blobNormalize::FishEyeGrid( calGridPoints_ );
    }
}

blobNormalize::CalibrationGridPoints * BlobCalibration::createCalibrationGridPoints()
{
    vector2df * cameraPoints = calibrationUtils_.getCameraPoints();
    float * sourceIntensities = calibrationUtils_.cameraSourceIntensities();
    float * backgroundIntensities = calibrationUtils_.cameraBackgroundIntensities();
    int pointsPerRow = calibrationUtils_.pointsPerGridRow(),
        pointsPerCol = calibrationUtils_.pointsPerGridCol(),
        numPoints = calibrationUtils_.numberOfGridPoints(),
        index = 0;

    blobNormalize::CalibrationGridPoints * calGridPoints = NULL;
    calGridPoints = new blobNormalize::CalibrationGridPoints( pointsPerCol, pointsPerRow  );
    calGridPoints->setCameraWidthAndHeight( calibrationUtils_.cameraWidth(),
                                            calibrationUtils_.cameraHeight() );

    for( int row = 0; row < pointsPerCol; ++row ) {
        for( int col = 0; col < pointsPerRow; ++ col ) {
            if( index < numPoints ) {
                float x = cameraPoints[index].X,
                      y = cameraPoints[index].Y,
                      sourceIntensity = sourceIntensities[index],
                      backgroundIntensity = backgroundIntensities[index];
                calGridPoints->setPointXY( x, y, row, col );
                calGridPoints->setSourceIntensity( sourceIntensity, row, col );
                calGridPoints->setBackgroundIntensity( backgroundIntensity, row, col );
            }
            ++index;
        }
    }
    calGridPoints->calculateSrcMinusBkIntensities();
    calGridPoints->calculateIntensityNormalizations();
    return calGridPoints;
}

std::string BlobCalibration::intToStr( int n )
{
    std::stringstream out;
    out << n;
    return out.str();
}

void BlobCalibration::debugPrintKeyPressed( blobEnums::KeyEnum::Enum key )
{
    std::string msg( "BlobCalibration::keyPressed(" );
    debug_.printLn( msg + KeyEnum::convertToString( key ) + ") called..." );
}

void BlobCalibration::debugPrintKeyReleased( blobEnums::KeyEnum::Enum key )
{
    std::string msg( "BlobCalibration::keyReleased(" );
    debug_.printLn( msg + KeyEnum::convertToString( key ) + ") called..." );
}

void BlobCalibration::debugPrintCameraDimensions( float videoWidth, 
                                                  float videoHeight )
{
    static int count = -1;
    ++count;

    if( (count % 600) == 0 ) { // Print on every 600th frame.
        std::cout << "\nvideoWidth = " << videoWidth << "\n"
                  << "videoHeight = " << videoHeight << "\n";
    }
}

void BlobCalibration::debugPrintGridPoints( blobNormalize::CalibrationGridPoints * gridPoints )
{
    static int count = -1;
    ++count;

    if( (count % 600) == 0 ) { // Print on every 600th frame.
        int camWidth = gridPoints->camWidth(),
            camHeight = gridPoints->camHeight(),
            numRows = gridPoints->numRows(),
            numCols = gridPoints->numCols(),
            numPoints = gridPoints->numPoints();

        std::cout << "\ncamWidth = " << camWidth << "\n"
                  << "camHeight = " << camHeight << "\n"
                  << "numRows = " << numRows << "\n"
                  << "numCols = " << numCols << "\n"
                  << "numPoints = " << numPoints << "\n"
                  << "gridPoints:\n\n";

        for( int row = 0; row < numRows; ++row ) {
            for( int col = 0; col < numCols; ++col ) {
                blobNormalize::GridPoint p = gridPoints->gridPoint( row, col );
                std::cout << "(x, y, srcMinusBk, src, bk) = (" 
                          << p.x() << ", " 
                          << p.y() << ", "
                          << p.srcMinusBkIntensity() << ", "
                          << p.sourceIntensity() << ", "
                          << p.backgroundIntensity() << ")\n";
            }
            std::cout << "\n";
        }
    }
}
