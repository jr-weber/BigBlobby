////////////////////////////////////////////////////////////////////////////////
// BlobMainWindow
//
// PURPOSE: Creates the main window of the Qt GUI placed in front of the CCV
//          1.3 blob detection code.
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
#include "blobGui/BlobMainWindow.h"
#include "blobGui/BlobMainWindowView.h"
#include "blobGui/SourceGLWidget.h"
#include "blobGui/FilterGLWidget.h"
#include "blobGui/CalibrationGLWidget.h"
#include "blobGui/BlobXmlSettings.h"
#include "blobGui/Help.h"
#include "blobGui/BlobSignalsToSlots.h"
#include "blobGui/CustomZoomDialog.h"
#include "blobGui/FactorySettingsDialog.h"
#include "blobExceptions/FileNotFoundException.h"
#include "blobExceptions/BlobXmlReaderException.h"
#include "blobExceptions/BlobParamsException.h"
#include "blobExceptions/UnknownXmlTagException.h"
#include "blobTcpServer/SimpleMessageServer.h"
#include <QApplication>
#include <QActionGroup>
#include <QKeyEvent>
#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QScrollBar>

using blobGui::BlobMainWindow;
using blobGui::BlobMainWindowView;
using blobExceptions::FileNotFoundException;
using blobExceptions::BlobXmlReaderException;
using blobExceptions::BlobParamsException;
using blobExceptions::UnknownXmlTagException;

const int BlobMainWindow::MIN_WIDTH_FOR_RENDERING = 250;

const QColor BlobMainWindow::BACKGROUND_COLOR = QColor( 226, 235, 235 ),
             BlobMainWindow::BOX_COLOR = QColor( 218, 235, 235 );

/***************************************************************************//**
The constructor sets up the GUI main window defined in the Qt Designer 
BlobMainWindow.ui file and then adds to it the GLWidget objects used for
rendering the Source image from the video camera, the saved Background image, 
the filter images (Smoothing, Highpass, and Amplify) and the final image of
the Tracked blobs.  Several helper objects will also be created for specialized
tasks like reading/writing settings from XML files, presenting dialog boxes, and
adjusting how the main window is viewed.  The last object created by the 
constructor is the BlobSignalsToSlots object, which uses the Qt signals / slots
paradigm to make most of the connections from GUI controls to the objects that
will call on CCV code to do the actual blob detection.
<br />
*******************************************************************************/
BlobMainWindow::BlobMainWindow() :
  ui_(),
  sourceGLWidget_( NULL ),
  trackedGLWidget_( NULL ),

  preBkSmoothingGLWidget1_( NULL ),
  preBkSmoothingGLWidget2_( NULL ),
  backgroundGLWidget1_( NULL ),
  backgroundGLWidget2_( NULL ),
  highpassGLWidget1_( NULL ),
  highpassGLWidget2_( NULL ),
  highpassTwoGLWidget1_( NULL ),
  highpassTwoGLWidget2_( NULL ),
  smoothingGLWidget1_( NULL ),
  smoothingGLWidget2_( NULL ),

  calibrationGLWidget_( NULL ),
  view_( NULL ),
  blobXmlSettings_( NULL ),
  help_( NULL ),
  blobSignalsToSlots_( NULL ),
  backgroundActionGroup_( new QActionGroup( this ) ),
  tuioUdpProfileActionGroup_( new QActionGroup( this ) ),
  flashXmlProfileActionGroup_( new QActionGroup( this ) ),
  unmatchedBlobActionGroup_( new QActionGroup( this ) ),
  customZoomDialog_( new blobGui::CustomZoomDialog( this, Qt::Window ) ),
  factorySettingsDialog_( new blobGui::FactorySettingsDialog( this ) ),
  scrollBar_( NULL ),
  needToCalibrate_( false ),
  hideGaussianControls_( false ),
  hideEarlySmoothingWidget_( false ),
  simpleMessageServer_( new blobTcpServer::SimpleMessageServer( this ) ),
  debug_( false )
{
    ui_.setupUi( this );
    setWidgetColors();
    addSourceGLWidget();
    addTrackedGLWidget();
    addFilterWidgets();
    addCalibrationGLWidget();
    addBlobMainWindowView();
    addBlobXmlSettings();
    addHelp();
    makeGuiConnections();
    scrollBar_ = ui_.scrollArea_mainWidget->horizontalScrollBar();
}

/***************************************************************************//**
The destructor cleans up memory for any objects that will not be cleaned up
automatically when the top level GUI component is destroyed.
*******************************************************************************/
BlobMainWindow::~BlobMainWindow()
{
    delete blobSignalsToSlots_;
    delete blobXmlSettings_;
    delete view_;
    delete customZoomDialog_;
    delete factorySettingsDialog_;
    delete simpleMessageServer_;
    delete backgroundActionGroup_;
    delete tuioUdpProfileActionGroup_;
    delete flashXmlProfileActionGroup_;
    delete unmatchedBlobActionGroup_;
}

void BlobMainWindow::startSimpleMessageServer()
{
    simpleMessageServer_->startServer();
}

void BlobMainWindow::setSimpleMessageServerPort( int port )
{
    simpleMessageServer_->setPortNumber( port );
}

int BlobMainWindow::simpleMessageServerPort()
{
    return simpleMessageServer_->portNumber();
}

void BlobMainWindow::hideGaussianControls( bool b )
{
    hideGaussianControls_ = b;
    QSizePolicy::Policy policy = b ? QSizePolicy::Fixed : QSizePolicy::Expanding;

    ui_.doubleSpinBox_preBkSmoothingSigma1->setVisible( !hideGaussianControls_ );
    ui_.label_preBkSmoothingSigma1->setVisible( !hideGaussianControls_ );
    ui_.checkBox_gaussianPreBkSmoothing1->setVisible( !hideGaussianControls_ );
    ui_.horizontalSpacer_preBkSmoothing1->changeSize( 3, 20, policy );
    
    ui_.doubleSpinBox_preBkSmoothingSigma2->setVisible( !hideGaussianControls_ );
    ui_.label_preBkSmoothingSigma2->setVisible( !hideGaussianControls_ );
    ui_.checkBox_gaussianPreBkSmoothing2->setVisible( !hideGaussianControls_ );
    ui_.horizontalSpacer_preBkSmoothing2->changeSize( 3, 20, policy );

    ui_.doubleSpinBox_smoothingSigma1->setVisible( !hideGaussianControls_ );
    ui_.label_smoothingSigma1->setVisible( !hideGaussianControls_ );
    ui_.checkBox_gaussianSmoothing1->setVisible( !hideGaussianControls_ );
    ui_.horizontalSpacer_smoothing1->changeSize( 3, 20, policy );
    
    ui_.doubleSpinBox_smoothingSigma2->setVisible( !hideGaussianControls_ );
    ui_.label_smoothingSigma2->setVisible( !hideGaussianControls_ );
    ui_.checkBox_gaussianSmoothing2->setVisible( !hideGaussianControls_ );
    ui_.horizontalSpacer_smoothing2->changeSize( 3, 20, policy );
}

bool BlobMainWindow::hideGaussianControls()
{
    return hideGaussianControls_;
}

/***************************************************************************//**
This function is only intended to be called once at beginning of program run.
If the arg is true, it will hide widget 0, which is the Early Smoothing widget.
*******************************************************************************/
void BlobMainWindow::hideEarlySmoothingWidget( bool b )
{
    hideEarlySmoothingWidget_ = b;

    if( hideEarlySmoothingWidget_ ) {
        ui_.tabWidget_filtersOnLeft->removeTab( 0 );
        ui_.tabWidget_filtersOnRight->removeTab( 0 );
    }
}

bool BlobMainWindow::hideEarlySmoothingWidget()
{
    return hideEarlySmoothingWidget_;
}

/***************************************************************************//**
This is the key function of the class, as it calls on the blob detection and 
rendering functions.  It starts by calling on the  frame() function of the 
SourceGLWidget to update the CoreVision object and then render the Source image 
if the camera has a new frame.  If a new frame is processed, then this function
will also call on the FilterWidgets to render the Background, Highpass, 
Smoothing, and Tracked images.<br />

This function will also check to see if calibration has been requested, in
which case it will call on the CalibrationGLWidget to open up.  Lastly, this
function will check if it is time to update the frames per second and other
info shown in the text area of the main window.  The time to update is 
determined in the CoreVision class, and should be about every one second.
*******************************************************************************/
void BlobMainWindow::frame( double deltaFrameTime )
{
    sourceGLWidget_->frame( deltaFrameTime );

    if( needToCalibrate_ ) {
        calibrationGLWidget_->frame( deltaFrameTime );
    }
    else if( sourceGLWidget_->hasNewFrame() ) {
        if( isAtLeastOneImageVisible() ) {
            trackedGLWidget_->frame( deltaFrameTime );
            
            preBkSmoothingGLWidget1_->frame( deltaFrameTime );
            preBkSmoothingGLWidget2_->frame( deltaFrameTime );
            backgroundGLWidget1_->frame( deltaFrameTime );
            backgroundGLWidget2_->frame( deltaFrameTime );
            highpassGLWidget1_->frame( deltaFrameTime );
            highpassGLWidget2_->frame( deltaFrameTime );
            highpassTwoGLWidget1_->frame( deltaFrameTime );
            highpassTwoGLWidget2_->frame( deltaFrameTime );
            smoothingGLWidget1_->frame( deltaFrameTime );
            smoothingGLWidget2_->frame( deltaFrameTime );
        }
        if( sourceGLWidget_->needToDisplayUpdatedFps() ) {
            displayFpsMessage();
        }
    }
}

bool BlobMainWindow::isAtLeastOneImageVisible()
{
    int effectiveWidth = width() + scrollBar_->value();
    return (effectiveWidth > MIN_WIDTH_FOR_RENDERING) && !isMinimized();
}

void BlobMainWindow::displayFpsMessage()
{
    // Update fps.
    ui_.textEdit_fpsMessage->clear();
    ui_.textEdit_fpsMessage->append( sourceGLWidget_->getUpdatedFps() );

    // Update network and filter info.
    QScrollBar * scrollBar = ui_.textEdit_outputMessage->verticalScrollBar();
    int position = scrollBar->sliderPosition();
    ui_.textEdit_outputMessage->clear();
    ui_.textEdit_outputMessage->append( sourceGLWidget_->getNetworkAndFiltersInfo() );
    scrollBar->setSliderPosition( position );
}

/***************************************************************************//**
Calls on the BlobXmlSettings object to read general program settings from a
default XML file and then use those settings to update both the GUI and the 
CoreVision object used to access CCV blob detection functionality.

The default XML file is &quot;data/settings/DefaultBlobParams.xml&quot.
<br />

ERROR HANDLING:<br />
If an error occurs while attempting to read and parse the default XML file,  
BlobXmlSettings will warn the user of the error, and then use a set of
&quot;factory default&quot; settings that are hard-coded in the BlobXmlValidator
class.
*******************************************************************************/
void BlobMainWindow::loadDefaultSettingsFromXmlFile()
{
    blobXmlSettings_->loadDefaultSettingsAndSaveAsStartupSettings();
}

/***************************************************************************//**
Hides the Network menu that is otherwise seen in the menubar at the top of 
the main window.
*******************************************************************************/
void BlobMainWindow::setNetworkMenuVisible( bool b )
{
    ui_.menu_network->menuAction()->setVisible( b );
}

/***************************************************************************//**
Returns true if the Network menu is visible in the menubar at the top of the
main window.
*******************************************************************************/
bool BlobMainWindow::isNetworkMenuVisible()
{
    return ui_.menu_network->menuAction()->isVisible();
}

/***************************************************************************//**
Hides the Experimental menu that is otherwise seen in the menubar at the top of 
the main window.
*******************************************************************************/
void BlobMainWindow::setExperimentalMenuVisible( bool b )
{
    ui_.menu_experimental->menuAction()->setVisible( b );
}

/***************************************************************************//**
Returns true if the Experimental menu is visible in the menubar at the top of 
the main window.
*******************************************************************************/
bool BlobMainWindow::isExperimentalMenuVisible()
{
    return ui_.menu_experimental->menuAction()->isVisible();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Public getter functions for key objects owned by BlobMainWindow.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Returns a reference to the Qt Designer GUI object defined in the 
BlobMainWindow.ui file.  This reference will be needed by the BlobSignalsToSlots
object that establishes most of the GUI connections.
*******************************************************************************/
Ui::BlobMainWindowUi * BlobMainWindow::mainWindowUi()
{
    return &ui_;
}

/***************************************************************************//**
Returns a reference to the Qt Designer GUI object defined in the 
CustomZoomDialog.ui file.  This reference will be needed by the 
BlobSignalsToSlots object that establishes most of the GUI connections.
*******************************************************************************/
Ui::CustomZoomDialogUi * BlobMainWindow::zoomDialogUi()
{
    return customZoomDialog_->customZoomDialogUi();
}

/***************************************************************************//**
Returns a reference to the CustomZoomDialog object that opens the dialog box
for increasing the size of the images shown in the main panel (Source and
Tracked) or the images in the filters panel (Background, Smoothing, Highpass).
*******************************************************************************/
blobGui::CustomZoomDialog * BlobMainWindow::zoomDialog()
{
    return customZoomDialog_;
}

/***************************************************************************//**
Returns a reference to the SourceGLWidget that owns the CoreVision object and
is also used to render the Source image from the video camera.
*******************************************************************************/
blobGui::SourceGLWidget * BlobMainWindow::sourceGLWidget()
{
    return sourceGLWidget_;
}

/***************************************************************************//**
Returns a reference to the CalibrationGLWidget that is used for calibrating the
screen surface used for blob (fingertip) detection.
*******************************************************************************/
blobGui::CalibrationGLWidget * BlobMainWindow::calibrationGLWidget()
{
    return calibrationGLWidget_;
}

/***************************************************************************//**
Returns a reference to the BlobMainWindowView object that is used to respond to
the options in the View menu at the top of the GUI main window.
*******************************************************************************/
blobGui::BlobMainWindowView * BlobMainWindow::blobMainWindowView()
{
    return view_;
}

/***************************************************************************//**
Returns a reference to the BlobXmlSettings object that is used to read/write
program settings from/to XML files.  This reference will be needed by the
BigBlobby class (to load settings at program start up) and by BlobSignalsToSlots
(to make connections with options in the File menu).
*******************************************************************************/
blobGui::BlobXmlSettings * BlobMainWindow::blobXmlSettings()
{
    return blobXmlSettings_;
}

/***************************************************************************//**
Returns a reference to the Help object that is needed for the options in the
Help menu at the top of the main window.  The Help object will be needed by
the BlobSignalsToSlots class.
*******************************************************************************/
blobGui::Help * BlobMainWindow::help()
{
    return help_;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                       Slots for Calibration Screen
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Opens a GLWidget to full screen so that it can be used by the CCV monitor
calibration routine.
*******************************************************************************/
void BlobMainWindow::openCalibrationScreen()
{
    if( !needToCalibrate_ ) {
        needToCalibrate_ = true;
        setVisible( false );
        calibrationGLWidget_->showFullScreen();
        sourceGLWidget_->startCalibrationRoutine();
    }
}

/***************************************************************************//**
Closes the screen used for the CCV calibration routine.
*******************************************************************************/
void BlobMainWindow::closeCalibrationScreen()
{
    needToCalibrate_ = false;
    setVisible( true );
    calibrationGLWidget_->setVisible( false );
    sourceGLWidget_->onCalibrationEnded();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                       Slots for Images Zoom Dialog
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Opens a dialog box that allows the user to set a custom zoom size for the
video camera source and processed images shown in the GUI.  Signals from the
CustomZoomDialog will call on slots of the BlobMainWindowView class.
*******************************************************************************/
void BlobMainWindow::openCustomZoomDialog()
{
    customZoomDialog_->show();
}

/***************************************************************************//**
Closes the dialog box used for setting a custom zoom size for the video camera
source and processed images shown in the GUI.
*******************************************************************************/
void BlobMainWindow::closeCustomZoomDialog()
{
    customZoomDialog_->close();
}

/***************************************************************************//**
Saves a screenshot of the GUI as a PNG file.
*******************************************************************************/
void BlobMainWindow::saveScreenshot()
{
    QString filename = QFileDialog::getSaveFileName( this,
                                        "Save BigBlobby Window as Image File",
                                        "./data/screenshots/untitled.png",
                                        "Image(*.png)" );
    if( filename != NULL && !filename.isEmpty() ) {
        int x = this->x(),
            y = this->y(),
            w = frameGeometry().width(),
            h = frameGeometry().height();
        WId id = QApplication::desktop()->winId();
        QPixmap pixmap = QPixmap::grabWindow( id, x, y, w, h );
        bool success = pixmap.save( filename, "png" );
        
        if( !success ) {
            showScreenshotErrorMessage();
        }
    }
}

/***************************************************************************//**
Opens a dialog box to inform the user that the screenshot could not be saved.
*******************************************************************************/
void BlobMainWindow::showScreenshotErrorMessage()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle( "Screenshot Error" );
    messageBox.setText( "The screenshot could not be saved to a file.\n\n"
                        "Perhaps there was a Windows permissions issue." );
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.exec();
}

/***************************************************************************//**
Opens a dialog box to get a filename for the video to play.
*******************************************************************************/
void BlobMainWindow::playTestVideo()
{
    QString filename = QFileDialog::getOpenFileName( this,
                                                     "Test Video for Playback",
                                                     "./data/test_videos",
                                                     "Video(*.avi)" );
    if( filename != NULL ) {
        ui_.action_openTestVideo->setEnabled( false );
        sourceGLWidget_->playTestVideo( filename );
        ui_.action_stopTestVideo->setEnabled( true );
    }
}

/***************************************************************************//**
Stops any in-progress video playback.
*******************************************************************************/
void BlobMainWindow::stopTestVideo()
{
    ui_.action_stopTestVideo->setEnabled( false );
    sourceGLWidget_->stopTestVideo();
    ui_.action_openTestVideo->setEnabled( true );
}

/***************************************************************************//**
Opens a dialog box to get a filename for the video to save.
*******************************************************************************/
void BlobMainWindow::startSourceInputRecording()
{
    QString filename = QFileDialog::getSaveFileName( this,
                                                     "Record Raw Input Source as AVI File",
                                                     "./data/test_videos/untitled.avi",
                                                     "Video(*.avi)" );
    if( filename != NULL ) {
        ui_.menu_recordSourceImage->setEnabled( false );
        bool ok = sourceGLWidget_->startSourceInputRecording( filename );

        if( ok ) {
            ui_.action_stopRecording->setEnabled( true );
        }
        else {
            ui_.menu_recordSourceImage->setEnabled( true );
            ui_.action_stopRecording->setEnabled( false );
            openVideoWriterWarningBox();
        }
    }
}

/***************************************************************************//**
Opens a dialog box to warn the user that the video writer failed to start.
*******************************************************************************/
void BlobMainWindow::openVideoWriterWarningBox()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle( "Video Writer Failed" );
    messageBox.setText( "The video writer could not be started.\n\n"
                        "Either the Cinepak Codec by Radius is missing, or\n"
                        "perhaps there was a Windows permissions problem." );
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.exec();
}

/***************************************************************************//**
Opens a dialog box to get a filename for the video to save.
*******************************************************************************/
void BlobMainWindow::startSourceColorRecording()
{
    QString filename = QFileDialog::getSaveFileName( this,
                                                     "Record Color Source Images as AVI File",
                                                     "./data/demo_videos/untitled.avi",
                                                     "Video(*.avi)" );
    if( filename != NULL ) {
        ui_.menu_recordSourceImage->setEnabled( false );
        bool ok = sourceGLWidget_->startSourceColorRecording( filename );

        if( ok ) {
            ui_.action_stopRecording->setEnabled( true );
        }
        else {
            ui_.menu_recordSourceImage->setEnabled( true );
            ui_.action_stopRecording->setEnabled( false );
            openVideoWriterWarningBox();
        }
    }
}

/***************************************************************************//**
Stops any in-progress writing by the VideoWriter.
*******************************************************************************/
void BlobMainWindow::stopVideoWriter()
{
    ui_.action_stopRecording->setEnabled( false );
    sourceGLWidget_->stopVideoWriter();
    ui_.menu_recordSourceImage->setEnabled( true );
}

/***************************************************************************//**
Opens a dialog box to inform the user that the video writer failed, possibly
because the Source image changed sizes during the recording.
*******************************************************************************/
void BlobMainWindow::onWriteColorVideoFailed()
{
    stopVideoWriter();
    QMessageBox messageBox;
    messageBox.setWindowTitle( "Video Writer Error" );
    messageBox.setText( "The video writer encountered an error.\n\n"
                        "Perhaps the Source image window changed sizes "
                        "while recording was in-progress." );
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.exec();
}

/***************************************************************************//**
Silences all TUIO output channels and keeps the Silent Mode option in the 
Network menu synchronized with the Silent Mode checkbox.  

This mode is intended to allow a user to make adjustments to BigBlobby 
without having the TUIO output affect other open programs such as the 
PlaysurfaceLauncher.  The main concern here is that random blobs from noise 
could cause the PlaysurfaceLauncher to start randomly launching multitouch 
applications.
*******************************************************************************/
void BlobMainWindow::setNetworkSilentMode( bool b )
{
    sourceGLWidget_->setNetworkSilentMode( b );

    if( ui_.checkBox_silentMode->isChecked() != b ) {
        ui_.checkBox_silentMode->blockSignals( true );
        ui_.checkBox_silentMode->setChecked( b );
        ui_.checkBox_silentMode->blockSignals( false );
    }
    if( ui_.action_silentMode->isChecked() != b ) {
        ui_.action_silentMode->blockSignals( true );
        ui_.action_silentMode->setChecked( b );
        ui_.action_silentMode->blockSignals( false );
    }
    ui_.menu_tuioUdp->setDisabled( b );
    ui_.menu_flashXml->setDisabled( b );
    ui_.action_useBinaryTcpChannel->setDisabled( b );
}

/***************************************************************************//**
Saves GUI and PS3 camera settings to XML files.
*******************************************************************************/
void BlobMainWindow::saveGuiAndCameraSettingsToXmlFiles()
{
    blobXmlSettings_->saveDefaultSettings();
    sourceGLWidget_->saveCameraSettingsToXmlFile();
}

void BlobMainWindow::showSourceHelp( bool show )
{
    sourceGLWidget_->showHelpImage( show );
}

void BlobMainWindow::showThresholdHelp( bool show )
{
    trackedGLWidget_->showHelpImage( show );
}

void BlobMainWindow::showPreBkSmoothingHelp1( bool show )
{
    preBkSmoothingGLWidget1_->showHelpImage( show );
}

void BlobMainWindow::showPreBkSmoothingHelp2( bool show )
{
    preBkSmoothingGLWidget2_->showHelpImage( show );
}

void BlobMainWindow::showBackgroundHelp1( bool show )
{
    backgroundGLWidget1_->showHelpImage( show );
}

void BlobMainWindow::showBackgroundHelp2( bool show )
{
    backgroundGLWidget2_->showHelpImage( show );
}

void BlobMainWindow::showHighpass1Help1( bool show )
{
    highpassGLWidget1_->showHelpImage( show );
}

void BlobMainWindow::showHighpass1Help2( bool show )
{
    highpassGLWidget2_->showHelpImage( show );
}

void BlobMainWindow::showHighpass2Help1( bool show )
{
    highpassTwoGLWidget1_->showHelpImage( show );
}

void BlobMainWindow::showHighpass2Help2( bool show )
{
    highpassTwoGLWidget2_->showHelpImage( show );
}

void BlobMainWindow::showSmoothingHelp1( bool show )
{
    smoothingGLWidget1_->showHelpImage( show );
}

void BlobMainWindow::showSmoothingHelp2( bool show )
{
    smoothingGLWidget2_->showHelpImage( show );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                      Protected Event Handling Functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Responds to keyboard shortcuts if the main window has focus.
The current shortcuts are as follows:<br /><br />

Key_B - save Background image (to subract from Source image).<br />
Key_C - open the calibration screen.<br />
*******************************************************************************/
void BlobMainWindow::keyPressEvent( QKeyEvent * event )
{
    debug_.printLn( "BlobMainWindow::keyPressEvent() called..." );

    switch( event->key() ) {
        case Qt::Key_B:  sourceGLWidget_->learnBackground();  break;
        case Qt::Key_C:  openCalibrationScreen();             break;
        case Qt::Key_F:  executeFactorySettingsDialog();      break;
    } 
}

void BlobMainWindow::executeFactorySettingsDialog()
{
    int dialogCode = factorySettingsDialog_->exec();

    if( dialogCode == QDialog::Accepted ) {
        blobXmlSettings_->saveFactorySettingsToXmlFile();
    }
}

/***************************************************************************//**
Save program settings to an XML file and then closes all windows to quit the
program.
*******************************************************************************/
void BlobMainWindow::closeEvent( QCloseEvent * event )
{
    debug_.printLn( "BlobMainWindow::closeEvent(event) called..." );
    setVisible( false );
    blobXmlSettings_->saveDefaultSettings();
    // PS3 camera settings are saved to an XML file by the ofxPS3 destructor.
    QMainWindow::closeEvent( event );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                Private Helper Functions for the Constructor
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*******************************************************************************
Sets the window background color and a color for control boxes.  Otherwise,
everything would be gray.
*******************************************************************************/
void BlobMainWindow::setWidgetColors()
{
    setWidgetColor( this, BACKGROUND_COLOR );
    setWidgetColor( ui_.tab_preBkSmoothing1, BOX_COLOR );
    setWidgetColor( ui_.tab_preBkSmoothing2, BOX_COLOR );
    setWidgetColor( ui_.tab_background1, BOX_COLOR );
    setWidgetColor( ui_.tab_background2, BOX_COLOR );
    setWidgetColor( ui_.tab_highpass1, BOX_COLOR );
    setWidgetColor( ui_.tab_highpass2, BOX_COLOR );
    setWidgetColor( ui_.tab_highpassTwo1, BOX_COLOR );
    setWidgetColor( ui_.tab_highpassTwo2, BOX_COLOR );
    setWidgetColor( ui_.tab_smoothing1, BOX_COLOR );
    setWidgetColor( ui_.tab_smoothing2, BOX_COLOR );
    
    setWidgetColor( ui_.tab_periodicBackground, BOX_COLOR );
    setWidgetColor( ui_.tab_dynamicBackground, BOX_COLOR );

    setWidgetColor( ui_.groupBox_pipeline, BOX_COLOR );
    setWidgetColor( ui_.groupBox_threshold, BOX_COLOR );
    setWidgetColor( ui_.groupBox_blobArea, BOX_COLOR );
    setWidgetColor( ui_.groupBox_periodicSubtract, BOX_COLOR );
    setWidgetColor( ui_.groupBox_dynamicSubtration, BOX_COLOR );

    setWidgetColor( ui_.groupBox_preBkSmoothing1, BOX_COLOR );
    setWidgetColor( ui_.groupBox_preBkSmoothing2, BOX_COLOR );
    setWidgetColor( ui_.groupBox_subtractBackground1, BOX_COLOR );
    setWidgetColor( ui_.groupBox_subtractBackground2, BOX_COLOR );
    setWidgetColor( ui_.groupBox_highpass1, BOX_COLOR );
    setWidgetColor( ui_.groupBox_highpass2, BOX_COLOR );
    setWidgetColor( ui_.groupBox_highpassTwoFilter1, BOX_COLOR );
    setWidgetColor( ui_.groupBox_highpassTwoFilter2, BOX_COLOR );
    setWidgetColor( ui_.groupBox_smoothing1, BOX_COLOR );
    setWidgetColor( ui_.groupBox_smoothing2, BOX_COLOR );

    setWidgetColor( ui_.tabWidget_filtersOnLeft, BOX_COLOR );
    setWidgetColor( ui_.tabWidget_filtersOnRight, BOX_COLOR );
}

/*******************************************************************************
Helper function for setWidgetColors().  Gets the color palette belonging to the
widget and sets its Window property to the color given as an argument.
QPalette::Window is used because QPalette::Background is considered as
deprecated in Qt 4.8.
*******************************************************************************/
void BlobMainWindow::setWidgetColor( QWidget * w, const QColor & color )
{
    QPalette pal = w->palette();
    pal.setColor( QPalette::Window, color );
    w->setAutoFillBackground( true );
    w->setPalette( pal );
}

/*******************************************************************************
Creates a SourceGLWidget object for displaying the video camera Source image and
adds the widget to the GUI created from the Qt Designer BlobMainWindow.ui file.
*******************************************************************************/
void BlobMainWindow::addSourceGLWidget()
{
    QImage image( ":/QResources/images/help/SourceHelpImage-320x240.png" );
    sourceGLWidget_ = new blobGui::SourceGLWidget();
    sourceGLWidget_->setHelpImage( image );
    ui_.horizontalLayout_sourceImage->addWidget( sourceGLWidget_ );
}

/*******************************************************************************
Creates a FilterGLWidget object for displaying the video camera Tracked image
and adds the widget to the GUI defined in the Qt Designer BlobMainWindow.ui 
file.
*******************************************************************************/
void BlobMainWindow::addTrackedGLWidget()
{
    QImage image( ":/QResources/images/help/ThresholdHelpImage-320x240.png" );
    trackedGLWidget_ = new blobGui::FilterGLWidget( NULL, 
                                        sourceGLWidget_,
                                        sourceGLWidget_->getCoreVision(),
                                        blobEnums::FilterEnum::TRACKED );
    trackedGLWidget_->setHelpImage( image );
    ui_.horizontalLayout_trackedImage->addWidget( trackedGLWidget_ );
}

/*******************************************************************************
Adds filter widgets to the QTabWidgets in the BlobMainWindow.ui file.
*******************************************************************************/
void BlobMainWindow::addFilterWidgets()
{
    addPreBkSmoothingGLWidget();
    addBackgroundGLWidget();
    addHighpassGLWidget();
    addHighpassTwoWidget();
    addSmoothingGLWidget();
}

/*******************************************************************************
Creates a FilterGLWidget object for displaying a Smoothing image and adds
the widget to the GUI defined in the Qt Designer BlobMainWindow.ui file.
*******************************************************************************/
void BlobMainWindow::addPreBkSmoothingGLWidget()
{
    QImage image( ":/QResources/images/help/PreBkSmoothingHelpImage-320x240.png" );
    preBkSmoothingGLWidget1_ = createPreBkSmoothingWidget();
    preBkSmoothingGLWidget2_ = createPreBkSmoothingWidget();
    preBkSmoothingGLWidget1_->setHelpImage( image );
    preBkSmoothingGLWidget2_->setHelpImage( image );
    ui_.horizontalLayout_preBkSmoothingImage1->addWidget( preBkSmoothingGLWidget1_ );
    ui_.horizontalLayout_preBkSmoothingImage2->addWidget( preBkSmoothingGLWidget2_ );
}

blobGui::FilterGLWidget * BlobMainWindow::createPreBkSmoothingWidget()
{
    return new blobGui::FilterGLWidget( NULL, 
                                        sourceGLWidget_,
                                        sourceGLWidget_->getCoreVision(),
                                        blobEnums::FilterEnum::PRE_BKGRD_SMOOTH );
}

/*******************************************************************************
Creates a FilterGLWidget object for displaying the Background image and adds
the widget to the GUI defined in the Qt Designer BlobMainWindow.ui file.
*******************************************************************************/
void BlobMainWindow::addBackgroundGLWidget()
{
    QImage image( ":/QResources/images/help/BackgroundHelpImage-320x240.png" );
    backgroundGLWidget1_ = createBackgroundWidget();
    backgroundGLWidget2_ = createBackgroundWidget();
    backgroundGLWidget1_->setHelpImage( image );
    backgroundGLWidget2_->setHelpImage( image );
    ui_.horizontalLayout_backgroundImage1->addWidget( backgroundGLWidget1_ );
    ui_.horizontalLayout_backgroundImage2->addWidget( backgroundGLWidget2_ );
}

blobGui::FilterGLWidget * BlobMainWindow::createBackgroundWidget()
{
    return new blobGui::FilterGLWidget( NULL, 
                                        sourceGLWidget_,
                                        sourceGLWidget_->getCoreVision(),
                                        blobEnums::FilterEnum::BACKGROUND );
}

/*******************************************************************************
Creates a FilterGLWidget object for displaying the Highpass image and adds
the widget to the GUI defined in the Qt Designer BlobMainWindow.ui file.
*******************************************************************************/
void BlobMainWindow::addHighpassGLWidget()
{
    QImage image( ":/QResources/images/help/Highpass1HelpImage-320x240.png" );
    highpassGLWidget1_ = createHighpassWidget();
    highpassGLWidget2_ = createHighpassWidget();
    highpassGLWidget1_->setHelpImage( image );
    highpassGLWidget2_->setHelpImage( image );
    ui_.horizontalLayout_highpassImage1->addWidget( highpassGLWidget1_ );
    ui_.horizontalLayout_highpassImage2->addWidget( highpassGLWidget2_ );
}

blobGui::FilterGLWidget * BlobMainWindow::createHighpassWidget()
{
    return new blobGui::FilterGLWidget( NULL, 
                                        sourceGLWidget_,
                                        sourceGLWidget_->getCoreVision(),
                                        blobEnums::FilterEnum::HIGHPASS ); 
}

/*******************************************************************************
Creates a FilterGLWidget object for displaying the HighpassTwo image and adds
the widget to the GUI defined in the Qt Designer BlobMainWindow.ui file.
*******************************************************************************/
void BlobMainWindow::addHighpassTwoWidget()
{
    QImage image( ":/QResources/images/help/Highpass2HelpImage-320x240.png" );
    highpassTwoGLWidget1_ = createHighpassTwoWidget();
    highpassTwoGLWidget2_ = createHighpassTwoWidget();
    highpassTwoGLWidget1_->setHelpImage( image );
    highpassTwoGLWidget2_->setHelpImage( image );
    ui_.horizontalLayout_highpassTwoFilterImage1->addWidget( highpassTwoGLWidget1_ );
    ui_.horizontalLayout_highpassTwoFilterImage2->addWidget( highpassTwoGLWidget2_ );
}

blobGui::FilterGLWidget * BlobMainWindow::createHighpassTwoWidget()
{
    return new blobGui::FilterGLWidget( NULL, 
                                        sourceGLWidget_,
                                        sourceGLWidget_->getCoreVision(),
                                        blobEnums::FilterEnum::HIGHPASS_TWO ); 
}

/*******************************************************************************
Creates a FilterGLWidget object for displaying the Smoothing image and adds
the widget to the GUI defined in the Qt Designer BlobMainWindow.ui file.
*******************************************************************************/
void BlobMainWindow::addSmoothingGLWidget()
{
    QImage image( ":/QResources/images/help/SmoothingHelpImage-320x240.png" );
    smoothingGLWidget1_ = createSmoothingWidget();
    smoothingGLWidget2_ = createSmoothingWidget();
    smoothingGLWidget1_->setHelpImage( image );
    smoothingGLWidget2_->setHelpImage( image );
    ui_.horizontalLayout_smoothingImage1->addWidget( smoothingGLWidget1_ );
    ui_.horizontalLayout_smoothingImage2->addWidget( smoothingGLWidget2_ );
}

blobGui::FilterGLWidget * BlobMainWindow::createSmoothingWidget()
{
    return new blobGui::FilterGLWidget( NULL, 
                                        sourceGLWidget_,
                                        sourceGLWidget_->getCoreVision(),
                                        blobEnums::FilterEnum::SMOOTH ); 
}

/*******************************************************************************
Creates the GLWidget that will be used for the CCV calibration routine.
*******************************************************************************/
void BlobMainWindow::addCalibrationGLWidget()
{
    calibrationGLWidget_ = new blobGui::CalibrationGLWidget( NULL, 
                                            sourceGLWidget_, 
                                            sourceGLWidget_->getCoreVision() );
    calibrationGLWidget_->resize( width(), height() );
}

/*******************************************************************************
Creates the BlobMainWindowView object and gives it references to the GUI objects
that it will manipulate in response to signals from the View menu.
*******************************************************************************/
void BlobMainWindow::addBlobMainWindowView()
{
    view_ = new BlobMainWindowView( this, &ui_ );
}

/*******************************************************************************
Creates the BlobXmlSettings object and gives it references to the GUI objects
that it will need for loading/saving program settings from/to XML files.
*******************************************************************************/
void BlobMainWindow::addBlobXmlSettings()
{
    blobXmlSettings_ = new blobGui::BlobXmlSettings( this );
}

void BlobMainWindow::addHelp()
{
    help_ = new blobGui::Help( sourceGLWidget_ );
}

/*******************************************************************************
Converts some menu options into radio buttons by adding them to an action group,
and then creates the BlobSignalsToSlots object that is used to establish most
of the connections from GUI controls to the objects that will ultimately do 
the blob detection by calling on CCV code.
*******************************************************************************/
void BlobMainWindow::makeGuiConnections()
{
    ui_.action_useTuio2DcurProfile->setActionGroup( tuioUdpProfileActionGroup_ );
    ui_.action_useTuio2DblbProfile->setActionGroup( tuioUdpProfileActionGroup_ );
    ui_.action_useCurAndBlbProfile->setActionGroup( tuioUdpProfileActionGroup_ );
    ui_.action_useFlashXmlTuio2DcurProfile->setActionGroup( flashXmlProfileActionGroup_ );
    ui_.action_useFlashXmlTuio2DblbProfile->setActionGroup( flashXmlProfileActionGroup_ );
    ui_.action_useFlashXmlCurAndBlbProfile->setActionGroup( flashXmlProfileActionGroup_ );

    ui_.action_deathCount_0->setActionGroup( unmatchedBlobActionGroup_ );
    ui_.action_deathCount_5->setActionGroup( unmatchedBlobActionGroup_ );
    ui_.action_deathCount_10->setActionGroup( unmatchedBlobActionGroup_ );
    ui_.action_deathCount_20->setActionGroup( unmatchedBlobActionGroup_ );

    // Make all signals to slots connections.
    blobSignalsToSlots_ = new blobGui::BlobSignalsToSlots( this );
    blobSignalsToSlots_->makeConnections();
}
