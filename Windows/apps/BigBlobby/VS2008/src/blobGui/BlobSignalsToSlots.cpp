////////////////////////////////////////////////////////////////////////////////
// BlobSignalsToSlots
//
// PURPOSE: Connects signals from GUI components with slots on BlobMainWindow or
//          one of BlobMainWindow's helper objects.
//
// CREATED:  5/17/2011
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
#include "blobGui/BlobSignalsToSlots.h"
#include "blobGui/BlobMainWindow.h"
#include "blobGui/BlobMainWindowView.h"
#include "blobGui/SourceGLWidget.h"
#include "blobGui/CalibrationGLWidget.h"
#include "blobGui/BlobXmlSettings.h"
#include "blobGui/Help.h"
#include "blobGui/CustomZoomDialog.h"
#include "ui_CustomZoomDialog.h"
#include "ui_BlobMainWindow.h"

using blobGui::BlobSignalsToSlots;

/***************************************************************************//**
The constructor stores a reference to the BlobMainWindow along with references 
to several of BlobMainWindow's helper classes.

All of the connections are made later when makeConnections() is called.
*******************************************************************************/
BlobSignalsToSlots::BlobSignalsToSlots( blobGui::BlobMainWindow * blobMainWindow ) :
  blobMainWindow_( blobMainWindow ),
  mainWindowUi_( blobMainWindow->mainWindowUi() ),
  zoomDialogUi_( blobMainWindow->zoomDialogUi() ),
  zoomDialog_( blobMainWindow->zoomDialog() ),
  blobMainWindowView_( blobMainWindow->blobMainWindowView() ),
  sourceGLWidget_( blobMainWindow->sourceGLWidget() ),
  calibrationGLWidget_( blobMainWindow->calibrationGLWidget() ),
  blobXmlSettings_( blobMainWindow_->blobXmlSettings() )
{
}

BlobSignalsToSlots::~BlobSignalsToSlots()
{
}

/***************************************************************************//**
Makes nearly all of the signals to slots connections used in this program.

One minor exception is that the BigBlobby class uses a signal to slot connection
for the timer that runs the frame calls.
<br />

The helper functions for makeConnections() are generally organized by some type
of GUI grouping such as all the options in a menu or all of the controls on
a major panel.  These helper functions will also take care of synchronizing
spin boxes with sliders wherever that is needed.
*******************************************************************************/
void BlobSignalsToSlots::makeConnections()
{
    connectMenuBar();
    connectMainPanel();
    connectFiltersPanel();
    connectCalibrationScreen();
    connectCustomZoomDialog();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                               MenuBar Connections
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BlobSignalsToSlots::connectMenuBar()
{
    connectFileMenu();
    connectViewMenu();
    connectCameraMenu();
    connectBlobsMenu();
    connectNetworkMenu();
    connectHelpMenu();
    connectExperimentalMenu();
}

void BlobSignalsToSlots::connectFileMenu()
{
    connect( mainWindowUi_->action_saveSettings, 
             SIGNAL( triggered() ),
             blobMainWindow_, 
             SLOT( saveGuiAndCameraSettingsToXmlFiles() ) );

    connect( mainWindowUi_->action_quit, 
             SIGNAL( triggered() ),
             QApplication::instance(), 
             SLOT( closeAllWindows() ) );

    connect( mainWindowUi_->action_useStartupSettings, 
             SIGNAL( triggered() ),
             blobXmlSettings_, 
             SLOT( restoreStartupSettings() ) );

    connect( mainWindowUi_->action_useFactoryDefaults, 
             SIGNAL( triggered() ),
             blobXmlSettings_, 
             SLOT( loadFactorySettings() ) );
}

void BlobSignalsToSlots::connectViewMenu()
{
    connect( mainWindowUi_->action_view_minimalWindow, 
             SIGNAL( triggered() ),
             blobMainWindowView_, 
             SLOT( setViewToMinimalWindow() ) );

    connect( mainWindowUi_->action_view_defaultWindow, 
             SIGNAL( triggered() ),
             blobMainWindowView_, 
             SLOT( setViewToDefault() ) );

    connect( mainWindowUi_->action_zoom100, 
             SIGNAL( triggered() ),
             blobMainWindowView_, 
             SLOT( zoomMainPanelImages100() ) );

    connect( mainWindowUi_->action_zoom200, 
             SIGNAL( triggered() ),
             blobMainWindowView_, 
             SLOT( zoomMainPanelImages200() ) );

    connect( mainWindowUi_->action_zoom400, 
             SIGNAL( triggered() ),
             blobMainWindowView_, 
             SLOT( zoomMainPanelImages400() ) );

    connect( mainWindowUi_->action_zoomCustom, 
             SIGNAL( triggered() ),
             blobMainWindow_, 
             SLOT( openCustomZoomDialog() ) );
}

void BlobSignalsToSlots::connectCameraMenu()
{
    connect( mainWindowUi_->action_openCameraSettings,
             SIGNAL( triggered() ),
             sourceGLWidget_,
             SLOT( openCameraSettings() ) );

    connect( mainWindowUi_->action_saveScreenshot,
             SIGNAL( triggered() ),
             blobMainWindow_,
             SLOT( saveScreenshot() ) );

    connect( mainWindowUi_->action_openTestVideo,
             SIGNAL( triggered() ),
             blobMainWindow_,
             SLOT( playTestVideo() ) );

    connect( mainWindowUi_->action_stopTestVideo,
             SIGNAL( triggered() ),
             blobMainWindow_,
             SLOT( stopTestVideo() ) );

    connect( mainWindowUi_->action_recordSourceInputVideo,
             SIGNAL( triggered() ),
             blobMainWindow_,
             SLOT( startSourceInputRecording() ) );

    connect( mainWindowUi_->action_recordSourceColorVideo,
             SIGNAL( triggered() ),
             blobMainWindow_,
             SLOT( startSourceColorRecording() ) );

    connect( mainWindowUi_->action_stopRecording,
             SIGNAL( triggered() ),
             blobMainWindow_,
             SLOT( stopVideoWriter() ) );

    connect( sourceGLWidget_,
             SIGNAL( writeColorVideoFailed() ),
             blobMainWindow_,
             SLOT( onWriteColorVideoFailed() ) );

    connect( mainWindowUi_->action_flipImageHorizontal,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( flipImageHorizontal( bool ) ) );

    connect( mainWindowUi_->action_flipImageVertical,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( flipImageVertical( bool ) ) );
}

void BlobSignalsToSlots::connectBlobsMenu()
{
    connect( mainWindowUi_->action_showBlobAreas,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showBlobAreas( bool ) ) );

    connect( mainWindowUi_->action_showBlobOutlines,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showBlobOutlines( bool ) ) );

    connect( mainWindowUi_->action_showBlobCrosshairs,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showBlobCrosshairs( bool ) ) );

    connect( mainWindowUi_->action_showBlobBoundingBox,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showBlobBoundingBoxes( bool ) ) );

    connect( mainWindowUi_->action_showIdNumbers,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showBlobLabels( bool ) ) );

    connect( mainWindowUi_->action_showCalibrationWireframe,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showCalibrationWireframe( bool ) ) );

    connect( mainWindowUi_->action_showCalibrationBlobIntensities,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showCalibrationIntensities( bool ) ) );

    connect( mainWindowUi_->action_normalizeBlobIntensities,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( correctForSignalVariation( bool ) ) );

    connect( mainWindowUi_->action_trackDarkBlobs,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useDarkBlobs( bool ) ) );
}

void BlobSignalsToSlots::connectNetworkMenu()
{
    connect( mainWindowUi_->action_silentMode,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( setNetworkSilentMode( bool ) ) );

    connectTuioUdpSubmenu();
    connectFlashXmlSubmenu();
  
    connect( mainWindowUi_->action_useBinaryTcpChannel,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useBinaryTcpChannel( bool ) ) );
}

void BlobSignalsToSlots::connectTuioUdpSubmenu()
{
    connect( mainWindowUi_->action_useTuioUdpChannel_1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useTuioUdpChannelOne( bool ) ) );

    connect( mainWindowUi_->action_useTuioUdpChannel_2,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useTuioUdpChannelTwo( bool ) ) );

    connect( mainWindowUi_->action_useTuio2DcurProfile,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( setTuioUdpProfileToTuio2Dcur( bool ) ) );

    connect( mainWindowUi_->action_useTuio2DblbProfile,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( setTuioUdpProfileToTuio2Dblb( bool ) ) );

    connect( mainWindowUi_->action_useCurAndBlbProfile,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( setTuioUdpProfileToCurAndBlb( bool ) ) );

    connect( mainWindowUi_->checkBox_silentMode,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( setNetworkSilentMode( bool ) ) );
}

void BlobSignalsToSlots::connectFlashXmlSubmenu()
{
    connect( mainWindowUi_->action_useFlashXmlChannel_1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useFlashXmlChannel( bool ) ) );

    connect( mainWindowUi_->action_useFlashXmlTuio2DcurProfile,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( setFlashXmlProfileToTuio2Dcur( bool ) ) );

    connect( mainWindowUi_->action_useFlashXmlTuio2DblbProfile,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( setFlashXmlProfileToTuio2Dblb( bool ) ) );

    connect( mainWindowUi_->action_useFlashXmlCurAndBlbProfile,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( setFlashXmlProfileToCurAndBlb( bool ) ) );
}

void BlobSignalsToSlots::connectHelpMenu()
{
    connect( mainWindowUi_->action_help_about,
             SIGNAL( triggered() ),
             blobMainWindow_->help(),
             SLOT( openAboutBox() ) );

    connect( mainWindowUi_->action_help_online,
             SIGNAL( triggered() ),
             blobMainWindow_->help(),
             SLOT( openOnlineHelp() ) );

    connect( mainWindowUi_->action_infoOpenGL,
             SIGNAL( triggered() ),
             blobMainWindow_->help(),
             SLOT( openOpenGLInfoDialogBox() ) );
}

void BlobSignalsToSlots::connectExperimentalMenu()
{
    connect( mainWindowUi_->action_deathCount_0, SIGNAL( toggled( bool ) ),
             sourceGLWidget_, SLOT( setUnmatchedBlobZeroFramesToSkip( bool ) ) );

    connect( mainWindowUi_->action_deathCount_5, SIGNAL( toggled( bool ) ),
             sourceGLWidget_, SLOT( setUnmatchedBlobFiveFramesToSkip( bool ) ) );

    connect( mainWindowUi_->action_deathCount_10, SIGNAL( toggled( bool ) ),
             sourceGLWidget_, SLOT( setUnmatchedBlobTenFramesToSkip( bool ) ) );

    connect( mainWindowUi_->action_deathCount_20, SIGNAL( toggled( bool ) ),
             sourceGLWidget_, SLOT( setUnmatchedBlobTwentyFramesToSkip( bool ) ) );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                            Main Panel Connections
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BlobSignalsToSlots::connectMainPanel()
{
    connectMainPanelButtons();
    connectImageThresholdControls();
    connectMinBlobSizeControls();
    connectMaxBlobSizeControls();
}

void BlobSignalsToSlots::connectMainPanelButtons()
{
    connect( mainWindowUi_->pushButton_subtractBackground,
             SIGNAL( pressed() ),
             sourceGLWidget_,
             SLOT( learnBackground() ) );

    connect( mainWindowUi_->pushButton_cameraSettings,
             SIGNAL( pressed() ),
             sourceGLWidget_,
             SLOT( openCameraSettings() ) );

    connect( mainWindowUi_->pushButton_enterCalibration,
             SIGNAL( pressed() ),
             blobMainWindow_,
             SLOT( openCalibrationScreen() ) );

    connect( mainWindowUi_->checkBox_sourceHelp,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showSourceHelp( bool ) ) );
}

void BlobSignalsToSlots::connectImageThresholdControls()
{
    connect( mainWindowUi_->spinBox_imageThreshold,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setImageThreshold( int ) ) );

    connect( mainWindowUi_->checkBox_thresholdHelp,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showThresholdHelp( bool ) ) );

    synchronizeSliderAndSpinBox( mainWindowUi_->slider_imageThreshold, 
                                 mainWindowUi_->spinBox_imageThreshold );
}

void BlobSignalsToSlots::connectMinBlobSizeControls()
{
    connect( mainWindowUi_->spinBox_minBlobSize,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setMinBlobSize( int ) ) );

    synchronizeSliderAndSpinBox( mainWindowUi_->slider_minBlobSize, 
                             mainWindowUi_->spinBox_minBlobSize );
}

void BlobSignalsToSlots::connectMaxBlobSizeControls()
{
    connect( mainWindowUi_->spinBox_maxBlobSize,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setMaxBlobSize( int ) ) );

    synchronizeSliderAndSpinBox( mainWindowUi_->slider_maxBlobSize, 
                             mainWindowUi_->spinBox_maxBlobSize );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                            Filters Panel Connections
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BlobSignalsToSlots::connectFiltersPanel()
{
    connectPreBkSmoothingControls();
    connectMaskControls();
    connectBackgroundControls();
    connectHighpassControls();
    connectHighpassAmplifyControls();
    connectHighpassTwoControls();
    connectHighpassTwoAmplifyControls();
    connectSmoothingControls();
    connectSmoothingAmplifyControls();
}

void BlobSignalsToSlots::connectPreBkSmoothingControls()
{
    connect( mainWindowUi_->checkBox_preBkSmoothing1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( usePreBkSmoothing( bool ) ) );

    connect( mainWindowUi_->checkBox_gaussianPreBkSmoothing1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useGaussianPreBkSmoothing( bool ) ) );

    connect( mainWindowUi_->spinBox_preBkSmoothingKernel1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setPreBkSmoothingBlur( int ) ) );

    connect( mainWindowUi_->checkBox_preBkSmoothingHelp1,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showPreBkSmoothingHelp1( bool ) ) );

    connect( mainWindowUi_->checkBox_preBkSmoothingHelp2,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showPreBkSmoothingHelp2( bool ) ) );

    connect( mainWindowUi_->doubleSpinBox_preBkSmoothingSigma1,
             SIGNAL( valueChanged( double ) ),
             sourceGLWidget_,
             SLOT( setGaussianPreBkSmoothingSigma( double ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_preBkSmoothing1, 
                                    mainWindowUi_->checkBox_preBkSmoothing2 );
    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_gaussianPreBkSmoothing1, 
                                    mainWindowUi_->checkBox_gaussianPreBkSmoothing2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_preBkSmoothingKernel1, 
                                  mainWindowUi_->spinBox_preBkSmoothingKernel2 );
    synchronizeDblSpinBoxAndDblSpinBox( mainWindowUi_->doubleSpinBox_preBkSmoothingSigma1,
                                        mainWindowUi_->doubleSpinBox_preBkSmoothingSigma2 );

    QCheckBox * box = mainWindowUi_->checkBox_preBkSmoothing1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_gaussianPreBkSmoothing1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_gaussianPreBkSmoothing2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_preBkSmoothingKernel1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_preBkSmoothingKernel2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->doubleSpinBox_preBkSmoothingSigma1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->doubleSpinBox_preBkSmoothingSigma2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_preBkSmoothingKernel1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_preBkSmoothingKernel2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_preBkSmoothingSigma1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_preBkSmoothingSigma2 );
}

void BlobSignalsToSlots::connectMaskControls()
{
    connect( mainWindowUi_->checkBox_maskFilter1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useMask( bool ) ) );

    connect( mainWindowUi_->spinBox_maskPadX1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setMaskWidthPadding( int ) ) );

    connect( mainWindowUi_->spinBox_maskPadY1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setMaskHeightPadding( int ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_maskFilter1, 
                                    mainWindowUi_->checkBox_maskFilter2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_maskPadX1, 
                                  mainWindowUi_->spinBox_maskPadX2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_maskPadY1, 
                                  mainWindowUi_->spinBox_maskPadY2 );

    QCheckBox * box = mainWindowUi_->checkBox_maskFilter1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_maskPadX1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_maskPadY1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_maskPadX2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_maskPadY2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maskPadX1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maskPadY1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maskPadX2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maskPadY2 );
}

void BlobSignalsToSlots::connectBackgroundControls()
{
    connect( mainWindowUi_->pushButton_subtractBackground1,
             SIGNAL( pressed() ),
             sourceGLWidget_,
             SLOT( learnBackground() ) );

    connect( mainWindowUi_->pushButton_subtractBackground2,
             SIGNAL( pressed() ),
             sourceGLWidget_,
             SLOT( learnBackground() ) );
    
    connect( mainWindowUi_->groupBox_dynamicSubtration,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useAutoBackground( bool ) ) );

    connect( mainWindowUi_->spinBox_dynamicSubtraction,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setBackgroundLearnRate( int ) ) );

    connect( mainWindowUi_->groupBox_periodicSubtract,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( usePeriodicBackground( bool ) ) );

    connect( mainWindowUi_->spinBox_periodicSubtractSeconds,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setPeriodicBackgroundSeconds( int ) ) );

    connect( mainWindowUi_->spinBox_periodicSubtractPercent,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setPeriodicBackgroundPercent( int ) ) );

    connect( mainWindowUi_->checkBox_backgroundHelp1,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showBackgroundHelp1( bool ) ) );

    connect( mainWindowUi_->checkBox_backgroundHelp2,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showBackgroundHelp2( bool ) ) );

    synchronizeSliderAndSpinBox( mainWindowUi_->slider_dynamicSubtraction,
                                 mainWindowUi_->spinBox_dynamicSubtraction );

}

void BlobSignalsToSlots::connectHighpassControls()
{
    connect( mainWindowUi_->checkBox_highpass1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useHighpass( bool ) ) );

    connect( mainWindowUi_->spinBox_highpass1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setHighpassBlur( int ) ) );

    connect( mainWindowUi_->checkBox_highpass1Help1,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showHighpass1Help1( bool ) ) );

    connect( mainWindowUi_->checkBox_highpass1Help2,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showHighpass1Help2( bool ) ) );

    connect( mainWindowUi_->checkBox_highpass1ShowImageToSubtract1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showHighpassSubtractionImage( bool ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_highpass1,
                                    mainWindowUi_->checkBox_highpass2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_highpass1,
                                  mainWindowUi_->spinBox_highpass2 );
    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_highpass1ShowImageToSubtract1,
                                    mainWindowUi_->checkBox_highpass1ShowImageToSubtract2 );

    QCheckBox * box = mainWindowUi_->checkBox_highpass1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpass1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpass2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_highpassKernel1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_highpassKernel2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_highpass1ShowImageToSubtract1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_highpass1ShowImageToSubtract2 );
}

void BlobSignalsToSlots::connectHighpassAmplifyControls()
{
    connect( mainWindowUi_->checkBox_highpassAmplify1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useHighpassAmplify( bool ) ) );

    connect( mainWindowUi_->spinBox_highpassAmplify1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setHighpassAmplifyLevel( int ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_highpassAmplify1, 
                                    mainWindowUi_->checkBox_highpassAmplify2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_highpassAmplify1, 
                                  mainWindowUi_->spinBox_highpassAmplify2 );
    synchronizeSliderAndSpinBox( mainWindowUi_->slider_highpassAmplify1,
                                 mainWindowUi_->spinBox_highpassAmplify1 );
    synchronizeSliderAndSpinBox( mainWindowUi_->slider_highpassAmplify2,
                                 mainWindowUi_->spinBox_highpassAmplify2 );

    QCheckBox * box = mainWindowUi_->checkBox_highpassAmplify1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->slider_highpassAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->slider_highpassAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpassAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpassAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_minHigpassAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maxHighpassAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_minHigpassAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maxHigpassAmplify2 );
}

void BlobSignalsToSlots::connectHighpassTwoControls()
{
    connect( mainWindowUi_->checkBox_highpassTwoFilter1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useHighpassTwo( bool ) ) );

    connect( mainWindowUi_->spinBox_highpassTwoFilter1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setHighpassTwoKernel( int ) ) );

    connect( mainWindowUi_->checkBox_highpass2Help1,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showHighpass2Help1( bool ) ) );

    connect( mainWindowUi_->checkBox_highpass2Help2,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showHighpass2Help2( bool ) ) );

    connect( mainWindowUi_->checkBox_highpassTwoShowImageToSubtract1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( showHighpassTwoSubtractionImage( bool ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_highpassTwoFilter1, 
                                    mainWindowUi_->checkBox_highpassTwoFilter2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_highpassTwoFilter1, 
                                  mainWindowUi_->spinBox_highpassTwoFilter2 );
    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_highpassTwoShowImageToSubtract1, 
                                    mainWindowUi_->checkBox_highpassTwoShowImageToSubtract2 );

    QCheckBox * box = mainWindowUi_->checkBox_highpassTwoFilter1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpassTwoFilter1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpassTwoFilter2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_highpassTwoKernel1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_highpassTwoKernel2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_highpassTwoShowImageToSubtract1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_highpassTwoShowImageToSubtract2 );
}

void BlobSignalsToSlots::connectHighpassTwoAmplifyControls()
{
    connect( mainWindowUi_->checkBox_highpassTwoAmplify1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useHighpassTwoAmplify( bool ) ) );

    connect( mainWindowUi_->spinBox_highpassTwoAmplify1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setHighpassTwoAmplifyLevel( int ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_highpassTwoAmplify1, 
                                    mainWindowUi_->checkBox_highpassTwoAmplify2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_highpassTwoAmplify1, 
                                  mainWindowUi_->spinBox_highpassTwoAmplify2 );
    synchronizeSliderAndSpinBox( mainWindowUi_->slider_highpassTwoAmplify1,
                                 mainWindowUi_->spinBox_highpassTwoAmplify1 );
    synchronizeSliderAndSpinBox( mainWindowUi_->slider_highpassTwoAmplify2,
                                 mainWindowUi_->spinBox_highpassTwoAmplify2 );

    QCheckBox * box = mainWindowUi_->checkBox_highpassTwoAmplify1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->slider_highpassTwoAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->slider_highpassTwoAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpassTwoAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_highpassTwoAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_minHighpassTwoAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maxHighpassTwoAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_minHighpassTwoAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maxHighpassTwoAmplify2 );
}

void BlobSignalsToSlots::connectSmoothingControls()
{
    connect( mainWindowUi_->checkBox_smoothing1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useSmoothing( bool ) ) );

    connect( mainWindowUi_->checkBox_gaussianSmoothing1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useGaussianSmoothing( bool ) ) );

    connect( mainWindowUi_->spinBox_smoothingKernel1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setSmoothingBlur( int ) ) );

    connect( mainWindowUi_->checkBox_smoothingHelp1,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showSmoothingHelp1( bool ) ) );

    connect( mainWindowUi_->checkBox_smoothingHelp2,
             SIGNAL( toggled( bool ) ),
             blobMainWindow_,
             SLOT( showSmoothingHelp2( bool ) ) );

    connect( mainWindowUi_->doubleSpinBox_smoothingSigma1,
             SIGNAL( valueChanged( double ) ),
             sourceGLWidget_,
             SLOT( setGaussianSmoothingSigma( double ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_smoothing1, 
                                    mainWindowUi_->checkBox_smoothing2 );
    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_gaussianSmoothing1, 
                                    mainWindowUi_->checkBox_gaussianSmoothing2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_smoothingKernel1, 
                                  mainWindowUi_->spinBox_smoothingKernel2 );
    synchronizeDblSpinBoxAndDblSpinBox( mainWindowUi_->doubleSpinBox_smoothingSigma1,
                                        mainWindowUi_->doubleSpinBox_smoothingSigma2 );

    QCheckBox * box = mainWindowUi_->checkBox_smoothing1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_gaussianSmoothing1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->checkBox_gaussianSmoothing2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_smoothingKernel1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_smoothingKernel2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->doubleSpinBox_smoothingSigma1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->doubleSpinBox_smoothingSigma2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_smoothingKernel1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_smoothingKernel2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_smoothingSigma1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_smoothingSigma2 );
}

void BlobSignalsToSlots::connectSmoothingAmplifyControls()
{
    connect( mainWindowUi_->checkBox_smoothingAmplify1,
             SIGNAL( toggled( bool ) ),
             sourceGLWidget_,
             SLOT( useSmoothingAmplify( bool ) ) );

    connect( mainWindowUi_->spinBox_smoothingAmplify1,
             SIGNAL( valueChanged( int ) ),
             sourceGLWidget_,
             SLOT( setSmoothingAmplifyLevel( int ) ) );

    synchronizeCheckBoxAndCheckBox( mainWindowUi_->checkBox_smoothingAmplify1, 
                                    mainWindowUi_->checkBox_smoothingAmplify2 );
    synchronizeSpinBoxAndSpinBox( mainWindowUi_->spinBox_smoothingAmplify1, 
                                  mainWindowUi_->spinBox_smoothingAmplify2 );
    synchronizeSliderAndSpinBox( mainWindowUi_->slider_smoothingAmplify1,
                                 mainWindowUi_->spinBox_smoothingAmplify1 );
    synchronizeSliderAndSpinBox( mainWindowUi_->slider_smoothingAmplify2,
                                 mainWindowUi_->spinBox_smoothingAmplify2 );

    QCheckBox * box = mainWindowUi_->checkBox_smoothingAmplify1;
    connectCheckBoxToEnabledState( box, mainWindowUi_->slider_smoothingAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->slider_smoothingAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_smoothingAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->spinBox_smoothingAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_minSmoothingAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maxSmoothingAmplify1 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_minSmoothingAmplify2 );
    connectCheckBoxToEnabledState( box, mainWindowUi_->label_maxSmoothingAmplify2 );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                    Calibration Screen Connections
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BlobSignalsToSlots::connectCalibrationScreen()
{
    connect( calibrationGLWidget_, 
             SIGNAL( closeCalibrationScreen() ),
             blobMainWindow_, 
             SLOT( closeCalibrationScreen() ) );
}

void BlobSignalsToSlots::connectCustomZoomDialog()
{
    connect( zoomDialogUi_->spinBox_zoom_mainPanelImages,
             SIGNAL( valueChanged( int ) ),
             blobMainWindowView_,
             SLOT( zoomMainPanelImagesPercent( int ) ) );

    connect( blobMainWindowView_,
             SIGNAL( zoomMainPanelImagesCalled( int ) ),
             zoomDialog_,
             SLOT( quietlySetZoomControlsForMainPanel( int ) ) );

    connect( zoomDialogUi_->pushButton_zoom_close,
             SIGNAL( pressed() ),
             blobMainWindow_,
             SLOT( closeCustomZoomDialog() ) );

    synchronizeSliderAndSpinBox( zoomDialogUi_->slider_zoom_mainPanelImages,
                                 zoomDialogUi_->spinBox_zoom_mainPanelImages );
}

void BlobSignalsToSlots::synchronizeSliderAndSpinBox( QSlider * slider, 
                                                      QSpinBox * spinBox )
{
    connect( slider, SIGNAL( valueChanged( int  ) ),
             spinBox, SLOT( setValue( int ) ) );
    connect( spinBox, SIGNAL( valueChanged( int  ) ),
             slider, SLOT( setValue( int ) ) );
}

void BlobSignalsToSlots::synchronizeSpinBoxAndSpinBox( QSpinBox * spinBox1, 
                                                       QSpinBox * spinBox2 )
{
    connect( spinBox1, SIGNAL( valueChanged( int  ) ),
             spinBox2, SLOT( setValue( int ) ) );
    connect( spinBox2, SIGNAL( valueChanged( int  ) ),
             spinBox1, SLOT( setValue( int ) ) );
}

void BlobSignalsToSlots::synchronizeDblSpinBoxAndDblSpinBox( QDoubleSpinBox * dblSpinBox1,
                                                             QDoubleSpinBox * dblSpinBox2 )
{
    connect( dblSpinBox1, SIGNAL( valueChanged( double  ) ),
             dblSpinBox2, SLOT( setValue( double ) ) );
    connect( dblSpinBox2, SIGNAL( valueChanged( double  ) ),
             dblSpinBox1, SLOT( setValue( double ) ) );
}

void BlobSignalsToSlots::synchronizeCheckBoxAndCheckBox( QCheckBox * checkBox1, 
                                                         QCheckBox * checkBox2 )
{
    connect( checkBox1, SIGNAL( toggled( bool ) ),
             checkBox2, SLOT( setChecked( bool ) ) );
    connect( checkBox2, SIGNAL( toggled( bool ) ),
             checkBox1, SLOT( setChecked( bool ) ) );
}

void BlobSignalsToSlots::connectCheckBoxToEnabledState( QCheckBox * checkBox1, 
                                                        QCheckBox * checkBox2 )
{
    connect( checkBox1, SIGNAL( toggled( bool ) ),
             checkBox2, SLOT( setEnabled( bool ) ) );
}

void BlobSignalsToSlots::connectCheckBoxToEnabledState( QCheckBox * checkBox, 
                                                        QSlider * slider )
{
    connect( checkBox, SIGNAL( toggled( bool ) ),
             slider, SLOT( setEnabled( bool ) ) );
}

void BlobSignalsToSlots::connectCheckBoxToEnabledState( QCheckBox * checkBox, 
                                                        QSpinBox * spinBox )
{
    connect( checkBox, SIGNAL( toggled( bool ) ),
             spinBox, SLOT( setEnabled( bool ) ) );
}

void BlobSignalsToSlots::connectCheckBoxToEnabledState( QCheckBox * checkBox, 
                                                        QDoubleSpinBox * dblSpinBox )
{
    connect( checkBox, SIGNAL( toggled( bool ) ),
             dblSpinBox, SLOT( setEnabled( bool ) ) );
}

void BlobSignalsToSlots::connectCheckBoxToEnabledState( QCheckBox * checkBox, 
                                                        QLabel * label )
{
    connect( checkBox, SIGNAL( toggled( bool ) ),
             label, SLOT( setEnabled( bool ) ) );
}
