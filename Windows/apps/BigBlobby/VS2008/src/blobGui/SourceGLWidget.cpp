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
#include "blobGui/SourceGLWidget.h"
#include "blobIO/BlobParamsValidator.h"
#include "blobFilters/ImageFilters.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobVideo/SimpleVideoWriter.h"
#include <QPalette>
#include <QColor>

using blobGui::SourceGLWidget;
using blobEnums::FilterEnum;
using blobCore::CoreVision;
using blobIO::BlobParamsValidator;  

const int SourceGLWidget::VIDEO_WIDTH = 320,
          SourceGLWidget::VIDEO_HEIGHT = 240;

/***************************************************************************//**
The constructor creates the CoreVision object that runs the CCV 1.3 code and
also creates a few other helper objects.  However, the OpenGL state is not 
initialized here.  That happens later when the protected initializeGL() function
is called.
*******************************************************************************/
SourceGLWidget::SourceGLWidget( QWidget * parent ) :
  QGLWidget( parent ),
  coreVision_(),
  imageFilters_( NULL ),
  paramsValidator_(),
  initializeCoreVision_( false ),
  updateCoreVision_( false ),
  backgroundColor_(),
  textureName_( 0 ),
  textureWidth_( 0 ),
  textureHeight_( 0 ),
  viewportWidth_( VIDEO_WIDTH ),
  viewportHeight_( VIDEO_HEIGHT ),
  videoWidth_( VIDEO_WIDTH ),
  videoHeight_( VIDEO_HEIGHT ),
  videoWriter_( new blobVideo::SimpleVideoWriter() ),
  recordSourceImages_( false ),
  showHelpImage_( false ),
  helpImage_(),
  helpTextureName_( 0 ),
  debug_( false )
{
    imageFilters_ = coreVision_.imageFilters();
    initBackgroundColor();
}

SourceGLWidget::~SourceGLWidget()
{
    makeCurrent();
    coreVision_.clear();
}

void SourceGLWidget::setHelpImage( QImage & helpImage )
{
    helpImage_ = helpImage;

    if( helpImage_.isNull() ) {
        std::cout << "The image is NULL.\n"; 
    }
}

void SourceGLWidget::showHelpImage( bool show )
{
    showHelpImage_ = show;
}

/***************************************************************************//**
Asks the CoreVision object if the camera has a new frame.
*******************************************************************************/
bool SourceGLWidget::hasNewFrame()
{
    return coreVision_.hasNewFrame();
}

/***************************************************************************//**
Calls on updateGL(), which will set up the OpenGL context and then call on 
paintGL().  

This function does not block.
*******************************************************************************/
void SourceGLWidget::frame( double deltaFrameTime )
{
    updateGL();
}

/***************************************************************************//**
Checks with the CoreVision object to see if it is time to update the GUI with
the most recent frames per second calculation (usually only done every second
or so).
*******************************************************************************/
bool SourceGLWidget::needToDisplayUpdatedFps()
{
    return coreVision_.needToDisplayUpdatedFps();
}

/***************************************************************************//**
Gets a message with frames per second and other info from the CoreVision object.
*******************************************************************************/
QString SourceGLWidget::getUpdatedFps()
{
    return QString( coreVision_.getUpdatedFps().c_str() );
}

/***************************************************************************//**
Gets a message with network and filter timing info from the CoreVision object.
*******************************************************************************/
QString SourceGLWidget::getNetworkAndFiltersInfo()
{
    return QString( coreVision_.getNetworkAndFiltersInfo().c_str() );
}

/***************************************************************************//**
Returns a reference to the CoreVision object that runs the CCV 1.3 code.
*******************************************************************************/
blobCore::CoreVision * SourceGLWidget::getCoreVision()
{
    return &coreVision_;
}

/***************************************************************************//**
Uses information from the validator arg to update the CoreVision object.

The video camera width and height will be updated immediately.  The rest of the
info in the validator object given as an argument is copied over to a persistent
validator object owned by this class, and the updateCoreVision_ boolean data
member will be set to true.  That boolean value will be seen the next time the
paintGL() function is called, and that is when the resetCoreVsion() function will
be called to update all settings from the persistent validator object.  This
approach was taken to avoid any potential threading issues.  By updating the
settings as the first action within paintGL(), all settings are guaranteed to be
set before the call to process the next image for blob detection and then render
the new image.
*******************************************************************************/
void SourceGLWidget::initializeCoreVisionParams( blobIO::BlobParamsValidator * validator )
{
    coreVision_.setVideoWidth( validator->getVideoWidth() );
    coreVision_.setVideoHeight( validator->getVideoHeight() );
    paramsValidator_ = *validator; // Copy values to persistent validator.
    initializeCoreVision_ = true; // Update occurs on next call to paintGL()
}

void SourceGLWidget::updateCoreVisionParams( blobIO::BlobParamsValidator * validator )
{
    paramsValidator_ = *validator; // Copy values to persistent validator.
    updateCoreVision_ = true; // Update occurs on next call to paintGL()
}

void SourceGLWidget::intitializeCoreVision()
{
    // Camera slots
    setDeviceID( paramsValidator_.getDeviceID() );
    setVideoRate( paramsValidator_.getVideoRate() );
    setVideoWidth( paramsValidator_.getVideoWidth() );
    setVideoHeight( paramsValidator_.getVideoHeight() );

    resetCoreVision();

    // setUpTuio() only has an effect the first time it is called.
    setupTuio( paramsValidator_.getLocalHost(),
               paramsValidator_.getTuioUdpChannelOnePort(),
               paramsValidator_.getTuioUdpChannelTwoPort(),
               paramsValidator_.getFlashXmlChannelPort(),
               paramsValidator_.getBinaryTcpChannelPort() );
    coreVision_.setUpBlobCalibration();
}

void SourceGLWidget::resetCoreVision()
{
    // Camera slots
    flipImageHorizontal( paramsValidator_.flipImageHorizontally() );
    flipImageVertical( paramsValidator_.flipImageVertically() );

    // Blob slots
    setImageThreshold( paramsValidator_.getImageThreshold() );
    setMinBlobSize( paramsValidator_.getMinBlobSize() );
    setMaxBlobSize( paramsValidator_.getMaxBlobSize() );
    setMaxNumberBlobs( paramsValidator_.getMaxNumberBlobs() );
    useDarkBlobs( paramsValidator_.useDarkBlobs() );

    showCalibrationWireframe( paramsValidator_.isShowCalibrationGridOn() );
    showBlobAreas( paramsValidator_.isShowBlobAreasOn() );
    showBlobOutlines( paramsValidator_.isShowBlobOutlinesOn() );
    showBlobCrosshairs( paramsValidator_.isShowBlobCrosshairsOn() );
    showBlobBoundingBoxes( paramsValidator_.isShowBlobBoundingBoxesOn() );

    normalizeBlobIntensities( paramsValidator_.isNormalizeBlobIntensitiesOn() );
    showBlobLabels( paramsValidator_.isShowBlobLabelsOn() );

    // Background and Dynamic Background slots
    learnBackground();
    useAutoBackground( paramsValidator_.useAutoBackground() );
    setBackgroundLearnRate( paramsValidator_.getBackgroundLearnRate() );
    usePeriodicBackground( paramsValidator_.usePeriodicBackground() );
    setPeriodicBackgroundSeconds( paramsValidator_.getPeriodicBackgroundSeconds() );
    setPeriodicBackgroundPercent( paramsValidator_.getPeriodicBackgroundPercent() );

    // Filters slots
    usePreBkSmoothing( paramsValidator_.usePreBkSmoothing() );
    setPreBkSmoothingBlur( paramsValidator_.getPreBkSmoothingBlur() );
    useGaussianPreBkSmoothing( paramsValidator_.useGaussianPreBkSmoothing() );
    setGaussianPreBkSmoothingSigma( paramsValidator_.getGaussianPreBkSigma() );

    useMask( paramsValidator_.useMask() );
    setMaskWidthPadding( paramsValidator_.getMaskWidthPadding() );
    setMaskHeightPadding( paramsValidator_.getMaskHeightPadding() );

    useHighpass( paramsValidator_.useHighpass() );
    setHighpassBlur( paramsValidator_.getHighpassBlur() );
    useHighpassAmplify( paramsValidator_.useHighpassAmplify() );
    setHighpassAmplifyLevel( paramsValidator_.getHighpassAmplifyLevel() );

    useHighpassTwo( paramsValidator_.useHighpassTwo() );
    setHighpassTwoKernel( paramsValidator_.getHighpassTwoKernel() );
    useHighpassTwoAmplify( paramsValidator_.useHighpassTwoAmplify() );
    setHighpassTwoAmplifyLevel( paramsValidator_.getHighpassTwoAmplifyLevel() );

    useSmoothing( paramsValidator_.useSmoothing() );
    setSmoothingBlur( paramsValidator_.getSmoothingBlur() );
    useGaussianSmoothing( paramsValidator_.useGaussianSmoothing() );
    setGaussianSmoothingSigma( paramsValidator_.getGaussianSigma() );
    useSmoothingAmplify( paramsValidator_.useSmoothingAmplify() );
    setSmoothingAmplifyLevel( paramsValidator_.getSmoothingAmplifyLevel() );

    // Network slots
    useTuioUdpChannelOne( paramsValidator_.useTuioUdpChannelOne() );
    useTuioUdpChannelTwo( paramsValidator_.useTuioUdpChannelTwo() );
    useFlashXmlChannel( paramsValidator_.useFlashXmlChannel() );
    useBinaryTcpChannel( paramsValidator_.useBinaryTcpChannel() );
    setTuioUdpProfile( paramsValidator_.getTuioUdpProfile() );
    setFlashXmlProfile( paramsValidator_.getFlashXmlProfile() );
}

/***************************************************************************//**
Obtains values from the CoreVision object and uses them to set all data members
of the BlobParamsValidator given as an argument.

This function is needed so that the BlobParamsValidator object can be populated
with program settings and then handed over to an XML file writer.  The writer
should save out program settings when the program exits normally.

Use camera will always be set to true, and the video width and height will stay 
the same.  Otherwise, there is a potential problem with use of the video player
when the video player has a different resolution than camera.
*******************************************************************************/
void SourceGLWidget::getCoreVisionParams( blobIO::BlobParamsValidator * validator )
{
    // Camera unchecked setters
    validator->setDeviceID( coreVision_.getDeviceID() );
    validator->setVideoRate( coreVision_.getVideoRate() );
    validator->setVideoWidth( paramsValidator_.getVideoWidth() );
    validator->setVideoHeight( paramsValidator_.getVideoHeight() );
    validator->flipImageVertically( coreVision_.flipImageVertical() );
    validator->flipImageHorizontally( coreVision_.flipImageHorizontal() );

    // Blobs unchecked setters
    validator->setImageThreshold( coreVision_.getImageThreshold() );
    validator->setMinBlobSize( coreVision_.getMinBlobSize() );
    validator->setMaxBlobSize( coreVision_.getMaxBlobSize() );
    validator->setMaxNumberBlobs( coreVision_.getMaxNumberBlobs() );
    validator->useDarkBlobs( imageFilters_->useDarkBlobs() );

    validator->showCalibrationGrid( coreVision_.isShowCalibrationWireframeOn() );
    validator->showBlobAreas( coreVision_.isShowBlobAreasOn() );
    validator->showBlobOutlines( coreVision_.isShowBlobOutlinesOn() );
    validator->showBlobCrosshairs( coreVision_.isShowBlobCrosshairsOn() );
    validator->showBlobBoundingBoxes( coreVision_.isShowBlobBoundingBoxesOn() );

    validator->normalizeBlobIntensities( imageFilters_->isSignalNormalizationActive() );
    validator->showBlobLabels( coreVision_.isShowBlobLabelsOn() );

    // Background unchecked setters
    validator->useAutoBackground( coreVision_.useAutoBackground() );
    validator->setBackgroundLearnRate( coreVision_.getBackgroundLearnRate() );
    validator->usePeriodicBackground( imageFilters_->isPeriodicBackgroundActive() );
    validator->setPeriodicBackgroundSeconds( imageFilters_->getPeriodicBackgroundSeconds() );
    validator->setPeriodicBackgroundPercent( imageFilters_->getPeriodicBackgroundPercent() );

    // Filtering unchecked setters
    validator->usePreBkSmoothing( imageFilters_->isPreBackgroundSmoothingActive() );
    validator->setPreBkSmoothingBlur( imageFilters_->preBackgroundSmoothingBlurSize() );
    validator->useGaussianPreBkSmoothing( imageFilters_->isGaussianPreBackgroundSmoothingActive() );
    validator->setGaussianPreBkSigma( imageFilters_->gaussianPreBackgroundSmoothingSigma() );

    validator->useMask( imageFilters_->isMaskActive() );
    validator->setMaskWidthPadding( imageFilters_->maskWidthPadding() );
    validator->setMaskHeightPadding( imageFilters_->maskHeightPadding() );

    validator->useHighpass( imageFilters_->isHighpassActive() );
    validator->setHighpassBlur( imageFilters_->highpassBlurSize() );
    validator->useHighpassAmplify( imageFilters_->isHighpassAmplifyActive() );
    validator->setHighpassAmplifyLevel( imageFilters_->highpassAmplifyLevel() );

    validator->useHighpassTwo( imageFilters_->isHighpassTwoActive() );
    validator->setHighpassTwoKernel( imageFilters_->highpassTwoKernel() );
    validator->useHighpassTwoAmplify( imageFilters_->isHighpassTwoAmplifyActive() );
    validator->setHighpassTwoAmplifyLevel( imageFilters_->highpassTwoAmplifyLevel() );

    validator->useSmoothing( imageFilters_->isSmoothingActive() );
    validator->setSmoothingBlur( imageFilters_->smoothingBlurSize() );
    validator->useGaussianSmoothing( imageFilters_->isGaussianSmoothingActive() );
    validator->setGaussianSigma( imageFilters_->gaussianSmoothingSigma() );
    validator->useSmoothingAmplify( imageFilters_->isSmoothingAmplifyActive() );
    validator->setSmoothingAmplifyLevel( imageFilters_->smoothingAmplifyLevel() );

    // Network unchecked setters
    // TODO: Add silent mode to CoreVision so state can be obtained here.
    validator->useNetworkSilentMode( coreVision_.useNetworkSilentMode() );
    validator->useTuioUdpChannelOne( coreVision_.useTuioUdpChannelOne() );
    validator->useTuioUdpChannelTwo( coreVision_.useTuioUdpChannelTwo() );
    validator->useFlashXmlChannel( coreVision_.useFlashXmlChannel() );
    validator->useBinaryTcpChannel( coreVision_.useBinaryTcpChannel() );
    validator->setLocalHost( coreVision_.getLocalHost() );
    validator->setTuioUdpChannelOnePort(coreVision_.getTuioUdpChannelOnePort() );
    validator->setTuioUdpChannelTwoPort(coreVision_.getTuioUdpChannelTwoPort() );
    validator->setTuioUdpProfile( coreVision_.getTuioUdpProfile() );
    validator->setFlashXmlChannelPort( coreVision_.getFlashXmlChannelPort() );
    validator->setFlashXmlProfile( coreVision_.getFlashXmlProfile() );
    validator->setBinaryTcpChannelPort( coreVision_.getBinaryTcpChannelPort() );
}

/***************************************************************************//**
If a PS3 camera is in use, its settings will be saved out to an XML file.
*******************************************************************************/
void SourceGLWidget::saveCameraSettingsToXmlFile()
{
    coreVision_.saveCameraSettingsToXmlFile();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                  Calibration Slot
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::startCalibrationRoutine()
{
    debug_.printLn( "SourceGLWidget::startCalibrationRoutine() called...\n" );

    // Periodic subtraction will interfere with calibration, so shut it off.
    imageFilters_->usePeriodicBackground( false );
    coreVision_.startCalibrationRoutine();
}

void SourceGLWidget::onCalibrationEnded()
{
    coreVision_.onCalibrationEnded();

    // Restore periodic subtraction if it had been turned off for calibration.
    imageFilters_->usePeriodicBackground( usePeriodicSubtraction_ );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                  Camera Slots
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setDeviceID( int id )
{
    coreVision_.setDeviceID( id );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setVideoRate( int rate )
{
    coreVision_.setVideoRate( rate );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setVideoWidth( int width )
{
    coreVision_.setVideoWidth( width );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setVideoHeight( int height )
{
    coreVision_.setVideoHeight( height );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::flipImageHorizontal( bool b )
{
    coreVision_.flipImageHorizontal( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::flipImageVertical( bool b )
{
    coreVision_.flipImageVertical( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::openCameraSettings()
{
    coreVision_.openCameraSettings();
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::playTestVideo( const QString & filename )
{
    coreVision_.playTestVideo( filename.toStdString() );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::stopTestVideo()
{
    coreVision_.stopTestVideo();
}

/***************************************************************************//**
Slot to call on CoreVision startVideoWriter().
*******************************************************************************/
bool SourceGLWidget::startSourceInputRecording( const QString & filename )
{
    return coreVision_.startVideoWriter( filename.toStdString() );
}

/***************************************************************************//**
Starts a video recording of the Source image and includes anything written on 
top of the image in color.
*******************************************************************************/
bool SourceGLWidget::startSourceColorRecording( const QString & filename )
{
    videoWriter_->flipImageHorizontal( !imageFilters_->flipImageHorizontal() );
    videoWriter_->flipImageVertical( !imageFilters_->flipImageVertical() );
    recordSourceImages_ = videoWriter_->initColor( filename.toStdString(), width(), height() );
    return recordSourceImages_;
}

/***************************************************************************//**
Clears any in-progress video writer (either the one held by CoreVision or the
one held by this class).
*******************************************************************************/
void SourceGLWidget::stopVideoWriter()
{
    coreVision_.stopVideoWriter(); // Grayscale recording of source input.
    recordSourceImages_ = false;
    videoWriter_->clear(); // Color recording of source image.
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                   Blob Slots
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setImageThreshold( int imageThreshold )
{
    coreVision_.setImageThreshold( imageThreshold );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setMinBlobSize( int min )
{
    coreVision_.setMinBlobSize( min );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setMaxBlobSize( int max )
{
    coreVision_.setMaxBlobSize( max );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setMaxNumberBlobs( int max )
{
    coreVision_.setMaxNumberBlobs( max );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::useDarkBlobs( bool b )
{
    imageFilters_->useDarkBlobs( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::showBlobAreas( bool b )
{
    coreVision_.showBlobAreas( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::showBlobOutlines( bool b )
{
    coreVision_.showBlobOutlines( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::showBlobCrosshairs( bool b )
{
    coreVision_.showBlobCrosshairs( b );
}
/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::showBlobBoundingBoxes( bool b )
{
    coreVision_.showBlobBoundingBoxes( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::normalizeBlobIntensities( bool b )
{
    imageFilters_->setSignalNormalizationActive( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::showBlobLabels( bool b )
{
    coreVision_.showBlobLabels( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::showCalibrationWireframe( bool b )
{
    coreVision_.showCalibrationWireframe( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::showCalibrationIntensities( bool b )
{
    coreVision_.showCalibrationIntensities( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::correctForSignalVariation( bool b )
{
    imageFilters_->setSignalNormalizationActive( b );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                  Background and Dynamic Background Slots
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::learnBackground()
{
    coreVision_.learnBackground();
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::useAutoBackground( bool b )
{
    coreVision_.useAutoBackground( b );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setBackgroundLearnRate( float rate )
{
    coreVision_.setBackgroundLearnRate( rate );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setBackgroundLearnRate( int rate )
{
    coreVision_.setBackgroundLearnRate( rate );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::usePeriodicBackground( bool b )
{
    imageFilters_->usePeriodicBackground( b );
    usePeriodicSubtraction_ = b;
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setPeriodicBackgroundSeconds( float t )
{
    imageFilters_->setPeriodicBackgroundSeconds( t );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setPeriodicBackgroundSeconds( int t )
{
    imageFilters_->setPeriodicBackgroundSeconds( t );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setPeriodicBackgroundPercent( float percent )
{
    imageFilters_->setPeriodicBackgroundPercent( percent );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.
*******************************************************************************/
void SourceGLWidget::setPeriodicBackgroundPercent( int percent )
{
    imageFilters_->setPeriodicBackgroundPercent( percent );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                  Filters Slots
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::usePreBkSmoothing( bool b )
{
    imageFilters_->setPreBackgroundSmoothingActive( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setPreBkSmoothingBlur( int blur )
{
    imageFilters_->setPreBackgroundSmoothingBlurSize( blur );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useGaussianPreBkSmoothing( bool b )
{
    imageFilters_->useGaussianPreBackgroundSmoothing( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setGaussianPreBkSmoothingSigma( double sigma )
{
    imageFilters_->setGaussianPreBackgroundSmoothingSigma( sigma );
}

void SourceGLWidget::useMask( bool b )
{
    imageFilters_->setMaskActive( b );
}

void SourceGLWidget::setMaskWidthPadding( int pixels )
{
    imageFilters_->setMaskWidthPadding( pixels );
}

void SourceGLWidget::setMaskHeightPadding( int pixels )
{
    imageFilters_->setMaskHeightPadding( pixels );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useHighpass( bool b )
{
    imageFilters_->setHighpassActive( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setHighpassBlur( int blur )
{
    imageFilters_->setHighpassBlurSize( blur );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::showHighpassSubtractionImage( bool show )
{
    imageFilters_->showHighpassSubtractionImage( show );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useHighpassAmplify( bool b )
{
    imageFilters_->setHighpassAmplifyActive( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setHighpassAmplifyLevel( int level )
{
    imageFilters_->setHighpassAmplifyLevel( level );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useHighpassTwo( bool b )
{
    imageFilters_->setHighpassTwoActive( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setHighpassTwoKernel( int size )
{
    imageFilters_->setHighpassTwoKernel( size );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::showHighpassTwoSubtractionImage( bool show )
{
    imageFilters_->showHighpassTwoSubtractionImage( show );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useHighpassTwoAmplify( bool b )
{
    imageFilters_->setHighpassTwoAmplifyActive( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setHighpassTwoAmplifyLevel( int amplify )
{
    imageFilters_->setHighpassTwoAmplifyLevel( amplify );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useSmoothing( bool b )
{
    imageFilters_->setSmoothingActive( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setSmoothingBlur( int blur )
{
    imageFilters_->setSmoothingBlurSize( blur );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useGaussianSmoothing( bool b )
{
    imageFilters_->useGaussianSmoothing( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setGaussianSmoothingSigma( double sigma )
{
    imageFilters_->setGaussianSmoothingSigma( sigma );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::useSmoothingAmplify( bool b )
{
    imageFilters_->setSmoothingAmplifyActive( b );
}

/***************************************************************************//**
Slot to call on ImageFilters function.
*******************************************************************************/
void SourceGLWidget::setSmoothingAmplifyLevel( int amplify )
{
    imageFilters_->setSmoothingAmplifyLevel( amplify );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                  Network Slots
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
If arg is true, this slot calls on CoreVision function setUseTuioUdpChannelOne().
*******************************************************************************/
void SourceGLWidget::setNetworkSilentMode( bool b )
{
    //std::cout << "SourceGLWidget::setNetworkSilentMode(" << b << ") called.\n";
    coreVision_.setNetworkSilentMode( b );
}

/***************************************************************************//**
Calls on CoreVision to turn TUIO UDP channel 1 output on/off.
*******************************************************************************/
void SourceGLWidget::useTuioUdpChannelOne( bool b )
{
    debug_.printLn( "SourceGLWidget::useTuioUdpChannelOne(" + debug_.boolToStr( b ) + ")\n" );
    coreVision_.setUseTuioUdpChannelOne( b );
}

/***************************************************************************//**
Calls on CoreVision to turn TUIO UDP channel 2 output on/off.
*******************************************************************************/
void SourceGLWidget::useTuioUdpChannelTwo( bool b )
{
    debug_.printLn( "SourceGLWidget::useTuioUdpChannelTwo( " + debug_.boolToStr( b ) + " )\n" );
    coreVision_.setUseTuioUdpChannelTwo( b );
}

/***************************************************************************//**
Calls on CoreVision to turn Flash XML output on/off.
*******************************************************************************/
void SourceGLWidget::useFlashXmlChannel( bool b )
{
    debug_.printLn( "SourceGLWidget::useFlashXmlChannel(" + debug_.boolToStr( b ) + ")\n" );
    coreVision_.setUseFlashXmlChannel( b );
}

/***************************************************************************//**
If arg is true, this slot calls on CoreVision function setUseBinaryTcpChannel().
*******************************************************************************/
void SourceGLWidget::useBinaryTcpChannel( bool b )
{
    debug_.printLn( "SourceGLWidget::useBinaryTcpChannel(" + debug_.boolToStr( b ) + ")\n" );
    coreVision_.setUseBinaryTcpChannel( b );
}

/***************************************************************************//**
If arg is true, this slot calls on CoreVision to use /tuio/2Dcur profile
for the TUIO UDP channel.
*******************************************************************************/
void SourceGLWidget::setTuioUdpProfileToTuio2Dcur( bool b )
{
    if( b ) {
        setTuioUdpProfile( blobEnums::TuioProfileEnum::TUIO_2D_CURSOR );
    }
}

/***************************************************************************//**
If arg is true, this slot calls on CoreVision to use /tuio/2Dblb profile
for the TUIO UDP channel.
*******************************************************************************/
void SourceGLWidget::setTuioUdpProfileToTuio2Dblb( bool b )
{
    if( b ) {
        setTuioUdpProfile( blobEnums::TuioProfileEnum::TUIO_2D_BLOB );
    }
}

/***************************************************************************//**
If arg is true, this slot calls on CoreVision to use /tuio/2Dblb profile
for the TUIO UDP channel.
*******************************************************************************/
void SourceGLWidget::setTuioUdpProfileToCurAndBlb( bool b )
{
    if( b ) {
        setTuioUdpProfile( blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB );
    }
}

/***************************************************************************//**
Calls on the CoreVision class to set the TUIO profile used for TUIO UDP.
*******************************************************************************/
void SourceGLWidget::setTuioUdpProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    coreVision_.setTuioUdpProfile( profile );
}

/***************************************************************************//**
If arg is true, this slot calls on CoreVision to use the /tuio/2Dcur profile
for Flash XML.
*******************************************************************************/
void SourceGLWidget::setFlashXmlProfileToTuio2Dcur( bool b )
{
    if( b ) {
        setFlashXmlProfile( blobEnums::TuioProfileEnum::TUIO_2D_CURSOR );
    }
}

/***************************************************************************//**
If arg is true, this slot calls on CoreVision to use /tuio/2Dblb profile
for Flash XML.
*******************************************************************************/
void SourceGLWidget::setFlashXmlProfileToTuio2Dblb( bool b )
{
    if( b ) {
        setFlashXmlProfile( blobEnums::TuioProfileEnum::TUIO_2D_BLOB );
    }
}

/***************************************************************************//**
If arg is true, this slot calls on CoreVision to use /tuio/2Dblb profile
for Flash XML.
*******************************************************************************/
void SourceGLWidget::setFlashXmlProfileToCurAndBlb( bool b )
{
    if( b ) {
        setFlashXmlProfile( blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB );
    }
}

/***************************************************************************//**
Calls on the CoreVision class to set the TUIO profile used for Flash XML.
*******************************************************************************/
void SourceGLWidget::setFlashXmlProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    coreVision_.setFlashXmlProfile( profile );
}

/***************************************************************************//**
Slot to call on CoreVision function of the same name.  This function only has 
an affect the first time it is called.  The TuioChannels object will ignore 
any subsequent calls because it does not have a means to shut down the UDP and 
TCP servers and clean up memory before doing setup a second time.
*******************************************************************************/
void SourceGLWidget::setupTuio( const std::string & localHost,
                                int tuioUdpChannelOnePort,
                                int tuioUdpChannelTwoPort,
                                int flashXmlChannelPort,
                                int binaryTcpChannelPort )
{
    debugPrintSetUpTuio( localHost, 
                         tuioUdpChannelOnePort, 
                         tuioUdpChannelTwoPort, 
                         flashXmlChannelPort,
                         binaryTcpChannelPort );
    coreVision_.setupTuio( localHost, 
                           tuioUdpChannelOnePort, 
                           tuioUdpChannelTwoPort, 
                           flashXmlChannelPort,
                           binaryTcpChannelPort );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Protected Functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Initializes the OpenGL state, including setting the background color and loading
the font that will be used for blob ID numbers.
*******************************************************************************/
void SourceGLWidget::initializeGL()
{
    qglClearColor( backgroundColor_ );
    glShadeModel( GL_FLAT );
    coreVision_.loadFontForBlobIDNumbers();
}

/***************************************************************************//**
This function will check whether the aspect ratio of the viewport matches the
aspect ratio of the camera Source image, which should be 4:3 for a 320 x 240 
pixel VGA image (or a 640 x 480 pixel higher resolution image).  If the aspect
ratio does not match, the image will not be stretched.  Rather, the largest
correct aspect 4:3 image that can be centered on the widget surface, and the
extra area will be painted as the same background color of the window.  
<br />

WARNING:<br />
Trying an alternate approach such as calling on resize(w,h) inside of resizeGL()
would be a very bad idea, as that would lead to an infinite recursion.
*******************************************************************************/
void SourceGLWidget::resizeGL( int viewportWidth, int viewportHeight )
{
    viewportWidth_ = viewportWidth;
    viewportHeight_ = viewportHeight;
    videoWidth_ = coreVision_.getVideoWidth();
    videoHeight_ = coreVision_.getVideoHeight();
    float videoAspect = (float)videoWidth_ / videoHeight_,
          viewportAspect = (float)viewportWidth_ / viewportHeight_;

    //debugPrintResizeGL( viewportWidth_, viewportHeight_, videoWidth_, videoHeight_ );
    glViewport( 0, 0, viewportWidth_, viewportHeight_ );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    if( viewportAspect > videoAspect ) {
        // Viewport is too wide, so need some left/right padding (on x-axis)
        int biggerWidth = (int)((viewportAspect / videoAspect) * videoWidth_),
            adjustment = (biggerWidth - videoWidth_) / 2;
        glOrtho( -adjustment, videoWidth_ + adjustment, 0, videoHeight_, -1.0, 1.0 );
    }
    else if( videoAspect > viewportAspect ) {
        // Viewport is too tall, so need some top/bottom padding (on y-axis).
        int biggerHeight = (int)((videoAspect / viewportAspect) * videoHeight_),
            adjustment = (biggerHeight - videoHeight_) / 2;
        glOrtho( 0, videoWidth_, -adjustment, videoHeight_ + adjustment, -1.0, 1.0 );
    }
    else { // Aspect ratios are equal.
        glOrtho( 0, videoWidth_, 0, videoHeight_, -1.0, 1.0 );
    }
    glMatrixMode( GL_MODELVIEW );
}

/***************************************************************************//**
Calls on the CoreVision object to check if there is a new image from the camera.
If so, it will be processed for blob detection and then the Source image will
be rendered on this widget.<br />

If new program settings have been loaded from an XML file, this function will
reset the CoreVison object.

WARNING:<br />
This function should never be called directly.  Call on frame(), which will use
updateGL() to set up the OpenGL context before calling paintGL() in its own
time (no blocking).
*******************************************************************************/
void SourceGLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( initializeCoreVision_ ) {
        intitializeCoreVision(); // Use copy of validator to update CoreVision object.
        initializeCoreVision_ = false;
    }
    else if( updateCoreVision_ ) {
        resetCoreVision(); // Use copy of validator to update CoreVision object.
        updateCoreVision_ = false;
    }
    coreVision_.update();

    if( isVisible() && !isMinimized() ) {
        drawSourceImage();
        recordSourceImageIfRequested();
    }
}

/*******************************************************************************
Obtains a copy of the source image from the ImageFilters object and renders it.
Then it has to call on the CoreVision object to render blob outlines if they
have been requested.  Eventually, the blob outline info should be handed over
to this class, which should do all of the OpenGL rendering.
*******************************************************************************/
void SourceGLWidget::drawSourceImage()
{
    blobBuffers::GrayBuffer * grayBuffer = NULL;
    grayBuffer = imageFilters_->grayBuffer( FilterEnum::SOURCE );

    if( grayBuffer != NULL ) {
        if( cameraSizeHasChanged( grayBuffer ) ) {
            resizeGL( viewportWidth_, viewportHeight_ );
        }
        if( showHelpImage_ ) {
            drawHelpImage( grayBuffer->width(), grayBuffer->height() );
        }
        else {
            drawSourceImage( grayBuffer );
        }
    }
}

/*******************************************************************************
If the video player is used and it has a different size than the camera, the
resizeGL() function will need to be called again.
*******************************************************************************/
bool SourceGLWidget::cameraSizeHasChanged( blobBuffers::GrayBuffer * grayBuffer )
{
    return (grayBuffer->width() != videoWidth_) 
        && (grayBuffer->height() != videoHeight_);
}

/*******************************************************************************
Draws an image containing help instructions for this filter.  Texture mapping 
is used only to be consistent with how the filter images are rendered.
*******************************************************************************/
void SourceGLWidget::drawHelpImage( int width, int height )
{
    if( helpTextureName_ == 0 && !helpImage_.isNull() ) {
        helpTextureName_ = bindTexture( helpImage_ );
    }
    if( helpTextureName_ != 0 ) {
        glMatrixMode( GL_MODELVIEW );
        glEnable( GL_TEXTURE_2D );
            glBindTexture( GL_TEXTURE_2D, helpTextureName_ );
            glBegin( GL_QUADS );
                glTexCoord2f( 0, 0 );
                glVertex2f( 0, 0 );
                glTexCoord2f( 1.0f, 0 );
                glVertex2f( width, 0 );
                glTexCoord2f( 1.0f, 1.0f );
                glVertex2f( width, height );
                glTexCoord2f( 0, 1.0f );
                glVertex2f( 0, height );
            glEnd();
        glDisable( GL_TEXTURE_2D );
    }
}

/*******************************************************************************
Rotates and translates the image to be drawn to account for OpenGL using 
positive y as upwards, while Qt widgets use positive y as downwards.
*******************************************************************************/
void SourceGLWidget::drawSourceImage( blobBuffers::GrayBuffer * grayBuffer )
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
        glTranslatef( 0.0f, coreVision_.getVideoHeight(), 0.0f );
        glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );
        drawTexture( grayBuffer );
        coreVision_.drawCalibrationWireframe();
        coreVision_.drawCalibrationPointsWithIntensities();
        coreVision_.drawBlobOutlines();
    glPopMatrix();
}

/*******************************************************************************
Uses an OpenGL Texture object to render the GrayBuffer.
*******************************************************************************/
void SourceGLWidget::drawTexture( blobBuffers::GrayBuffer * grayBuffer )
{
    int bufferWidth = grayBuffer->width(),
        bufferHeight = grayBuffer->height();

    if( bufferWidth != textureWidth_ || bufferHeight != textureHeight_ ) {
        initializeTexture( bufferWidth, bufferHeight );
    }
    glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureName_ );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, bufferWidth, bufferHeight,
                         GL_LUMINANCE, GL_UNSIGNED_BYTE, grayBuffer->pixels() );

        glBegin( GL_QUADS );
            glTexCoord2f( 0, 0 );
            glVertex2f( 0, 0 );
            glTexCoord2f( 1.0f, 0 );
            glVertex2f( bufferWidth, 0 );
            glTexCoord2f( 1.0f, 1.0f );
            glVertex2f( bufferWidth, bufferHeight );
            glTexCoord2f( 0, 1.0f );
            glVertex2f( 0, bufferHeight );
        glEnd();

    glDisable( GL_TEXTURE_2D );
}

/*******************************************************************************
Initializes the texture map that will be used for rendering a GrayBuffer.
*******************************************************************************/
void SourceGLWidget::initializeTexture( int width, int height )
{
    textureWidth_ = width;
    textureHeight_ = height;
    glDeleteTextures( 1, &textureName_ );
    glGenTextures( 1, &textureName_ );

    glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureName_ );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, 
                      textureWidth_, textureHeight_, 0, 
                      GL_LUMINANCE, GL_UNSIGNED_BYTE, 0 );

        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glDisable( GL_TEXTURE_2D );
}

void SourceGLWidget::recordSourceImageIfRequested()
{
    if( recordSourceImages_ ) {
        int w = width(),
            h = height(),
            bytesPerLine = w * 3,
            numBytes = bytesPerLine * h;
        unsigned char * pixels = new unsigned char[numBytes];
        glPixelStorei( GL_PACK_ALIGNMENT, 1 );
        glPixelStorei( GL_PACK_ROW_LENGTH, w );
        glReadPixels( 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, pixels );
        bool success = videoWriter_->processRgbImage( pixels, w, h, bytesPerLine );

        if( !success ) {
            recordSourceImages_ = false;
            emit writeColorVideoFailed();
        }
        delete pixels;
    }
}

/*******************************************************************************
This helper function for the constructor obtains the background color of the
window and stores it for use by the OpenGL rendering code.

If the user resizes this widget to be something other than the 4:3 aspect ratio 
for a VGA image, the image will still be rendered with a 4:3 aspect, and any 
left over area will be filled with the background color.  This approach was
used because Qt does not have any built-in way of restricting the aspect ratio
of a QGLWidget as it is resized.
*******************************************************************************/
void SourceGLWidget::initBackgroundColor()
{
    QPalette pal = palette();
    backgroundColor_ = pal.color( QPalette::Window );
    debugPrintBackgroundColor();
}

/*******************************************************************************
If debugging is turned on for this class, writes the background color to 
standard out.
*******************************************************************************/
void SourceGLWidget::debugPrintBackgroundColor()
{
    debug_.printLn( "\nSourceGLWidget rgb = ("
        + debug_.intToStr( backgroundColor_.red() ) + ", " 
        + debug_.intToStr( backgroundColor_.green() ) + ", " 
        + debug_.intToStr( backgroundColor_.blue() ) + ")\n" );
}

/*******************************************************************************
If debugging is turned on for this class, writes the width and height of the
viewport and the width and height of the video camera.
*******************************************************************************/
void SourceGLWidget::debugPrintResizeGL( int viewportWidth, int viewportHeight, 
                                       int videoWidth, int videoHeight )
{
    debug_.printLn( "\n***** SourceGLWidget::resizeGL(" 
        + debug_.intToStr( viewportWidth ) + ", " 
        + debug_.intToStr( viewportHeight ) + ") *****\n"
        + "videoWidth = " + debug_.intToStr( videoWidth ) + "\n"
        + "videoHeight = " + debug_.intToStr( videoHeight ) + "\n" );
}

/*******************************************************************************
If debugging is turned on for this class, writes info on the TUIO set up.
*******************************************************************************/
void SourceGLWidget::debugPrintSetUpTuio( const std::string & localHost,
                                        int tuioUdpChannelOnePort,
                                        int tuioUdpChannelTwoPort,
                                        int flashXmlChannelPort,
                                        int binaryTcpChannelPort )
{
    debug_.printLn( "\nSourceGLWidget::setupTuio(" + localHost + ", " 
                    + debug_.intToStr( tuioUdpChannelOnePort ) + ", " 
                    + debug_.intToStr( tuioUdpChannelTwoPort ) + ", " 
                    + debug_.intToStr( flashXmlChannelPort ) + ", "
                    + debug_.intToStr( binaryTcpChannelPort ) + ")\n" );
}

/***************************************************************************//**
TEMPORARY CODE:  This function and the related functions below are a temporary
convenience for working out how many frames an unmatched blob should persist
before it is killed off in the the BlobTracker::track() function.
*******************************************************************************/
void SourceGLWidget::setUnmatchedBlobFramesToSkip( int frames )
{
    coreVision_.setUnmatchedBlobFramesToSkip( frames );
}

void SourceGLWidget::setUnmatchedBlobZeroFramesToSkip( bool b )   { if( b ) { setUnmatchedBlobFramesToSkip( 0 ); } }
void SourceGLWidget::setUnmatchedBlobFiveFramesToSkip( bool b )   { if( b ) { setUnmatchedBlobFramesToSkip( 5 ); } }
void SourceGLWidget::setUnmatchedBlobTenFramesToSkip( bool b )    { if( b ) { setUnmatchedBlobFramesToSkip( 10 ); } }
void SourceGLWidget::setUnmatchedBlobTwentyFramesToSkip( bool b ) { if( b ) { setUnmatchedBlobFramesToSkip( 20 ); } }
