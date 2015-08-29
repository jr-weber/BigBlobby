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
#ifndef BLOBGUI_COREVISION_H
#define BLOBGUI_COREVISION_H

#include "blobCalibration/BlobCalibration.h"
#include "blobEnums/KeyEnum.h"
#include "blobEnums/TuioProfileEnum.h"
#include "blobIO/DebugPrint.h"
#include "blobTracker/BlobTracker.h"
#include "blobTracker/CPUImageFilter.h"
#include <ofVideoGrabber.h>
#include <ofTrueTypeFont.h>
#include <string>

#ifdef TARGET_WIN32
    #include <ofxffmv.h>
    #include <ofxPS3.h>
    #include <ofxDSVL.h>
#endif

namespace blobTuio { class TuioChannels; }
namespace blobFilters { class ImageFilters; }
namespace blobUtils { class FunctionTimer; }
namespace blobUtils { class StopWatch; }
namespace blobStats { class Stats; }
namespace blobVideo { class SimpleVideoWriter; }
namespace blobVideo { class SimpleVideoReader; }

/***************************************************************************//** 
Contains classes for blob detection and screen calibration using code from CCV 
(Community Core Vision) version 1.3.  
*******************************************************************************/
namespace blobCore 
{
    /***********************************************************************//**
     Manages the CCV (Community Core Vision) classes needed for detecting blobs
     resulting from fingertips moving on a multi-touch surface.  The CoreVision 
     class is intended as the only way that the BigBlobby Qt framework GUI 
     classes should access CCV code, so it has facade-type functions for
     controlling its helper classes, rather than exposing any helper classes
     directly.  To maintain a good separation between view (GUI) and underlying
     model (blob detection), the CoreVision class and its helper classes should
     not know anything about the Qt GUI classes or anything else in the Qt
     library.

     The CoreVision class is based on code from the Community Core Vision
     ofxNCoreVision class (files ofxNCoreVision.h, ofxNCoreVision.cpp, and
     gui.h), but does not include any of the GLUT graphical user interface code.
     Accordingly, the copyright statement from the ofxNCore header file has been
     included further below, and a copy of the Lesser General Public License 
     should be kept with this distribution. -JRW
     <br />

     ofxNCore.h
     <br />
     NUI Group Community Core Vision
     <br />

     Created by NUI Group Dev Team A on 3/1/09.
     Copyright 2009 NUI Group/Inc. All rights reserved.       
     Version : 1.20
     <br />

     Redistribution and use in source and binary forms, with or without
     modification, are permitted provided that the following conditions are met:
     <br />

     1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer as the first lines of
     this file unmodified.
     <br />

     2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
     <br />

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
     <br />

     Web: http://nuigroup.com
    ***************************************************************************/
    class CoreVision
    {
        public:
            static const std::string VIDEO_FILE_NAME;
            static const int VIDEO_WIDTH,
                             VIDEO_HEIGHT,
                             VIDEO_RATE,
                             MAX_NUMBER_BLOBS,
                             BACKGROUND_FRAMES_COUNT;

            CoreVision();
            ~CoreVision();
            void loadFontForBlobIDNumbers();
            void update();
            bool hasNewFrame();
            void drawBlobIdNumbers();
            void drawBlobOutlines();

            bool needToDisplayUpdatedFps();
            std::string getUpdatedFps();
            std::string getNetworkAndFiltersInfo();

            blobCalibration::BlobCalibration * blobCalibrationObj();
            void setUpBlobCalibration();
            void startCalibrationRoutine();
            void onCalibrationEnded();
            void drawCalibration( int screenWidth, int screenHeight );
            void drawCalibrationWireframe();
            void drawCalibrationPointsWithIntensities();
            void clear();

            void setDeviceID( int id );
            int getDeviceID();
            void setVideoRate( int rate );
            int getVideoRate();
            void setVideoWidth( int width );
            int getVideoWidth();
            void setVideoHeight( int height );
            int getVideoHeight();
            void flipImageHorizontal( bool b );
            bool flipImageHorizontal();
            void flipImageVertical( bool b );
            bool flipImageVertical();
            void openCameraSettings();
            void saveCameraSettingsToXmlFile();

            bool startVideoWriter( const std::string & filename );
            void stopVideoWriter();
            void playTestVideo( const std::string & filename );
            void stopTestVideo();

            void setImageThreshold( int imageThreshold );
            int getImageThreshold();
            void setMinBlobSize( int min );
            int getMinBlobSize();
            void setMaxBlobSize( int max );
            int getMaxBlobSize();
            void setMaxNumberBlobs( int max );
            int getMaxNumberBlobs();

            void showBlobOutlines( bool b );
            bool isShowBlobOutlinesOn();
            void showBlobCrosshairs( bool b );
            bool isShowBlobCrosshairsOn();
            void showBlobBoundingBoxes( bool b );
            bool isShowBlobBoundingBoxesOn();
            void showBlobAreas( bool b );
            bool isShowBlobAreasOn();
            void showBlobLabels( bool b );
            bool isShowBlobLabelsOn();

            void showCalibrationWireframe( bool b );
            bool isShowCalibrationWireframeOn();
            void showCalibrationIntensities( bool b );
            bool isShowCalibrationIntensitiesOn();

            void learnBackground();
            void useAutoBackground( bool b );
            bool useAutoBackground();
            void setBackgroundLearnRate( float rate );
            float getBackgroundLearnRate();

            blobFilters::ImageFilters * imageFilters();
            void useGpuShaders( bool b );
            bool useGpuShaders();

            void setupTuio( const std::string & localHost,
                            int tuioUdpChannelOnePort,
                            int tuioUdpChannelTwoPort,
                            int flashXmlChannelPort,
                            int binaryTcpChannelPort );
            std::string getLocalHost();
            int getTuioUdpChannelOnePort();
            int getTuioUdpChannelTwoPort();
            int getFlashXmlChannelPort();
            int getBinaryTcpChannelPort();
            
            void setNetworkSilentMode( bool b );
            void setUseTuioUdpChannelOne( bool b );
            void setUseTuioUdpChannelTwo( bool b );
            void setUseFlashXmlChannel( bool b );
            void setUseBinaryTcpChannel( bool b );

            bool useNetworkSilentMode();
            bool useTuioUdpChannelOne();
            bool useTuioUdpChannelTwo();
            bool useFlashXmlChannel();
            bool useBinaryTcpChannel();

            void setTuioUdpProfile( blobEnums::TuioProfileEnum::Enum profile );
            blobEnums::TuioProfileEnum::Enum getTuioUdpProfile();
            void setFlashXmlProfile( blobEnums::TuioProfileEnum::Enum profile );
            blobEnums::TuioProfileEnum::Enum getFlashXmlProfile();

            void setUnmatchedBlobFramesToSkip( int frames );

            // Keyboard Input for Calibration
            void keyPressed( blobEnums::KeyEnum::Enum key );
            void keyReleased( blobEnums::KeyEnum::Enum key );

        private:
            void stopVideoPlayer();
            void clearVideoFrame();
            void initVideoCamera();
            void setUpProcessedImage();
            void initVideoPlayer();

            void processVideoFrame();
            void calculateFps();
            void checkPeriodicBackground();
            void filterCameraImage();
            void findAndTrackBlobs();
            void updateAutoBackground();
            void sendTuio();
            void writeVideoIfRequested();

            // Private Debugging Functions
            void debugPrintVideoWidthAndHeight( int width, int height );
            void debugPrintCamera( int requestedWidth, 
                                   int requestedHeight,
                                   int actualWidth, 
                                   int actualHeight );
            void debugPrintMaxNumBlobs( int maxNumBlobs );

            // Camera
            ofVideoGrabber * videoGrabber_;
            bool useCamera_,
                 startVideoCamera_;
            int deviceID_,
                videoRate_,
                videoWidth_,
                videoHeight_;

            // VideoPlayer
            blobVideo::SimpleVideoReader * videoReader_;
            std::string videoFileName_;
            bool startVideoPlayer_;

            blobVideo::SimpleVideoWriter * videoWriter_;
            bool recordSourceImages_;

            // Blobs
            blobTracker::ContourFinder contourFinder_; // blob finder
            blobTracker::BlobTracker * blobTracker_;
            int minBlobSize_,
                maxBlobSize_,
                maxNumberBlobs_;
            bool showBlobAreas_,
                 showBlobOutlines_,
                 showBlobCrosshairs_,
                 showBlobBoundingBoxes_,
                 showBlobLabels_,
                 showCalibrationWireframe_,
                 showCalibrationIntensities_;
            ofTrueTypeFont verdana_;

            // Filters, Network, and Calibration
            blobFilters::ImageFilters * imageFilters_;
            float backgroundLearnRate_;
            CPUImageFilter processedImage_;
            bool useGpuShaders_;
            blobTuio::TuioChannels * tuioChannels_;
            blobCalibration::BlobCalibration blobCalibration_;
            bool isCalibrating_,
                 isFullscreen_;
            int updateNormalizationCountDown_;
            
            // fps
            blobStats::Stats * stats_;
            blobUtils::StopWatch * stopWatch_;
            blobUtils::FunctionTimer * contourFinderTimer_,
                                     * blobTrackerTimer_,
                                     * processTimer_;
            bool hasNewFrame_,
                 needToDisplayUpdatedFps_;
            int frames_;
            double fps_;

            bool hasExited_; // Used in ofxNCoreVision.  Not needed with Qt?

            // printing utility for debugging
            blobIO::DebugPrint debug_;
           
            // Windows-Specific Video Capture Devices
            #ifdef TARGET_WIN32
                    ofxffmv * ffmvCamera_;
                    ofxPS3 * ps3Camera_;
                    ofxDSVL * dsvlCamera_;
            #endif
    };
}

#endif
