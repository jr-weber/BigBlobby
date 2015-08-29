////////////////////////////////////////////////////////////////////////////////
// CoreVision.h - based on ofxNCoreVision class of CCV version 1.3
//
// PURPOSE: Manages the CCV (Community Core Vision) classes needed for detecting 
//          blobs resulting from fingertips moving on a multi-touch surface.
//
// CREATED: 5/19/2011
// 
// This class is based on the ofxNCoreVision class of CCV version 1.3, but has
// been substantially modified.
//    -J.R.Weber <joe.weber77@gmail.com>
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2009 NUI Group/Inc. All rights reserved.  (CCV 1.3 classes)
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
#include "blobCore/CoreVision.h"
#include "blobTuio/TuioChannels.h"
#include "blobFilters/ImageFilters.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobStats/Stats.h"
#include "blobUtils/FunctionTimer.h"
#include "blobUtils/StopWatch.h"
#include "blobVideo/SimpleVideoWriter.h"
#include "blobVideo/SimpleVideoReader.h"
#include <iostream>
#include <sstream>

using blobCore::CoreVision;
using blobEnums::KeyEnum;

const std::string CoreVision::VIDEO_FILE_NAME( "./data/test_videos/untitled.avi" );
const int CoreVision::VIDEO_WIDTH = 320,
          CoreVision::VIDEO_HEIGHT = 240,
          CoreVision::VIDEO_RATE = 30,
          CoreVision::MAX_NUMBER_BLOBS = 20,
          CoreVision::BACKGROUND_FRAMES_COUNT = 5;

/***************************************************************************//**
Creates instances of several helper classes: BlobFilters, ContourFinder, 
BlobFinder, BlobTracker, BlobCalibration, TUIO, ofxCvColorImage (for the
source image), and CPUImageFilter (for the processed image).

Creation of an ofTrueTypeFont object (for blob ID numbers) is deferred until
the OpenGL state is initialized. and creation of a camera or video player is
deferred until the update() function is called.
*******************************************************************************/
CoreVision::CoreVision() :
  // Camera
  videoGrabber_( NULL ),
  useCamera_( true ),
  startVideoCamera_( true ),
  deviceID_( 0 ),
  videoRate_( VIDEO_RATE ),
  videoWidth_( VIDEO_WIDTH ),
  videoHeight_( VIDEO_HEIGHT ),

  // VideoPlayer
  videoReader_( new blobVideo::SimpleVideoReader() ),
  videoFileName_( VIDEO_FILE_NAME ),
  startVideoPlayer_( false ),

  videoWriter_( new blobVideo::SimpleVideoWriter() ),
  recordSourceImages_( false ),

  // Blobs
  contourFinder_(),
  blobTracker_( new blobTracker::BlobTracker() ),
  minBlobSize_( 20 ),
  maxBlobSize_( 300 ),
  maxNumberBlobs_( MAX_NUMBER_BLOBS ),
  showBlobAreas_( false ),
  showBlobOutlines_( false ),
  showBlobCrosshairs_( false ),
  showBlobBoundingBoxes_( false ),
  showBlobLabels_( false ),
  showCalibrationWireframe_( false ),
  showCalibrationIntensities_( false ),
  verdana_(),

  // Filters, Network, and Calibration
  imageFilters_( new blobFilters::ImageFilters() ),
  backgroundLearnRate_( 0.01f ),
  processedImage_(),
  useGpuShaders_( false ),
  tuioChannels_( new blobTuio::TuioChannels() ),
  blobCalibration_(),
  isCalibrating_( false ),
  isFullscreen_( false ),
  updateNormalizationCountDown_( 0 ),

  // fps
  stats_( new blobStats::Stats() ),
  stopWatch_( new blobUtils::StopWatch() ),
  contourFinderTimer_( new blobUtils::FunctionTimer() ),
  blobTrackerTimer_( new blobUtils::FunctionTimer() ),
  processTimer_( new blobUtils::FunctionTimer() ),
  hasNewFrame_( false ),
  needToDisplayUpdatedFps_( false ),
  frames_( 0 ),
  fps_( 0.0 ),

  hasExited_( false ),
  debug_( false )
{
#ifdef TARGET_WIN32
    ffmvCamera_ = NULL;
    ps3Camera_ = NULL;
    dsvlCamera_ = NULL;
#endif
    stopWatch_->start();
    ofSeedRandom(); // CCV timers still used in TUIO classes and maybe elsewhere.
    ofResetElapsedTimeCounter();	   
}

CoreVision::~CoreVision()
{
    hasExited_ = true;
    delete processTimer_;
    delete blobTrackerTimer_;
    delete contourFinderTimer_;
    delete stopWatch_;
    delete stats_;
    delete tuioChannels_;
    delete imageFilters_;
    delete videoReader_;
#ifdef TARGET_WIN32
    delete ffmvCamera_;
    delete ps3Camera_;
    delete dsvlCamera_;
#endif
}

/***************************************************************************//**
This function should be called when the OpenGL state is first initialized so 
that it can load the verdana font used for showing blob ID numbers in the
final Tracked image of the GUI.
*******************************************************************************/
void CoreVision::loadFontForBlobIDNumbers()
{
    verdana_.loadFont( "data/verdana.ttf", 12, true, true );
}

/***************************************************************************//**
Checks if the video camera (or video player) has a new frame for processing.

If so, sets the bNewFrame data member to true before calling on the private
processVideoFrame() function to do the actual image processing, blob detection,
and sending out TUIO messages.  
<br />

This function also checks to see if a new video camera (or video player) has 
been requested.  If so, any older camera or player will be shut down before 
starting the new camera or player.
*******************************************************************************/
void CoreVision::update()
{
    hasNewFrame_ = false;
    if( hasExited_ ) { return; }

    if( useCamera_ ) {
        if( startVideoCamera_ ) {
            stopVideoPlayer();
            clearVideoFrame();
            initVideoCamera();
            setUpProcessedImage();
            learnBackground();
            imageFilters_->saveFramesAsBackground( BACKGROUND_FRAMES_COUNT );
            startVideoCamera_ = false;
        }
        #ifdef TARGET_WIN32
            if( ps3Camera_ != NULL ) { //ps3 camera 
                hasNewFrame_ = ps3Camera_->isFrameNew();
            }
            else if( ffmvCamera_ != NULL ) {
                ffmvCamera_->grabFrame();
                hasNewFrame_ = true;
            }
            else if(videoGrabber_ != NULL) {
                videoGrabber_->grabFrame();
                hasNewFrame_ = videoGrabber_->isFrameNew();
            }
            //else if( dsvlCamera_ != NULL ) {
            //    bNewFrame = dsvlCamera_->isFrameNew();
            //}
        #else
            videoGrabber_->grabFrame();
            hasNewFrame_ = videoGrabber_->isFrameNew();
        #endif
    }
    else { // Video player requested or in use.
        if( startVideoPlayer_  ) {
            clearVideoFrame();
            initVideoPlayer();
            learnBackground();
            imageFilters_->saveFramesAsBackground( BACKGROUND_FRAMES_COUNT );
            startVideoPlayer_ = false;
        }
        videoReader_->updateFrame();
        hasNewFrame_ = videoReader_->isFrameNew();
    }
    processVideoFrame();
}

/*******************************************************************************
Calls close() on the video player.  

Also sets the bNewFrame data member to true so that the image display widgets 
will draw one more time to clear the displayed images.
*******************************************************************************/
void CoreVision::stopVideoPlayer()
{
    debug_.printLn( "About to stop video player..." );
    videoReader_->close();
    hasNewFrame_ = true;
}

/*******************************************************************************
Calls on OpenGL to clear the drawing surface.
*******************************************************************************/
void CoreVision::clearVideoFrame()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
}

/*******************************************************************************
Looks for a PS3 (Play Station 3) camera first.  In the context of BigBlobby, 
only the PS3Eye camera has been tested, so it is not clear if the Fire Fly or
other cameras will really work.
*******************************************************************************/
void CoreVision::initVideoCamera()
{
    debug_.printLn( "initVideoCamera() called()..." );
    int requestWidth = videoWidth_,
        requestHeight = videoHeight_;

    // Check for firefly, PS3, or other camera.
    #ifdef TARGET_WIN32

        debug_.printLn( "TARGET_WIN32 is defined." );

        // ps3Camera - PS3 camera only.
        if( ps3Camera_ != NULL ) {
            videoWidth_ = ps3Camera_->getCamWidth();
            videoHeight_ = ps3Camera_->getCamHeight();
        }
        else if( ffmvCamera_ != NULL ) {
            videoWidth_ = ffmvCamera_->getCamWidth();
            videoHeight_ = ffmvCamera_->getCamHeight();
        }
        else if( videoGrabber_ != NULL ) {
            videoWidth_ = videoGrabber_->width;
            videoHeight_ = videoGrabber_->height;
        }
        else if( ofxPS3::getDeviceCount() > 0 && ps3Camera_ == NULL ) {
            debug_.printLn( "About to create ps3Camera..." );
            ps3Camera_ = new ofxPS3();
            debug_.printLn( "About to initialize ps3Camera..." );
            ps3Camera_->listDevices();
            ps3Camera_->initPS3( videoWidth_, videoHeight_, videoRate_ );
            videoWidth_ = ps3Camera_->getCamWidth();
            videoHeight_ = ps3Camera_->getCamHeight();
        }
        // ffmvCamera - firefly camera only.
        else if( ofxffmv::getDeviceCount() > 0 && ffmvCamera_ == NULL && ps3Camera_ == NULL && videoGrabber_ == NULL ) {
            debug_.printLn( "About to create ffmvCamera..." );
            ffmvCamera_ = new ofxffmv();
            debug_.printLn( "About to initialize ffmvCamera..." );
            ffmvCamera_->listDevices();
            ffmvCamera_->initFFMV( videoWidth_, videoHeight_ );
            videoWidth_ = ffmvCamera_->getCamWidth();
            videoHeight_ = ffmvCamera_->getCamHeight();
        }
        else if( videoGrabber_ == NULL && ps3Camera_ == NULL && ffmvCamera_ == NULL ) {
            debug_.printLn( "About to create videoGrabber..." );
            videoGrabber_ = new ofVideoGrabber();
            debug_.printLn( "About to initialize videoGrabber..." );
            videoGrabber_->listDevices();
            videoGrabber_->setVerbose(true);
            videoGrabber_->initGrabber( videoWidth_, videoHeight_ );
            videoWidth_ = videoGrabber_->width;
            videoHeight_ = videoGrabber_->height;
        }
        //else if( dsvlCamera_ == NULL ) {
        //    debug_.printLn( "About to create dsvlCamera..." );
        //    dsvlCamera_ = new ofxDSVL();
        //    debug_.printLn( "About to initialize dsvlCamera..." );
        //    dsvlCamera_->initDSVL();
        //    videoWidth = dsvlCamera_->getCamWidth();
        //    videoHeight_ = dsvlCamera_->getCamHeight();
        //}
    #else
        debug_.printLn( "TARGET_WIN32 is not defined." );

        if( videoGrabber_ == NULL ) {
            debug_.printLn( "About to create videoGrabber..." );
            videoGrabber_ = new ofVideoGrabber();
            debug_.printLn( "About to initialize..." );
            videoGrabber_->listDevices();
            videoGrabber_->setVerbose(true);
            videoGrabber_->initGrabber( videoWidth_, videoHeight_);
            videoWidth_ = videoGrabber_->width;
            videoHeight_ = videoGrabber_->height;
        }
        else {
            videoWidth_ = videoGrabber_->width;
            videoHeight_ = videoGrabber_->height;
        }
    #endif

    tuioChannels_->setCameraWidthAndHeight( videoWidth_, videoHeight_ );
    blobTracker_->setMaxBlobDistance( videoWidth_ );
    debugPrintCamera( requestWidth, requestHeight, videoWidth_, videoHeight_ );
}

/*******************************************************************************
Allocates memory for the source (ofxCvColorImage) and tracked (CPUImageFilter) 
image buffers.
*******************************************************************************/
void CoreVision::setUpProcessedImage()
{
    debug_.printLn( "CoreVision::setUpProcessedImage() called...\n" );
    processedImage_.clear();
    processedImage_.setUseTexture( true );
    processedImage_.allocate( videoWidth_, videoHeight_ );
}

/*******************************************************************************
Quick Time must be installed for this to work.
*******************************************************************************/
void CoreVision::initVideoPlayer()
{
    debug_.printLn( "About to start video player..." );
    videoReader_->init( videoFileName_ );
    videoWidth_ = videoReader_->width();
    videoHeight_ = videoReader_->height();

    tuioChannels_->setCameraWidthAndHeight( videoWidth_, videoHeight_ );
    blobTracker_->setMaxBlobDistance( videoWidth_ );
    debugPrintVideoWidthAndHeight( videoWidth_, videoHeight_ );
}

/*******************************************************************************
This is the most important helper function for update().  If a new frame is
available, this function will call on the CPU frame grabber function (GPU is
not currently used), and then process the image by apply filters and finding 
any contours before calling on the TUIO object to send out tracking info.  
Number of frames and time for calculating fps (frames per second) are also 
tracked here.
*******************************************************************************/
void CoreVision::processVideoFrame()
{
    if( hasNewFrame_ ) {
        processTimer_->startTiming();
        calculateFps();
        filterCameraImage();
        findAndTrackBlobs();
        updateAutoBackground();
        sendTuio();
        checkPeriodicBackground();
        writeVideoIfRequested();
        processTimer_->stopTiming();
    }
}

void CoreVision::writeVideoIfRequested()
{
    if( recordSourceImages_ ) {
        videoWriter_->processGrayscaleImage( imageFilters_->graySourceBuffer() );
    }
}

/*******************************************************************************
Keeps track of the frame count and the time needed for fps calculation.
*******************************************************************************/
void CoreVision::calculateFps()
{
    ++frames_;
    double elapsedTime = stopWatch_->elapsed();

    if( elapsedTime > 1.0 ) {
        fps_ = frames_ / elapsedTime;
        stopWatch_->start();
        frames_ = 0;
        needToDisplayUpdatedFps_ = true;
    }
}

/*******************************************************************************
Grabs the camera or video frame and sends it to the ImageFilters object.
*******************************************************************************/
void CoreVision::filterCameraImage()
{
    unsigned char * pixels = NULL;
    bool useGrayscale = true;

    if( useCamera_ ) {
        #ifdef TARGET_WIN32
            if( ps3Camera_ != NULL ) {
                pixels = ps3Camera_->getPixels();
            }
            else if( ffmvCamera_ != NULL ) {
                pixels = ffmvCamera_->fcImage[ ffmvCamera_->getDeviceID() ].pData;
            }
            else if( videoGrabber_ != NULL ) {
                pixels = videoGrabber_->getPixels();
                useGrayscale = false;
            }
            else if( dsvlCamera_ != NULL ) {
                pixels = dsvlCamera_->getPixels();
                useGrayscale = (dsvlCamera_->getNumByes() == 1);
            }
        #else
            if( videoGrabber_ != NULL ) {
                pixels = videoGrabber_->getPixels();
                useGrayscale = false;
            }
        #endif
    }
    else { // Must be using video player.
        pixels = videoReader_->getPixels();
        useGrayscale = false;
    }
    if( pixels != NULL ) {
        if( useGrayscale ) {
            imageFilters_->processGrayscaleImage( pixels, videoWidth_, videoHeight_ );
        }
        else {
            imageFilters_->processRgbImage( pixels, videoWidth_, videoHeight_ );
        }
        // If background has changed, need to update signal intensity 
        // normalization after several frames.  Updating immediately might not 
        // work well, as a first camera or reader frame may have to much noise.
        if( (updateNormalizationCountDown_ > 0) && !isCalibrating_ ) {
            --updateNormalizationCountDown_;

            if( updateNormalizationCountDown_ == 0 ) {
                blobCalibration_.updateCalGridPointsBackground( imageFilters_->backgroundBuffer() );
                imageFilters_->setSignalFishEyeGrid( blobCalibration_.copyOfFishEyeGrid() );
            }
        }
    }
}

/*******************************************************************************
Copies the GrayBuffer processed image to a CCV CPUImageFilter that is then
sent to the OpenCV findContour function.  The ContourFinder is then passed 
to the BlobTracker.
*******************************************************************************/
void CoreVision::findAndTrackBlobs()
{
    contourFinderTimer_->startTiming();
    blobBuffers::GrayBuffer * grayOutputBuffer = imageFilters_->grayOutputBuffer();
    unsigned char * pixels = grayOutputBuffer->pixels();
    processedImage_.setFromPixels( pixels, videoWidth_, videoHeight_);
    int maxNumBlobs = maxNumberBlobs_;
    blobBuffers::GrayBuffer * sourceBuffer = NULL,
                            * backgroundBuffer = NULL;

    if( isCalibrating_ ) { 
        maxNumBlobs = blobCalibration_.numberOfCalibrationGridPoints() + 5;
        sourceBuffer = imageFilters_->smoothedSourceBuffer();
        backgroundBuffer = imageFilters_->backgroundBuffer();
    }
    //debugPrintMaxNumBlobs( maxNumBlobs );
    contourFinder_.findContours( processedImage_,
                                 minBlobSize_, maxBlobSize_, maxNumBlobs,
                                 false, 
                                 frames_,
                                 sourceBuffer,
                                 backgroundBuffer );
    contourFinderTimer_->stopTiming();

    blobTrackerTimer_->startTiming();
    blobTracker_->track( contourFinder_.getBlobs() );
    blobTrackerTimer_->stopTiming();
}

/*******************************************************************************
Updates auto background learn rate (if needed).  The learning rate is slower 
if the ContourFinder has blobs.
*******************************************************************************/
void CoreVision::updateAutoBackground()
{
    if( imageFilters_->isAutoBackgroundActive() ) {
        if( !contourFinder_.isEmpty() ) { // Learn slow if blobs present.
            imageFilters_->setBackgroundLearnRate( backgroundLearnRate_ * 0.0001f );
        }
        else {
            imageFilters_->setBackgroundLearnRate( backgroundLearnRate_ * 0.001f );
        }
    }
}

/*******************************************************************************
If not in calibration mode, passes the tracked blobs to the TuioChannels object.
*******************************************************************************/
void CoreVision::sendTuio()
{
    if( !isCalibrating_ ) {
        tuioChannels_->sendTuio( blobTracker_->getTrackedBlobs() );
    }
}

/***************************************************************************//**
If time to update, calls on the ImageTimer to save the next frame as background.
*******************************************************************************/
void CoreVision::checkPeriodicBackground()
{
    if( !blobTracker_->hasBlobsInScreenArea() ) {
        imageFilters_->checkPeriodicBackground();
    }
}

/***************************************************************************//**
Use this function to check if the last call to update() found a new video camera
or video player frame to process.
*******************************************************************************/
bool CoreVision::hasNewFrame()
{
    return hasNewFrame_;
}

/*******************************************************************************
Calls on each Blob object to draw its contour (outline) and a set of cross-hairs
that run through its centroid.
*******************************************************************************/
void CoreVision::drawBlobOutlines()
{
    if( hasExited_ ) { return; }

    if( showBlobOutlines_ || showBlobBoundingBoxes_ || showBlobCrosshairs_ || showBlobAreas_ ) {
        std::vector<blobTracker::Blob> & blobs = contourFinder_.getBlobs();
        int length = blobs.size();

        for( int i = 0; i < length; ++i ) {
            blobTracker::Blob & blob = blobs[i];

            if( showBlobOutlines_ )      { blob.drawContourPoints(); }
            if( showBlobBoundingBoxes_ ) { blob.drawAngularBoundingBox(); }
            if( showBlobCrosshairs_ )    { blob.drawCrosshairs(); }

            if( showBlobAreas_ ) { 
                float x = blob.centroid.x + 12,
                      y = blob.centroid.y + 4;
                ofSetColor( 0, 0, 255 );
                char idStr[100];
                sprintf( idStr, "%i", blob.averageArea );
                verdana_.drawString( idStr, x, y );
            }
        }
        ofSetColor( 0xFFFFFF );
    }
}

/***************************************************************************//**
Calls on the BlobCalibration class to use OpenGL to render the calibration
points as a wireframe grid.
*******************************************************************************/
void CoreVision::drawCalibrationWireframe()
{
    if( hasExited_ ) { return; }

    if( showCalibrationWireframe_ ) {
        blobCalibration_.drawCalibrationWireframe( videoWidth_, videoHeight_ );
    }
}

/***************************************************************************//**
Calls on the BlobCalibration class to use OpenGL to render the calibration
points as dots with an associated signal intensity (source - background).
*******************************************************************************/
void CoreVision::drawCalibrationPointsWithIntensities()
{
    if( hasExited_ ) { return; }

    if( showCalibrationIntensities_ ) {
        blobCalibration_.drawCalibrationPointsWithIntensities( videoWidth_, videoHeight_ );
    }
}

/*******************************************************************************
Uses an ofTrueTypeFont object to draw the ID number associated with each
blob that has been detected.
*******************************************************************************/
void CoreVision::drawBlobIdNumbers()
{
    if( hasExited_ ) { return; }

    if( showBlobLabels_ ) {
        std::vector<blobTracker::Blob> & blobs = contourFinder_.getBlobs();
        int length = blobs.size();

        for( int i = 0; i < length; ++i ) {
            blobTracker::Blob & blob = blobs[i];
            float xpos = blob.centroid.x + 6,
                  ypos = blob.centroid.y + 4;

            ofSetColor( 255, 0, 0 );
            char idStr[100];
            sprintf( idStr, "%i", blob.id );
            verdana_.drawString( idStr, xpos, ypos );
        }
        ofSetColor( 0xFFFFFF );
    }
}

/***************************************************************************//**
Returns true if it is time to update the frames per second shown in the GUI.
*******************************************************************************/
bool CoreVision::needToDisplayUpdatedFps()
{
    if( needToDisplayUpdatedFps_ ) {
        needToDisplayUpdatedFps_ = false;
        return true;
    }
    return needToDisplayUpdatedFps_;
}

/***************************************************************************//**
Returns frames per second and other key info.
*******************************************************************************/
std::string CoreVision::getUpdatedFps()
{
    stats_->showProcessingTime( true );
    return stats_->fpsStats( fps_, videoWidth_, videoHeight_ );
}

/***************************************************************************//**
Returns network and filter timing info.
*******************************************************************************/
std::string CoreVision::getNetworkAndFiltersInfo()
{
    stats_->showProcessingTime( true );
    return stats_->networkAndFilterStats( tuioChannels_->getOutputSummary(),
                                          imageFilters_->calculateStats(),
                                          contourFinderTimer_->calculateAverageTime(),
                                          blobTrackerTimer_->calculateAverageTime(),
                                          tuioChannels_->calculateAverageTime(),
                                          processTimer_->calculateAverageTime(),
                                          videoWriter_->calculateAverageTime(),
                                          recordSourceImages_ );
}

blobCalibration::BlobCalibration * CoreVision::blobCalibrationObj()
{
    return &blobCalibration_;
}

/***************************************************************************//**
Sets up the BlobCalibration object by passing in the current video frame width
and height along with a reference to the BlobTracker object.
*******************************************************************************/
void CoreVision::setUpBlobCalibration()
{
    blobCalibration_.setup( videoWidth_, videoHeight_, blobTracker_ );
    imageFilters_->setMaskCalibrationGridPoints( blobCalibration_.copyOfGridPoints() );
    imageFilters_->setSignalFishEyeGrid( blobCalibration_.copyOfFishEyeGrid() );
}

/***************************************************************************//**
Sets up the BlobCalibration and BlobTracker objects to start the screen
calibration routine.
*******************************************************************************/
void CoreVision::startCalibrationRoutine()
{
    debug_.printLn( "CoreVision::startCalibrationRoutine() called..." );

    //Enter/Exit Calibration
    isCalibrating_ = true; // Stop sending TUIO messages on second channel.
    blobCalibration_.setCalibrating( true );
    blobTracker_->setCalibrating( true );
    isFullscreen_ = true;
}

void CoreVision::onCalibrationEnded()
{
    isCalibrating_ = false; // Send TUIO messages on second channel again.
    blobTracker_->setCalibrating( false );
    blobCalibration_.checkForIncompleteCalibration();
    blobCalibration_.updateCalGridPoints();
    imageFilters_->setMaskCalibrationGridPoints( blobCalibration_.copyOfGridPoints() );
    imageFilters_->setSignalFishEyeGrid( blobCalibration_.copyOfFishEyeGrid() );
}

/***************************************************************************//**
Uses OpenGL code to draw the calibration screen.
*******************************************************************************/
void CoreVision::drawCalibration( int screenWidth, int screenHeight )
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
        glTranslatef( 0.0f, screenHeight, 0.0f );
        glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );
        blobCalibration_.doCalibration( screenWidth, screenHeight );
    glPopMatrix();
}

/***************************************************************************//**
Clears the source image (ofxCvColorImage) and the processed image
(CPUImageFilter).
*******************************************************************************/
void CoreVision::clear()
{
    processedImage_.clear();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                       Video Camera Setters and Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
&quot;Device&quot; means the video camera.  If there is only one camera, it 
will have an ID of 0.  If there are multiple cameras, the IDs will be positive
integers that increment by 1.
*******************************************************************************/
void CoreVision::setDeviceID( int id )
{
    deviceID_ = id;
    debug_.printLn( "CoreVision::setDeviceID(" 
                  + debug_.intToStr( deviceID_ ) + ") called." );
}

/***************************************************************************//**
Returns the ID for the camera in use (zero or a positive integer).
*******************************************************************************/
int CoreVision::getDeviceID()
{
    return deviceID_;
}

/***************************************************************************//**
The requested video rate is usually 30 or 60 frames per second.  If the number
is much higher than the camera or blob detection code can handle, the actually
rate may bog down by quite a bit.
*******************************************************************************/
void CoreVision::setVideoRate( int rate )
{
    videoRate_ = rate;
    debug_.printLn( "CoreVision::setVideoRate(" + debug_.intToStr( videoRate_ ) + ") called." );
}

/***************************************************************************//**
Returns the video rate (frames per second) that had been requested.  The actual
frames per second may be slower.
*******************************************************************************/
int CoreVision::getVideoRate()
{
    return videoRate_;
}

/***************************************************************************//**
The video camera or video player width will usually be 320 or 640 pixels.

VGA is 640 x 480 pixels, while QVGA (Quarter VGA) is 320 x 240.
*******************************************************************************/
void CoreVision::setVideoWidth( int width )
{
    videoWidth_ = width;
    debug_.printLn( "CoreVision::setVideoWidth(" + debug_.intToStr( videoWidth_ ) + ") called." );
}

/***************************************************************************//**
Returns the frame width in pixels.
*******************************************************************************/
int CoreVision::getVideoWidth()
{
    return videoWidth_;
}

/***************************************************************************//**
The video camera or video player height will usually be 240 or 480 pixels.

VGA is 640 x 480 pixels, while QVGA (Quarter VGA) is 320 x 240.
*******************************************************************************/
void CoreVision::setVideoHeight( int height )
{
    videoHeight_ = height;
    debug_.printLn( "CoreVision::setVideoHeight(" + debug_.intToStr( videoHeight_ ) + ") called." );
}

/***************************************************************************//**
Returns the frame height in pixels.
*******************************************************************************/
int CoreVision::getVideoHeight()
{
    return videoHeight_;
}

/***************************************************************************//**
The image may need to be flipped horizontally depending on how the camera below
the multi-touch surface is set up.
*******************************************************************************/
void CoreVision::flipImageHorizontal( bool b )
{
    imageFilters_->flipImageHorizontal( b );
    debug_.printLn( "CoreVision::flipImageHorizontal(" + debug_.boolToStr( b ) + ") called." );
}

/***************************************************************************//**
Returns whether the BlobFilters class is flipping the image horizontally.
*******************************************************************************/
bool CoreVision::flipImageHorizontal()
{
    return imageFilters_->flipImageHorizontal();
}

/***************************************************************************//**
The image may need to be flipped vertically depending on how the camera below
the multi-touch surface is set up.
*******************************************************************************/
void CoreVision::flipImageVertical( bool b )
{
    imageFilters_->flipImageVertical( b );
    debug_.printLn( "CoreVision::flipImageVertical(" + debug_.boolToStr( b ) + ") called." );
}

/***************************************************************************//**
Returns whether the BlobFilters class is flipping the image vertically.
*******************************************************************************/
bool CoreVision::flipImageVertical()
{
    return imageFilters_->flipImageVertical();
}

/***************************************************************************//**
Opens a dialog box for camera settings.  This dialog box is typically provided
by camera-specific code (like the Play Station 3), not by the Qt GUI.  

At some future time, a Qt dialog box would be a good improvement over the PS3
dialog box, which is a bit clunky (&quot;clunky&quot; here means that the 
numbers are difficult to set precisely with the sliders provided by the dialog
box).
*******************************************************************************/
void CoreVision::openCameraSettings()
{
    debug_.printLn( "CoreVision::openCameraSettings() called..." );

    if( ps3Camera_ != NULL ) {
        debug_.printLn( "Opening the PS3 camera settings..." );
        ps3Camera_->showSettings();
    }
    else if( videoGrabber_ != NULL ) {
        debug_.printLn( "About to open settings for camera..." );
        videoGrabber_->videoSettings();
    }
}

/***************************************************************************//**
If a PS3 camera is in use, its settings will be saved out to an XML file.
*******************************************************************************/
void CoreVision::saveCameraSettingsToXmlFile()
{
    if( ps3Camera_ != NULL ) {
        ps3Camera_->saveCameraSettingsToXmlFile();
    }
}

/***************************************************************************//**
In addition to the filename, video width, video height, and expected frame rate,
the video writer is also told if the image has been flipped horizontally or
vertically so that the image can be flipped back.
*******************************************************************************/
bool CoreVision::startVideoWriter( const std::string & filename )
{
    //std::cout << "\nCoreVision::startVideoWriter( " << filename << " ) called.\n\n";
    videoWriter_->flipImageVertical( imageFilters_->flipImageVertical() );
    videoWriter_->flipImageHorizontal( imageFilters_->flipImageHorizontal() );
    recordSourceImages_ = videoWriter_->initGrayscale( filename, videoWidth_, videoHeight_ );
    return recordSourceImages_;
}

/***************************************************************************//**
Clears the video writer.
*******************************************************************************/
void CoreVision::stopVideoWriter()
{
    //std::cout << "\nCoreVision::stopVideoWriter() called.\n\n";
    recordSourceImages_ = false;
    videoWriter_->clear();
}

/***************************************************************************//**
Sets up the video player to start the next time that update() is called.
*******************************************************************************/
void CoreVision::playTestVideo( const std::string & filename )
{
    videoFileName_ = filename;
    startVideoPlayer_ = true;
    useCamera_ = false;
    startVideoCamera_ = false;

    debug_.printLn( "\nCoreVision::playTestVideo() called.\n" );
    debug_.printLn( "filename = " + filename );
    debug_.printLn( "startVideoPlayer_ = true" );
    debug_.printLn( "useCamera_ = false" );
    debug_.printLn( "startVideoCamera_ = false" );
}

/***************************************************************************//**
Sets up the camera to start the next time that update() is called.
*******************************************************************************/
void CoreVision::stopTestVideo()
{
    startVideoCamera_ = true;
    useCamera_ = true;
    startVideoPlayer_ = false;
        
    debug_.printLn( "CoreVision::stopTestVideo() called.\n" );
    debug_.printLn( "startVideoCamera_ = true" );
    debug_.printLn( "useCamera_ = true" );
    debug_.printLn( "startVideoPlayer_ = false" );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                         Blob Params Setters and Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Sets the image threshold, which is shown on a scale of 0 to 255 in the GUI.
*******************************************************************************/
void CoreVision::setImageThreshold( int imageThreshold )
{
    imageFilters_->setImageThreshold( imageThreshold );
    debug_.printLn( "CoreVision::setImageThreshold(" 
                  + debug_.intToStr( imageThreshold ) + ") called." );
}

/***************************************************************************//**
Returns the image threshold, which is shown on a scale of 0 to 255 in the GUI.
*******************************************************************************/
int CoreVision::getImageThreshold()
{
    return imageFilters_->imageThreshold();
}

/***************************************************************************//**
The min blob size is expected to be on a scale from 0 to 500.
*******************************************************************************/
void CoreVision::setMinBlobSize( int min )
{
    minBlobSize_ = min;
    debug_.printLn( "CoreVision::setMinBlobSize(" 
                  + debug_.intToStr( minBlobSize_ ) + ") called." );
}

/***************************************************************************//**
Returns the min blob size, which is expected to be on a scale from 0 to 500.
*******************************************************************************/
int CoreVision::getMinBlobSize()
{
    return minBlobSize_;
}

/***************************************************************************//**
The max blob size is expected to be on a scale from 0 to 1000.
*******************************************************************************/
void CoreVision::setMaxBlobSize( int max )
{
    maxBlobSize_ = max;
    debug_.printLn( "CoreVision::setMaxBlobSize(" 
                  + debug_.intToStr( maxBlobSize_ ) + ") called." );
}

/***************************************************************************//**
Returns the max blob size, which is expected to be on a scale from 0 to 1000.
*******************************************************************************/
int CoreVision::getMaxBlobSize()
{
    return maxBlobSize_;
}

/***************************************************************************//**
Sets the max number of blobs to track.  Up to 20 is a very reasonable number.
Up to 50 or even a hundred might work, but could slow the process down too much.
*******************************************************************************/
void CoreVision::setMaxNumberBlobs( int max )
{ 
    maxNumberBlobs_ = max;
    debug_.printLn( "CoreVision::setMaxNumberBlobs(" 
                  + debug_.intToStr( maxNumberBlobs_ ) + ") called." );
}

/***************************************************************************//**
Returns the max number of blobs to track.
*******************************************************************************/
int CoreVision::getMaxNumberBlobs()
{
    return maxNumberBlobs_;
}

/***************************************************************************//**
If the arg is true, the area in pixels for each blob is shown on the source 
image.
*******************************************************************************/
void CoreVision::showBlobAreas( bool b )
{
    showBlobAreas_ = b;
    debug_.printLn( "CoreVision::showBlobAreas(" 
                  + debug_.boolToStr( showBlobAreas_ ) + ") called." );
}

/***************************************************************************//**
Returns true if showing blob areas on the source image has been requested.
*******************************************************************************/
bool CoreVision::isShowBlobAreasOn()
{
    return showBlobAreas_;
}

/***************************************************************************//**
If the arg is true, the outline (contour) of each blob will be shown on the
source image along with a set of cross-hairs that go through the blob centroid.
*******************************************************************************/
void CoreVision::showBlobOutlines( bool b )
{
    showBlobOutlines_ = b;
    debug_.printLn( "CoreVision::showBlobOutlines(" 
                  + debug_.boolToStr( showBlobOutlines_ ) + ") called." );
}

/***************************************************************************//**
Returns true if showing blob outlines on the source image has been requested.
*******************************************************************************/
bool CoreVision::isShowBlobOutlinesOn()
{
    return showBlobOutlines_;
}

/***************************************************************************//**
Show crosshairs through centroid of each blob.
*******************************************************************************/
void CoreVision::showBlobCrosshairs( bool b )
{
    showBlobCrosshairs_ = b;
    debug_.printLn( "CoreVision::showBlobCrosshairs(" 
                  + debug_.boolToStr( showBlobCrosshairs_ ) + ") called." );
}

/***************************************************************************//**
Returns true if crosshairs through blob centroid are being shown.
*******************************************************************************/
bool CoreVision::isShowBlobCrosshairsOn()
{
    return showBlobCrosshairs_;
}

/***************************************************************************//**
Show the angled bounding box for each blob.
*******************************************************************************/
void CoreVision::showBlobBoundingBoxes( bool b )
{
    showBlobBoundingBoxes_ = b;
    debug_.printLn( "CoreVision::showBlobBoundingBoxes(" 
                  + debug_.boolToStr( showBlobBoundingBoxes_ ) + ") called." );
}

/***************************************************************************//**
Returns true if the angled bounding box for each blob is being shown.
*******************************************************************************/
bool CoreVision::isShowBlobBoundingBoxesOn()
{
    return showBlobBoundingBoxes_;
}

/***************************************************************************//**
Sets whether blob ID numbers should be rendered next to each blob in the final
tracked image shown in the GUI.
*******************************************************************************/
void CoreVision::showBlobLabels( bool b )
{
    showBlobLabels_ = b;
    debug_.printLn( "CoreVision::showBlobLabels(" 
                  + debug_.boolToStr( showBlobLabels_ ) + ") called." );
}

/***************************************************************************//**
Returns true if showing blob ID numbers in the tracked image has been requested.
*******************************************************************************/
bool CoreVision::isShowBlobLabelsOn()
{
    return showBlobLabels_;
}

/***************************************************************************//**
Sets whether the calibration grid should be shown as a wireframe on top of the
Source image.
*******************************************************************************/
void CoreVision::showCalibrationWireframe( bool b )
{
    showCalibrationWireframe_ = b;
}

/***************************************************************************//**
Returns true if showing the calibration wireframe has been requested.
*******************************************************************************/
bool CoreVision::isShowCalibrationWireframeOn()
{
    return showCalibrationWireframe_;
}

/***************************************************************************//**
Sets whether the calibration grid should be shown as a points on top of the
Source image along with an associated (source - background) intensity.
*******************************************************************************/
void CoreVision::showCalibrationIntensities( bool b )
{
    showCalibrationIntensities_ = b;
}

/***************************************************************************//**
Returns true if showing calibration intensities (source minus background)
has been requested.
*******************************************************************************/
bool CoreVision::isShowCalibrationIntensitiesOn()
{
    return showCalibrationIntensities_;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                        Background Setters and Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
The next camera frame will be saved as the background image to subtract from
all subsequent frames before image processing.
*******************************************************************************/
void CoreVision::learnBackground()
{
    imageFilters_->saveNextFrameAsBackground();

    if( !isCalibrating_ ) {
        updateNormalizationCountDown_ = BACKGROUND_FRAMES_COUNT + 1;
    }
    debug_.printLn( "CoreVision::learnBackground() called." );
}

/***************************************************************************//**
If set to true, the background is dynamically adjusted at a rate set by the
setBackgroundLearnRate() function.  This may be useful if their is a problem
with environmental lighting changing enough to affect blob detection.  When
auto background is turned on, each new camera image makes a minor weighted
contribution to the saved background image.
*******************************************************************************/
void CoreVision::useAutoBackground( bool b )
{
    imageFilters_->setAutoBackgroundActive( b );
    debug_.printLn( "useAutoBackground("
                  + debug_.boolToStr( b ) + ") called." );
}

/***************************************************************************//**
Returns true if the auto background feature is turned on.
*******************************************************************************/
bool CoreVision::useAutoBackground()
{
    return imageFilters_->isAutoBackgroundActive();
}

/***************************************************************************//**
Sets the learning rate that will be used if the auto background feature is
turned on.  Each new camera image will make a minor contribution to the saved
background image.  The higher the learn rate, the greater that contribution
will be.  The scale is expected to be from 0 to 500.
*******************************************************************************/
void CoreVision::setBackgroundLearnRate( float rate )
{
    backgroundLearnRate_ = rate;
    debug_.printLn( "setBackgroundLearnRate(" 
                  + debug_.intToStr( rate ) + ") called." );
}

/***************************************************************************//**
Returns the background learn rate, which is expected to be on a scale from 0 to 
500.
*******************************************************************************/
float CoreVision::getBackgroundLearnRate()
{
    return backgroundLearnRate_;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                            Filters Setters and Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
The ImageFilters class has the setters and getters for controlling the filters.
*******************************************************************************/
blobFilters::ImageFilters * CoreVision::imageFilters()
{
    return imageFilters_;
}

/***************************************************************************//**
Requests that GPU acceleration be used for image processing.
*******************************************************************************/
void CoreVision::useGpuShaders( bool b )
{
    useGpuShaders_ = b;
    debug_.printLn( "useGpuShaders(" 
                  + debug_.boolToStr( useGpuShaders_ ) + ") called." );
}

/***************************************************************************//**                   
Returns true of GPU acceleration for image processing has been requested.
*******************************************************************************/
bool CoreVision::useGpuShaders()
{
    return useGpuShaders_;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                            Network Setters and Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Expects an IP address for local host and port numbers for the TUIO channels.
This function is only expected to be called on once at the beginning of the 
program.  If called again, the TuioChannels object will ignore the call 
because the server code that is ultimately called does not currently appear 
to be set up to shut down and restart a UDP or TCP server during a program run.
*******************************************************************************/
void CoreVision::setupTuio( const std::string & localHost, 
                            int tuioUdpChannelOnePort, 
                            int tuioUdpChannelTwoPort,
                            int flashXmlChannelPort,
                            int binaryTcpChannelPort )
{
    tuioChannels_->setupTuio( localHost, 
                              tuioUdpChannelOnePort, 
                              tuioUdpChannelTwoPort, 
                              flashXmlChannelPort, 
                              binaryTcpChannelPort );
}

/***************************************************************************//**
Returns the IP address for local host.
*******************************************************************************/
std::string CoreVision::getLocalHost()
{
    return tuioChannels_->getLocalHost();
}

/***************************************************************************//**
Returns the port used for sending TUIO UDP.
*******************************************************************************/
int CoreVision::getTuioUdpChannelOnePort()
{
    return tuioChannels_->getUdpChannelOnePort();
}

/***************************************************************************//**
Returns the port used for sending TUIO UDP.
*******************************************************************************/
int CoreVision::getTuioUdpChannelTwoPort()
{
    return tuioChannels_->getUdpChannelTwoPort();
}

/***************************************************************************//**
Returns the port used for sending Flash XML.
*******************************************************************************/
int CoreVision::getFlashXmlChannelPort()
{
    return tuioChannels_->getFlashXmlChannelPort();
}

/***************************************************************************//**
Returns the port used for sending Binary TCP.
*******************************************************************************/
int CoreVision::getBinaryTcpChannelPort()
{
    return tuioChannels_->getBinaryTcpChannelPort();
}

/***************************************************************************//**
Can be used to silence all TUIO output.
*******************************************************************************/
void CoreVision::setNetworkSilentMode( bool b )
{
    tuioChannels_->setNetworkSilentMode( b );
}

/***************************************************************************//**
Returns true if network silent mode is being used to quiet all TUIO output.
*******************************************************************************/
bool CoreVision::useNetworkSilentMode()
{
    return tuioChannels_->useNetworkSilentMode();
}

/***************************************************************************//**
Sets whether TUIO UDP channel one is on or off.
*******************************************************************************/
void CoreVision::setUseTuioUdpChannelOne( bool b )
{
    tuioChannels_->setUdpChannelOne( b );
}

/***************************************************************************//**
Returns true if TUIO UDP channel one is in use.
*******************************************************************************/
bool CoreVision::useTuioUdpChannelOne()
{
    return tuioChannels_->useUdpChannelOne();
}

/***************************************************************************//**
Sets whether TUIO UDP channel two is on or off.
*******************************************************************************/
void CoreVision::setUseTuioUdpChannelTwo( bool b )
{
    tuioChannels_->setUdpChannelTwo( b );
}

/***************************************************************************//**
Returns true if TUIO UDP channel two is in use.
*******************************************************************************/
bool CoreVision::useTuioUdpChannelTwo()
{
    return tuioChannels_->useUdpChannelTwo();
}

/***************************************************************************//**
Sets whether TUIO Flash XML is in on or off.
*******************************************************************************/
void CoreVision::setUseFlashXmlChannel( bool b )
{
    tuioChannels_->setFlashXmlChannel( b );
}

/***************************************************************************//**
Returns true if the Flash XML channel is in use.
*******************************************************************************/
bool CoreVision::useFlashXmlChannel()
{
    return tuioChannels_->useFlashXmlChannel();
}

/***************************************************************************//**
Sets whether Binary Tcp is in on or off.
*******************************************************************************/
void CoreVision::setUseBinaryTcpChannel( bool b )
{
    tuioChannels_->setBinaryTcpChannel( b );
}

/***************************************************************************//**
Returns true if Binary TCP is in use.
*******************************************************************************/
bool CoreVision::useBinaryTcpChannel()
{
    return tuioChannels_->useBinaryTcpChannel();
}

/***************************************************************************//**
Calls on the TuioChannels object to set the TUIO profile for TUIO UDP messages.
*******************************************************************************/
void CoreVision::setTuioUdpProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    tuioChannels_->setUdpProfile( profile );
}

/***************************************************************************//**
Returns the TUIO profile currently used for TUIO UDP messages.
*******************************************************************************/
blobEnums::TuioProfileEnum::Enum CoreVision::getTuioUdpProfile()
{
    return tuioChannels_->getUdpProfile();
}

/***************************************************************************//**
Calls on the TuioChannels object to set the TUIO profile for Flash XML messages.
*******************************************************************************/
void CoreVision::setFlashXmlProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    tuioChannels_->setFlashXmlTuioProfile( profile );
}

/***************************************************************************//**
Returns the TUIO profile currently used for Flash XML messages.
*******************************************************************************/
blobEnums::TuioProfileEnum::Enum CoreVision::getFlashXmlProfile()
{
    return tuioChannels_->getFlashXmlProfile();
}

void CoreVision::setUnmatchedBlobFramesToSkip( int frames )
{
    contourFinder_.setUnmatchedBlobFramesToSkip( frames );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                     Keyboard Input for Calibration
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Passes the key pressed event on to the BlobCalibration object.
*******************************************************************************/
void CoreVision::keyPressed( KeyEnum::Enum key )
{
    blobCalibration_.keyPressed( key );
}

/***************************************************************************//**
Passes the key released event on to the BlobCalibration object.
*******************************************************************************/
void CoreVision::keyReleased( KeyEnum::Enum key )
{
    blobCalibration_.keyReleased( key );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                            Private Debugging Functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*******************************************************************************
Prints the video width and height to standard out.
*******************************************************************************/
void CoreVision::debugPrintVideoWidthAndHeight( int width, int height )
{
    debug_.printLn( "Started new video with (w, h) = (" 
                  + debug_.intToStr( width )
                  + ", "
                  + debug_.intToStr( height )
                  + ") \n" );
}

/*******************************************************************************
Prints the camera info to standard out.
*******************************************************************************/
void CoreVision::debugPrintCamera( int requestedWidth, 
                                   int requestedHeight,
                                   int actualWidth,
                                   int actualHeight )
{
    if( debug_.isDebuggingOn() ) {
        char reportStr[2048];
        std::sprintf( 
            reportStr,
            "Camera Mode\nAsked for %i by %i - actual size is %i by %i \n\n", 
            requestedWidth, 
            requestedHeight, 
            actualWidth, 
            actualHeight );
        debug_.printLn( reportStr );
    }
}

/*******************************************************************************
Prints the max number of blobs every 60th time this function is called
(so prints at about once per second if the frame rate is 60).
*******************************************************************************/
void CoreVision::debugPrintMaxNumBlobs( int maxNumBlobs )
{
    static int count = 0;
    ++count;

    if( (count % 60) == 0 ) { 
        std::cout << "maxNumBlobs = " << maxNumBlobs << "\n"; 
    }
}
