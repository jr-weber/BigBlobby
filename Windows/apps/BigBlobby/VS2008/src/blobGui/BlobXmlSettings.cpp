////////////////////////////////////////////////////////////////////////////////
// BlobXmlSettings
//
// PURPOSE: Reads/writes XML files for loading/storing the program settings.
//
// CREATED: 6/26/2013
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2013, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#include "blobGui/BlobXmlSettings.h"
#include "blobGui/BlobMainWindow.h"
#include "ui_BlobMainWindow.h"
#include "blobGui/SourceGLWidget.h"
#include "blobIO/BlobParamsXmlReader.h"
#include "blobIO/BlobParamsXmlWriter.h"
#include "blobIO/BlobParamsValidator.h"
#include "blobExceptions/FileNotFoundException.h"
#include "blobExceptions/BlobXmlReaderException.h"
#include "blobExceptions/BlobXmlWriterException.h"
#include "blobExceptions/BlobParamsException.h"
#include "blobExceptions/UnknownXmlTagException.h"
#include "blobGui/DialogBoxUtils.h"
#include <QFileDialog>

using blobGui::BlobXmlSettings;
using blobExceptions::FileNotFoundException;
using blobExceptions::BlobXmlReaderException;
using blobExceptions::BlobXmlWriterException;
using blobExceptions::BlobParamsException;
using blobExceptions::UnknownXmlTagException;

const QString BlobXmlSettings::DEFAULT_CONFIG_FILE = "data/settings/DefaultBlobParams.xml",
              BlobXmlSettings::FACTORY_CONFIG_FILE = "data/settings/FactoryBlobParams.xml";

/***************************************************************************//**
The constructor creates the BlobParamsXmlReader and BlobParamsXmlWriter objects
that will be used for reading and writing program settings from/to XML files, as
well as a dialog box utility class that will be used for reporting any 
read/write errors.

The constructor also stores references to key objects that this class will need
to use: the GUI main window and the Source GLWidget object that holds onto the
CoreVision object used for blob detection.
*******************************************************************************/
BlobXmlSettings::BlobXmlSettings(  blobGui::BlobMainWindow * blobMainWindow  ) :
  blobMainWindow_( blobMainWindow ),
  mainWindowUi_( blobMainWindow->mainWindowUi() ),
  sourceGLWidget_( blobMainWindow->sourceGLWidget() ),
  blobParamsXmlReader_( new blobIO::BlobParamsXmlReader() ),
  blobParamsXmlWriter_( new blobIO::BlobParamsXmlWriter() ),
  dialogBoxUtils_( new blobGui::DialogBoxUtils() ),
  startupParamsValidator_( NULL ),
  debug_( false )
{
}

/***************************************************************************//**
The destructor cleans up memory for the XML reader/writer objects and a dialog
box utility object.
*******************************************************************************/
BlobXmlSettings::~BlobXmlSettings()
{
    delete startupParamsValidator_;
    delete blobParamsXmlWriter_;
    delete blobParamsXmlReader_;
    delete dialogBoxUtils_;
}

/***************************************************************************//**
Loads the default program settings from the file named in DEFAULT_CONFIG_FILE.  

This function should always be called when the program first starts up.
<br />

ERROR HANDLING:<br />
If the default XML file is missing, corrupted, or has an unrecognized XML tag 
or an unacceptable value, the user will be informed of the error through a
dialog box, and then the &quot;factory default&quot; settings defined in the 
BlobXmlValidator class will be used instead.
*******************************************************************************/
void BlobXmlSettings::loadDefaultSettingsAndSaveAsStartupSettings()
{
    startupParamsValidator_ = createValidator( DEFAULT_CONFIG_FILE );

    if( startupParamsValidator_ == NULL ) {
        startupParamsValidator_ = new blobIO::BlobParamsValidator();
        QString msg = "Built-in default settings will be used instead of settings from\n'";
        dialogBoxUtils_->warnUser( msg + DEFAULT_CONFIG_FILE + "'.\n\n" );
    }
    sourceGLWidget_->initializeCoreVisionParams( startupParamsValidator_ );
    blobMainWindow_->setSimpleMessageServerPort( startupParamsValidator_->getSimpleMessageServerPort() );
    quietlyUpdateGui( startupParamsValidator_ );
    debugPrint( startupParamsValidator_ );
}

blobIO::BlobParamsValidator * BlobXmlSettings::createValidator( const QString & filename )
{
    blobIO::BlobParamsValidator * validator = NULL;

    try {
        validator = blobParamsXmlReader_->read( filename.toStdString() );
    }
    catch( FileNotFoundException e ) {
        dialogBoxUtils_->warnUser( e );
    }
    catch( BlobXmlReaderException e ) {
        dialogBoxUtils_->warnUser( e );
    }
    catch( UnknownXmlTagException e ) {
        dialogBoxUtils_->warnUser( e );
    }
    catch( BlobParamsException e ) {
        dialogBoxUtils_->warnUser( e );
    }
    catch( ... ) {
        QString msg = "An unexpected error occurred while trying to read \n'" 
                    + filename + "'.";
        dialogBoxUtils_->warnUser( msg );
    }
    if( blobParamsXmlReader_->hasUnknownXmlTagExceptions() ) {
        dialogBoxUtils_->warnUser( blobParamsXmlReader_->unknownXmlTagExceptions() );
    }
    if( blobParamsXmlReader_->hasBlobParamsExceptions() ) {
        dialogBoxUtils_->warnUser( blobParamsXmlReader_->blobParamsExceptions() );
    }
    return validator;
}

void BlobXmlSettings::restoreStartupSettings()
{
    if( startupParamsValidator_ != NULL ) {
        sourceGLWidget_->updateCoreVisionParams( startupParamsValidator_ );
        quietlyUpdateGui( startupParamsValidator_ );
        debugPrint( startupParamsValidator_ );
    }
}

void BlobXmlSettings::loadFactorySettings()
{
    blobIO::BlobParamsValidator * validator = NULL;
    validator = createValidator( FACTORY_CONFIG_FILE );

    if( validator != NULL ) {
        sourceGLWidget_->updateCoreVisionParams( validator );
        quietlyUpdateGui( validator );
        debugPrint( validator );
        delete validator;
    }
    else {
        QString msg( "No update was done because the \"factory\"\n" );
        msg += "configuration file could not be found:\n\n";
        msg += FACTORY_CONFIG_FILE + "\n\n";
        dialogBoxUtils_->warnUser( msg );
    }
}

/***************************************************************************//**
Saves the current program settings to the DEFAULT_CONFIG_FILE.

This function should always be called before the program exits.
<br />

NOTE:<br />
The calibration settings and camera-specific settings (gain, exposure,
<i>etc.</i>) are not saved here.  The calibration.xml file is always saved
at the very end of a successful calibration (right after the last point is
touched).  The cameras.xml file is saved by the destructor of the ofxPS3 class
(in the ofxPS3.cpp file).
*******************************************************************************/
void BlobXmlSettings::saveDefaultSettings()
{
    saveCurrentSettingsAsXml( DEFAULT_CONFIG_FILE );
}

/***************************************************************************//**
Saves the current program settings to the FACTORY_CONFIG_FILE.

This function is intended to be called through a dialog box or keyboard
shortcut.  Once a factory configuration is saved, the user can restore the
current GUI settings from an option in the File menu.
*******************************************************************************/
void BlobXmlSettings::saveFactorySettingsToXmlFile()
{
    saveCurrentSettingsAsXml( FACTORY_CONFIG_FILE );
}

/***************************************************************************//**
Saves the current program settings to the XML file given as an argument.  The
saved settings are the same ones that are given in the DefaultBlobParams.xml
file that is read in when the program first starts up.

The program settings are obtained by asking the SourceGLWidget object to hand
over a BlobParamsValidator object with info from the CoreVision object.
<br />
*******************************************************************************/
void BlobXmlSettings::saveCurrentSettingsAsXml( const QString & filename )
{
    if( filename != "" ) {
        try {
            blobIO::BlobParamsValidator validator( filename.toStdString() );
            sourceGLWidget_->getCoreVisionParams( &validator );
            validator.showNetworkMenu( blobMainWindow_->isNetworkMenuVisible() );
            validator.showExperimentalMenu( blobMainWindow_->isExperimentalMenuVisible() );
            validator.hideGaussianControls( blobMainWindow_->hideGaussianControls() );
            validator.hideEarlySmoothing( blobMainWindow_->hideEarlySmoothingWidget() );
            validator.setSimpleMessageServerPort( blobMainWindow_->simpleMessageServerPort() );
            debugPrint( &validator );
            blobParamsXmlWriter_->write( &validator );
        }
        catch( BlobXmlWriterException e ) {
            dialogBoxUtils_->warnUser( e );
        }
        catch( ... ) {
            QString msg( "An unexpected error occurred while trying to write \n" );
            dialogBoxUtils_->warnUser( msg + filename );
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                Private Functions for Updating the GUI
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Uses values from the validator object to update all GUI numeric or boolean 
values.  
<br />

The term &quot;quietly&quot; in the function name indicates that Qt signals are
blocked while the update is occuring (except for radio buttons and checkboxes 
because the QActionGroup needst to keep track of state).
*******************************************************************************/
void BlobXmlSettings::quietlyUpdateGui( blobIO::BlobParamsValidator * validator )
{
    quietlyUpdateMenuBar( validator );
    quietlyUpdateMainPanel( validator );
    updateFiltersPanel( validator );
    updateBackgroundControls( validator );
}

/***************************************************************************//**
Updates the menu items that have checkbox or radio buttons.
<br />

The term &quot;quietly&quot; in the function name indicates that Qt signals are
blocked while the update is occuring (except for radio buttons and checkboxes 
because the QActionGroup needst to keep track of state).
*******************************************************************************/
void BlobXmlSettings::quietlyUpdateMenuBar( blobIO::BlobParamsValidator * validator )
{
    quietlyUpdateCameraMenu( validator );
    quietlyUpdateBlobsMenu( validator );
    quietlyUpdateNetworkMenu( validator );
    quietlyUpdateExperimentalMenu( validator );
}

/***************************************************************************//**
Updates some camera settings that show up in the Camera menu: horizontal or
vertical flip.
<br />

The term &quot;quietly&quot; in the function name indicates that Qt signals are
blocked while the update is occuring.
*******************************************************************************/
void BlobXmlSettings::quietlyUpdateCameraMenu( blobIO::BlobParamsValidator * validator )
{
    mainWindowUi_->action_flipImageVertical->blockSignals( true );
    mainWindowUi_->action_flipImageHorizontal->blockSignals( true );

    mainWindowUi_->action_flipImageVertical->setChecked( validator->flipImageVertically() );
    mainWindowUi_->action_flipImageHorizontal->setChecked( validator->flipImageHorizontally() );

    mainWindowUi_->action_flipImageVertical->blockSignals( false );
    mainWindowUi_->action_flipImageHorizontal->blockSignals( false );
}

/***************************************************************************//**
Updates checkboxes in the Blobs menu (show blob outlines, show blob IDs, and
track dark blobs).
<br />

The term &quot;quietly&quot; in the function name indicates that Qt signals are
blocked while the update is occuring.
*******************************************************************************/
void BlobXmlSettings::quietlyUpdateBlobsMenu( blobIO::BlobParamsValidator * validator )
{
    mainWindowUi_->action_showCalibrationWireframe->blockSignals( true );
    mainWindowUi_->action_showBlobAreas->blockSignals( true );
    mainWindowUi_->action_showBlobOutlines->blockSignals( true );
    mainWindowUi_->action_showBlobCrosshairs->blockSignals( true );
    mainWindowUi_->action_showBlobBoundingBox->blockSignals( true );
    mainWindowUi_->action_showIdNumbers->blockSignals( true );
    mainWindowUi_->action_trackDarkBlobs->blockSignals( true );

    mainWindowUi_->action_showCalibrationWireframe->setChecked( validator->isShowCalibrationGridOn() );
    mainWindowUi_->action_showBlobAreas->setChecked( validator->isShowBlobAreasOn() );
    mainWindowUi_->action_showBlobOutlines->setChecked( validator->isShowBlobOutlinesOn() );
    mainWindowUi_->action_showBlobCrosshairs->setChecked( validator->isShowBlobCrosshairsOn() );
    mainWindowUi_->action_showBlobBoundingBox->setChecked( validator->isShowBlobBoundingBoxesOn() );
    mainWindowUi_->action_normalizeBlobIntensities->setChecked( validator->isNormalizeBlobIntensitiesOn() );
    mainWindowUi_->action_showIdNumbers->setChecked( validator->isShowBlobLabelsOn() );
    mainWindowUi_->action_trackDarkBlobs->setChecked( validator->useDarkBlobs() );

    mainWindowUi_->action_showCalibrationWireframe->blockSignals( false );
    mainWindowUi_->action_showBlobAreas->blockSignals( false );
    mainWindowUi_->action_showBlobOutlines->blockSignals( false );
    mainWindowUi_->action_showBlobCrosshairs->blockSignals( false );
    mainWindowUi_->action_showBlobBoundingBox->blockSignals( false );
    mainWindowUi_->action_showIdNumbers->blockSignals( false );
    mainWindowUi_->action_trackDarkBlobs->blockSignals( false );
}

/***************************************************************************//**
Updates which output channels should be checked in the Network menu.
<br />
*******************************************************************************/
void BlobXmlSettings::quietlyUpdateNetworkMenu( blobIO::BlobParamsValidator * validator )
{

    blobMainWindow_->setNetworkMenuVisible( validator->showNetworkMenu() );
    mainWindowUi_->action_silentMode->blockSignals( true );
    mainWindowUi_->action_silentMode->setChecked( validator->showNetworkMenu() );
    mainWindowUi_->action_silentMode->blockSignals( false );
    mainWindowUi_->action_useTuioUdpChannel_1->setChecked( validator->useTuioUdpChannelOne() );
    mainWindowUi_->action_useTuioUdpChannel_2->setChecked( validator->useTuioUdpChannelTwo() );
    mainWindowUi_->action_useFlashXmlChannel_1->setChecked( validator->useFlashXmlChannel() );
    mainWindowUi_->action_useBinaryTcpChannel->setChecked( validator->useBinaryTcpChannel() );
    setNetworkMenuTuioUdpProfile( validator );
    setNetworkMenuFlashXmlProfile( validator );
}

void BlobXmlSettings::setNetworkMenuTuioUdpProfile( blobIO::BlobParamsValidator * validator )
{
    blobEnums::TuioProfileEnum::Enum profile = validator->getTuioUdpProfile();

    switch( profile ) {
        case blobEnums::TuioProfileEnum::TUIO_2D_CURSOR:
            mainWindowUi_->action_useTuio2DcurProfile->setChecked( true );
            mainWindowUi_->action_useTuio2DblbProfile->setChecked( false );
            mainWindowUi_->action_useCurAndBlbProfile->setChecked( false );
            break;
        case blobEnums::TuioProfileEnum::TUIO_2D_BLOB:
            mainWindowUi_->action_useTuio2DcurProfile->setChecked( false );
            mainWindowUi_->action_useTuio2DblbProfile->setChecked( true );
            mainWindowUi_->action_useCurAndBlbProfile->setChecked( false );
            break;
        case blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB:
            mainWindowUi_->action_useTuio2DcurProfile->setChecked( false );
            mainWindowUi_->action_useTuio2DblbProfile->setChecked( false );
            mainWindowUi_->action_useCurAndBlbProfile->setChecked( true );
            break;
    }
}

void BlobXmlSettings::setNetworkMenuFlashXmlProfile( blobIO::BlobParamsValidator * validator )
{
     blobEnums::TuioProfileEnum::Enum profile = validator->getFlashXmlProfile();

    switch( profile ) {
        case blobEnums::TuioProfileEnum::TUIO_2D_CURSOR:
            mainWindowUi_->action_useFlashXmlTuio2DcurProfile->setChecked( true );
            mainWindowUi_->action_useFlashXmlTuio2DblbProfile->setChecked( false );
            mainWindowUi_->action_useFlashXmlCurAndBlbProfile->setChecked( false );
            break;
        case blobEnums::TuioProfileEnum::TUIO_2D_BLOB:
            mainWindowUi_->action_useFlashXmlTuio2DcurProfile->setChecked( false );
            mainWindowUi_->action_useFlashXmlTuio2DblbProfile->setChecked( true );
            mainWindowUi_->action_useFlashXmlCurAndBlbProfile->setChecked( false );
            break;
        case blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB:
            mainWindowUi_->action_useFlashXmlTuio2DcurProfile->setChecked( false );
            mainWindowUi_->action_useFlashXmlTuio2DblbProfile->setChecked( false );
            mainWindowUi_->action_useFlashXmlCurAndBlbProfile->setChecked( true );
            break;
    }
}

void BlobXmlSettings::quietlyUpdateExperimentalMenu( blobIO::BlobParamsValidator * validator )
{
     blobMainWindow_->setExperimentalMenuVisible( validator->showExperimentalMenu() );
}

/***************************************************************************//**
Updates spin boxes and sliders in the main panel (the panel that holds the
Source and Tracked images).
<br />

The term &quot;quietly&quot; in the function name indicates that Qt signals are
blocked while the update is occuring (except for radio buttons and checkboxes 
because the QActionGroup needst to keep track of state).
*******************************************************************************/
void BlobXmlSettings::quietlyUpdateMainPanel( blobIO::BlobParamsValidator * validator )
{
    // Update spin boxes.
    mainWindowUi_->spinBox_imageThreshold->blockSignals( true );
    mainWindowUi_->spinBox_minBlobSize->blockSignals( true );
    mainWindowUi_->spinBox_maxBlobSize->blockSignals( true );
    
    mainWindowUi_->spinBox_imageThreshold->setValue( validator->getImageThreshold() );
    mainWindowUi_->spinBox_minBlobSize->setValue( validator->getMinBlobSize() );
    mainWindowUi_->spinBox_maxBlobSize->setValue( validator->getMaxBlobSize() );
    
    mainWindowUi_->spinBox_imageThreshold->blockSignals( false );
    mainWindowUi_->spinBox_minBlobSize->blockSignals( false );
    mainWindowUi_->spinBox_maxBlobSize->blockSignals( false );

    // Update sliders.
    mainWindowUi_->slider_imageThreshold->blockSignals( true );
    mainWindowUi_->slider_minBlobSize->blockSignals( true );
    mainWindowUi_->slider_maxBlobSize->blockSignals( true );

    mainWindowUi_->slider_imageThreshold->setValue( validator->getImageThreshold() );
    mainWindowUi_->slider_minBlobSize->setValue( validator->getMinBlobSize() );
    mainWindowUi_->slider_maxBlobSize->setValue( validator->getMaxBlobSize() );

    mainWindowUi_->slider_imageThreshold->blockSignals( false );
    mainWindowUi_->slider_minBlobSize->blockSignals( false );
    mainWindowUi_->slider_maxBlobSize->blockSignals( false );

    blobMainWindow_->setNetworkSilentMode( validator->useNetworkSilentMode() );
}

/***************************************************************************//**
Updates spin boxes and sliders in the filters panel (the panel that holds the
Background, Smoothing, Highpass, and Amplify images).

NOTE: Checkboxes are set true to begin with so that having the validator info
set them false will result in other controls being grayed out.  Disabling 
controls (graying them out) is best done programatically, rather than in the 
BlobMainWindowUi file.
*******************************************************************************/
void BlobXmlSettings::updateFiltersPanel( blobIO::BlobParamsValidator * validator )
{
    blobMainWindow_->hideGaussianControls( validator->hideGaussianControls() );
    blobMainWindow_->hideEarlySmoothingWidget( validator->hideEarlySmoothing() );

    mainWindowUi_->checkBox_preBkSmoothing1->setChecked( true );
    mainWindowUi_->checkBox_preBkSmoothing1->setChecked( validator->usePreBkSmoothing() );
    mainWindowUi_->spinBox_preBkSmoothingKernel1->setValue( validator->getPreBkSmoothingBlur() );
    mainWindowUi_->checkBox_gaussianPreBkSmoothing1->setChecked( validator->useGaussianPreBkSmoothing() );
    mainWindowUi_->doubleSpinBox_preBkSmoothingSigma1->setValue( validator->getGaussianPreBkSigma() );

    mainWindowUi_->checkBox_maskFilter1->setChecked( true );
    mainWindowUi_->checkBox_maskFilter1->setChecked( validator->useMask() );
    mainWindowUi_->spinBox_maskPadX1->setValue( validator->getMaskWidthPadding() );
    mainWindowUi_->spinBox_maskPadY1->setValue( validator->getMaskHeightPadding() );

    mainWindowUi_->checkBox_highpass1->setChecked( true );
    mainWindowUi_->checkBox_highpass1->setChecked( validator->useHighpass() );
    mainWindowUi_->spinBox_highpass1->setValue( validator->getHighpassBlur() );
    mainWindowUi_->checkBox_highpassAmplify1->setChecked( true );
    mainWindowUi_->checkBox_highpassAmplify1->setChecked( validator->useHighpassAmplify() );
    mainWindowUi_->spinBox_highpassAmplify1->setValue( validator->getHighpassAmplifyLevel() );

    mainWindowUi_->checkBox_highpassTwoFilter1->setChecked( true );
    mainWindowUi_->checkBox_highpassTwoFilter1->setChecked( validator->useHighpassTwo() );
    mainWindowUi_->spinBox_highpassTwoFilter1->setValue( validator->getHighpassTwoKernel() );
    mainWindowUi_->checkBox_highpassTwoAmplify1->setChecked( true );
    mainWindowUi_->checkBox_highpassTwoAmplify1->setChecked( validator->useHighpassTwoAmplify() );
    mainWindowUi_->spinBox_highpassTwoAmplify1->setValue( validator->getHighpassTwoAmplifyLevel() );

    mainWindowUi_->checkBox_smoothing1->setChecked( true );
    mainWindowUi_->checkBox_smoothing1->setChecked( validator->useSmoothing() );
    mainWindowUi_->spinBox_smoothingKernel1->setValue( validator->getSmoothingBlur() );
    mainWindowUi_->checkBox_gaussianSmoothing1->setChecked( validator->useGaussianSmoothing() );
    mainWindowUi_->doubleSpinBox_smoothingSigma1->setValue( validator->getGaussianSigma() );
    mainWindowUi_->checkBox_smoothingAmplify1->setChecked( true );
    mainWindowUi_->checkBox_smoothingAmplify1->setChecked( validator->useSmoothingAmplify() );
    mainWindowUi_->spinBox_smoothingAmplify1->setValue( validator->getSmoothingAmplifyLevel() );
}

/*******************************************************************************
Updates whether periodic or dynamic background should be used and what the 
learn rate or seconds should be.
*******************************************************************************/
void BlobXmlSettings::updateBackgroundControls( blobIO::BlobParamsValidator * validator )
{
    mainWindowUi_->groupBox_dynamicSubtration->setChecked( validator->useAutoBackground() );
    mainWindowUi_->spinBox_dynamicSubtraction->setValue( (int)(validator->getBackgroundLearnRate()) );
    mainWindowUi_->groupBox_periodicSubtract->setChecked( validator->usePeriodicBackground() );
    mainWindowUi_->spinBox_periodicSubtractSeconds->setValue( (int)(validator->getPeriodicBackgroundSeconds()) );
    mainWindowUi_->spinBox_periodicSubtractPercent->setValue( (int)(validator->getPeriodicBackgroundPercent()) );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                        Private Debugging Functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*******************************************************************************
Writes the contents of the BlobParamsValidator to standard out if debugging
is turned on.
*******************************************************************************/
void BlobXmlSettings::debugPrint( blobIO::BlobParamsValidator * validator )
{
    if( validator != NULL && debug_.isDebuggingOn() ) {
        std::string banner( 80, '*' );
        debug_.printLn( "\n" + banner );
        debug_.printLn( "BlobXmlSettings::debugPrint(validator) called..." );
        debug_.printLn( banner );
        blobParamsXmlWriter_->printXmlToStandardOut( validator );
        debug_.printLn( banner + "\n" );
    }
}
