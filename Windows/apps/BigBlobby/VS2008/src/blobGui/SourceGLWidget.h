////////////////////////////////////////////////////////////////////////////////
// SourceGLWidget
//
// PURPOSE: Operates the CoreVision object that provides blob detection and
//          provides an OpenGL context for displaying the Source image.
//
// CREATED: 4/13/2011
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2011, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#ifndef BLOBGUI_SOURCEGLWIDGET_H
#define BLOBGUI_SOURCEGLWIDGET_H

#include "blobCore/CoreVision.h"
#include "blobEnums/FilterEnum.h"
#include "blobEnums/TuioProfileEnum.h"
#include "blobIO/BlobParamsValidator.h"
#include "blobIO/DebugPrint.h"
#include <QGLWidget>
#include <QColor>

namespace blobCore { class BlobCalibration; }
namespace blobFilters { class ImageFilters; }
namespace blobBuffers { class GrayBuffer; }
namespace blobVideo { class SimpleVideoWriter; }

namespace blobGui 
{
    /***********************************************************************//**
    Operates the CoreVision object that provides blob detection and provides an 
    OpenGL context for displaying the Source image.

    Because of the way CCV (Community Core Vision) is written, an OpenGL context
    is needed for all rendering of images, and the rendering code is somewhat
    intertwined with the blob detection code.  At some future point, it would
    be a good idea to cleanly separate the blob detection code from the image
    rendering code.
    <br />

    To avoid having to make too many changes at this time to the underlying CCV
    code, this class, which is a subclass of QGLWidget, seemed like the best 
    place to call on the CCV code while providing a current OpenGL context.
    Therefore, this class creates and holds on to an instance of CoreVision,
    which provides access to the CCV functionality.
    <br />

    Design Note<br />
    A good separation between model classes and GUI classes is generally
    desirable because that makes the model classes much more reusable.  In other
    words, the relationship should be that the GUI classes know about the
    underlying model classes, but the model classes should not know about the 
    GUI.  Therefore, ideally, CoreVision should not know anything about the Qt 
    GUI.  For that reason, this class has a large number of public slots that 
    simply call on the CoreVision function of the same name (<i>i.e.</i>, slots 
    are a Qt concept that CoreVision should not know about).
    <br />

    Although CoreVision was written to be as separate as possible from the 
    GUI code, CoreVision does need to know about the blobEnums::FilterEnums and 
    blobEnums::KeyEnum classes that are also used by the GUI.  CoreVision
    also knows about the blobIO::DebugPrint class, but only calls on print
    functions that use standard C++ strings, not any QString objects.
    <br />
    ***************************************************************************/
    class SourceGLWidget : public QGLWidget
    {
        Q_OBJECT

        public:
            static const int VIDEO_WIDTH,
                             VIDEO_HEIGHT;

            SourceGLWidget( QWidget * parent = NULL );
            ~SourceGLWidget();

            void setHelpImage( QImage & helpImage );
            void showHelpImage( bool show );
            bool hasNewFrame();
            void frame( double deltaFrameTime );
            bool needToDisplayUpdatedFps();
            QString getUpdatedFps();
            QString getNetworkAndFiltersInfo();
            
            blobCore::CoreVision * getCoreVision();
            void initializeCoreVisionParams( blobIO::BlobParamsValidator * validator );
            void updateCoreVisionParams( blobIO::BlobParamsValidator * validator );
            void getCoreVisionParams( blobIO::BlobParamsValidator * validator );

        public slots:
            // Calibration Slots
            void startCalibrationRoutine();
            void onCalibrationEnded();
                
            // Camera Slots
            void setDeviceID( int id );
            void setVideoRate( int rate );
            void setVideoWidth( int width );
            void setVideoHeight( int height );
            void flipImageHorizontal( bool b );
            void flipImageVertical( bool b );
            void openCameraSettings();
            void saveCameraSettingsToXmlFile();

            // Video Writer/Player Slots
            void playTestVideo( const QString & filename );
            void stopTestVideo();
            bool startSourceInputRecording( const QString & filename );
            bool startSourceColorRecording( const QString & filename );
            void stopVideoWriter();
            
            // Blob Slots
            void setImageThreshold( int imageThreshold );
            void setMinBlobSize( int min );
            void setMaxBlobSize( int max );
            void setMaxNumberBlobs( int max );
            void useDarkBlobs( bool b );

            void showBlobAreas( bool b );
            void showBlobOutlines( bool b );
            void showBlobCrosshairs( bool b );
            void showBlobBoundingBoxes( bool b );

            void normalizeBlobIntensities( bool b );
            void showBlobLabels( bool b );
            void showCalibrationWireframe( bool b );
            void showCalibrationIntensities( bool b );
            void correctForSignalVariation( bool b );
            
            // Background and Dynamic Background Slots
            void learnBackground();
            void useAutoBackground( bool b );
            void setBackgroundLearnRate( float rate );
            void setBackgroundLearnRate( int rate );
            void usePeriodicBackground( bool b );
            void setPeriodicBackgroundSeconds( float t );
            void setPeriodicBackgroundSeconds( int t );
            void setPeriodicBackgroundPercent( float percent );
            void setPeriodicBackgroundPercent( int percent );
            
            // Filters Slots
            void usePreBkSmoothing( bool b );
            void setPreBkSmoothingBlur( int smooth );
            void useGaussianPreBkSmoothing( bool b );
            void setGaussianPreBkSmoothingSigma( double sigma );

            void useMask( bool b );
            void setMaskWidthPadding( int pixels );
            void setMaskHeightPadding( int pixels );
            void useHighpass( bool b );
            void setHighpassBlur( int blur );
            void showHighpassSubtractionImage( bool show );
            void useHighpassAmplify( bool b );
            void setHighpassAmplifyLevel( int level );

            void useHighpassTwo( bool b );
            void setHighpassTwoKernel( int size );
            void showHighpassTwoSubtractionImage( bool show );
            void useHighpassTwoAmplify( bool b );
            void setHighpassTwoAmplifyLevel( int amplify );

            void useSmoothing( bool b );
            void setSmoothingBlur( int smooth );
            void useGaussianSmoothing( bool b );
            void setGaussianSmoothingSigma( double sigma );
            void useSmoothingAmplify( bool b );
            void setSmoothingAmplifyLevel( int amplify );

            // Network Slots
            void setNetworkSilentMode( bool b );
            void useBinaryTcpChannel( bool b );
            void useTuioUdpChannelOne( bool b );
            void useTuioUdpChannelTwo( bool b );
            void useFlashXmlChannel( bool b );
            void setTuioUdpProfileToTuio2Dcur( bool b );
            void setTuioUdpProfileToTuio2Dblb( bool b );
            void setTuioUdpProfileToCurAndBlb( bool b );
            void setTuioUdpProfile( blobEnums::TuioProfileEnum::Enum profile );
            void setFlashXmlProfileToTuio2Dcur( bool b );
            void setFlashXmlProfileToTuio2Dblb( bool b );
            void setFlashXmlProfileToCurAndBlb( bool b );
            void setFlashXmlProfile( blobEnums::TuioProfileEnum::Enum profile );
            void setupTuio( const std::string & localHost,
                            int tuioUdpChannelOnePort,
                            int tuioUdpChannelTwoPort,
                            int flashXmlChannelPort,
                            int binaryTcpChannelPort );

            // TEMPORARY CODE
            void setUnmatchedBlobFramesToSkip( int frames );
            void setUnmatchedBlobZeroFramesToSkip( bool b );
            void setUnmatchedBlobFiveFramesToSkip( bool b );
            void setUnmatchedBlobTenFramesToSkip( bool b );
            void setUnmatchedBlobTwentyFramesToSkip( bool b );

        signals:
            void writeColorVideoFailed();

        protected:
            void initializeGL();
            void resizeGL( int viewportWidth, int viewportHeight );
            void paintGL();

        private:
            void intitializeCoreVision();
            void resetCoreVision();
            void drawHelpImage( int width, int height );
            void drawSourceImage();
            void drawSourceImage( blobBuffers::GrayBuffer * grayBuffer );

            void recordSourceImageIfRequested();
            bool cameraSizeHasChanged( blobBuffers::GrayBuffer * grayBuffer );
            void drawTexture( blobBuffers::GrayBuffer * grayBuffer );
            void initializeTexture( int width, int height );
            void initBackgroundColor();

            void debugPrintBackgroundColor();
            void debugPrintResizeGL( int viewportWidth, 
                                     int viewportHeight, 
                                     int videoWidth, 
                                     int videoHeight );
            void debugPrintSetUpTuio( const std::string & localHost,
                                      int tuioUdpChannelOnePort,
                                      int tuioUdpChannelTwoPort,
                                      int flashXmlChannelPort,
                                      int binaryTcpChannelPort );

            blobCore::CoreVision coreVision_;
            blobFilters::ImageFilters * imageFilters_;
            blobIO::BlobParamsValidator paramsValidator_;
            bool initializeCoreVision_,
                 updateCoreVision_;
            QColor backgroundColor_;
            GLuint textureName_;
            int textureWidth_,
                textureHeight_,
                viewportWidth_,
                viewportHeight_,
                videoWidth_,
                videoHeight_;
            blobVideo::SimpleVideoWriter * videoWriter_;
            bool recordSourceImages_,
                 usePeriodicSubtraction_;
            bool showHelpImage_;
            QImage helpImage_;
            GLuint helpTextureName_;

            // Debugging Utility that Writes to Standard Out
            blobIO::DebugPrint debug_;
    };
}

#endif
