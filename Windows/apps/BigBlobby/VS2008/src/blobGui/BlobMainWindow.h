////////////////////////////////////////////////////////////////////////////////
// BlobMainWindow
//
// PURPOSE:  Creates the main window of the Qt GUI placed in front of the CCV
//           1.3 blob detection code.
//
// CREATED:  4/13/2011
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
#ifndef BLOBGUI_BLOBMAINWINDOW_H
#define BLOBGUI_BLOBMAINWINDOW_H

#include "ui_BlobMainWindow.h"
#include "blobIO/DebugPrint.h"
#include <QMainWindow>
#include <QColor>

namespace blobGui { class SourceGLWidget; }
namespace blobGui { class FilterGLWidget; }
namespace blobGui { class BlobMainWindowView; }
namespace blobGui { class CalibrationGLWidget; }
namespace blobGui { class BlobFiltersWindow; }
namespace blobGui { class BlobXmlSettings; }
namespace blobGui { class BlobSignalsToSlots; }
namespace blobGui { class CustomZoomDialog; }
namespace blobGui { class FactorySettingsDialog; }
namespace blobGui { class Help; }
namespace blobTcpServer { class SimpleMessageServer; }
namespace Ui { class CustomZoomDialogUi; }
class QActionGroup;
class QScrollBar;

namespace blobGui 
{
    /***********************************************************************//**
    Creates the main window of the Qt GUI placed in front of the CCV 1.3 blob
    detection code.  The main window is primarily defined in the Qt Designer 
    BlobMainWindow.ui file.
    ***************************************************************************/
    class BlobMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        static const int MIN_WIDTH_FOR_RENDERING;
        static const QColor BACKGROUND_COLOR,
                            BOX_COLOR;

        BlobMainWindow();
        ~BlobMainWindow();
        void startSimpleMessageServer();
        void setSimpleMessageServerPort( int port );
        int simpleMessageServerPort();
        void frame( double deltaFrameTime );
        void loadDefaultSettingsFromXmlFile();
        void setNetworkMenuVisible( bool b );
        bool isNetworkMenuVisible();
        void setExperimentalMenuVisible( bool b );
        bool isExperimentalMenuVisible();
        void executeFactorySettingsDialog();

        void hideGaussianControls( bool b );
        bool hideGaussianControls();
        void hideEarlySmoothingWidget( bool b );
        bool hideEarlySmoothingWidget();

        // Public getter functions for key objects owned by BlobMainWindow.
        Ui::BlobMainWindowUi * mainWindowUi();
        Ui::CustomZoomDialogUi * zoomDialogUi();
        blobGui::CustomZoomDialog * zoomDialog();
        blobGui::SourceGLWidget * sourceGLWidget();
        blobGui::CalibrationGLWidget * calibrationGLWidget();
        blobGui::BlobMainWindowView * blobMainWindowView();
        blobGui::BlobXmlSettings * blobXmlSettings();
        blobGui::Help * help();

            
    public slots:
        void openCalibrationScreen();
        void closeCalibrationScreen();
        void openCustomZoomDialog();
        void closeCustomZoomDialog();
        void saveScreenshot();
        void playTestVideo();
        void stopTestVideo();
        void startSourceInputRecording();
        void startSourceColorRecording();
        void stopVideoWriter();
        void onWriteColorVideoFailed();
        void setNetworkSilentMode( bool b );
        void saveGuiAndCameraSettingsToXmlFiles();

        void showSourceHelp( bool show );
        void showThresholdHelp( bool show );
        void showPreBkSmoothingHelp1( bool show );
        void showPreBkSmoothingHelp2( bool show );
        void showBackgroundHelp1( bool show );
        void showBackgroundHelp2( bool show );
        void showHighpass1Help1( bool show );
        void showHighpass1Help2( bool show );
        void showHighpass2Help1( bool show );
        void showHighpass2Help2( bool show );
        void showSmoothingHelp1( bool show );
        void showSmoothingHelp2( bool show );

    signals:

    protected:
        // Protected Event Handling Functions
        virtual void keyPressEvent( QKeyEvent * event );
        virtual void closeEvent( QCloseEvent * event );

    private:
        bool isAtLeastOneImageVisible();

        void setWidgetColors();
        void setWidgetColor( QWidget * w, const QColor & color );
        void addSourceGLWidget();
        void addTrackedGLWidget();
        void addFilterWidgets();

        void addPreBkSmoothingGLWidget();
        void addBackgroundGLWidget();
        void addHighpassGLWidget();
        void addHighpassTwoWidget();
        void addSmoothingGLWidget();
        
        blobGui::FilterGLWidget * createPreBkSmoothingWidget();
        blobGui::FilterGLWidget * createBackgroundWidget();
        blobGui::FilterGLWidget * createHighpassWidget();
        blobGui::FilterGLWidget * createHighpassTwoWidget();
        blobGui::FilterGLWidget * createSmoothingWidget();

        void addCalibrationGLWidget();
        void addBlobMainWindowView();
        void addBlobXmlSettings();
        void addHelp();
        void makeGuiConnections();

        void displayFpsMessage();
        void openVideoWriterWarningBox();
        void showScreenshotErrorMessage();

        // Private Data Members
        Ui::BlobMainWindowUi ui_;
        blobGui::SourceGLWidget * sourceGLWidget_;
        blobGui::FilterGLWidget * trackedGLWidget_;

        blobGui::FilterGLWidget * preBkSmoothingGLWidget1_;
        blobGui::FilterGLWidget * preBkSmoothingGLWidget2_;
        blobGui::FilterGLWidget * backgroundGLWidget1_;
        blobGui::FilterGLWidget * backgroundGLWidget2_;
        blobGui::FilterGLWidget * highpassGLWidget1_;
        blobGui::FilterGLWidget * highpassGLWidget2_;
        blobGui::FilterGLWidget * highpassTwoGLWidget1_;
        blobGui::FilterGLWidget * highpassTwoGLWidget2_;
        blobGui::FilterGLWidget * smoothingGLWidget1_;
        blobGui::FilterGLWidget * smoothingGLWidget2_;

        blobGui::CalibrationGLWidget * calibrationGLWidget_;
        blobGui::BlobMainWindowView * view_;
        blobGui::BlobXmlSettings * blobXmlSettings_;
        blobGui::Help * help_;
        blobGui::BlobSignalsToSlots * blobSignalsToSlots_;
        QActionGroup * backgroundActionGroup_,
                     * tuioUdpProfileActionGroup_,
                     * flashXmlProfileActionGroup_,
                     * unmatchedBlobActionGroup_;
        blobGui::CustomZoomDialog * customZoomDialog_;
        blobGui::FactorySettingsDialog * factorySettingsDialog_;
        QScrollBar * scrollBar_;
        bool needToCalibrate_,
             hideGaussianControls_,
             hideEarlySmoothingWidget_;
            
        blobTcpServer::SimpleMessageServer * simpleMessageServer_;

        // Debugging Utility that Writes to Standard Out
        blobIO::DebugPrint debug_;
    };
}

#endif
