////////////////////////////////////////////////////////////////////////////////
// BlobSignalsToSlots
//
// PURPOSE: Connects signals from GUI components with slots on BlobMainWindow or
//          one of BlobMainWindow's helper objects.
//
// CREATED: 5/17/2011
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
#ifndef BLOBGUI_BLOBSIGNALSTOSLOTS_H
#define BLOBGUI_BLOBSIGNALSTOSLOTS_H

#include <QObject>

namespace Ui { class BlobMainWindowUi; }
namespace Ui { class CustomZoomDialogUi; }
namespace blobGui { class CustomZoomDialog; }
namespace blobGui { class SourceGLWidget; }
namespace blobGui { class BlobMainWindow; }
namespace blobGui { class BlobMainWindowView; }
namespace blobGui { class CalibrationGLWidget; }
namespace blobGui { class BlobXmlSettings; }
class QSlider;
class QSpinBox;
class QCheckBox;
class QLabel;
class QDoubleSpinBox;

namespace blobGui 
{
    /***********************************************************************//**
    Connects signals from GUI components with slots on BlobMainWindow or one of 
    BlobMainWindow's helper objects.

    The key helper classes for BlobMainWindow are CustomZoomDialog, 
    BlobMainWindowView, CalibrationGLWidget, and SourceGLWidget, which owns the
    CoreVision object.  Calling on the BlobSignalsToSlots::makeConnections() 
    function should be the very last operation carried out by the BlobMainWindow
    constructor.
    ***************************************************************************/
    class BlobSignalsToSlots : public QObject
    {
        Q_OBJECT

        public:
            BlobSignalsToSlots( blobGui::BlobMainWindow * blobMainWindow );
            ~BlobSignalsToSlots();
            void makeConnections();

        private:
            // MenuBar Connections
            void connectMenuBar();
            void connectFileMenu();
            void connectViewMenu();
            void connectCameraMenu();
            void connectBlobsMenu();
            void connectNetworkMenu();
            void connectTuioUdpSubmenu();
            void connectFlashXmlSubmenu();
            void connectHelpMenu();
            void connectExperimentalMenu();

            // Main Panel Connections
            void connectMainPanel();
            void connectMainPanelButtons();
            void connectImageThresholdControls();
            void connectMinBlobSizeControls();
            void connectMaxBlobSizeControls();

            // Filters Panel Connections
            void connectFiltersPanel();
            void connectPreBkSmoothingControls();
            void connectMaskControls();
            void connectBackgroundControls();
            void connectHighpassControls();
            void connectHighpassAmplifyControls();
            void connectHighpassTwoControls();
            void connectHighpassTwoAmplifyControls();
            void connectSmoothingControls();
            void connectSmoothingAmplifyControls();

            // Calibration Screen Connections
            void connectCalibrationScreen();

            // Zoom Dialog Box Connections
            void connectCustomZoomDialog();
            
            // Connect Slider and SpinBox
            void synchronizeSliderAndSpinBox( QSlider * slider, QSpinBox * spinBox );
            void synchronizeSpinBoxAndSpinBox( QSpinBox * spinBox1, QSpinBox * spinBox2 );
            void synchronizeDblSpinBoxAndDblSpinBox( QDoubleSpinBox * dblSpinBox1, QDoubleSpinBox * dblSpinBox2 );
            void synchronizeCheckBoxAndCheckBox( QCheckBox * checkBox1, QCheckBox * checkBox2 );

            void connectCheckBoxToEnabledState( QCheckBox * checkBox1, QCheckBox * checkBox2 );
            void connectCheckBoxToEnabledState( QCheckBox * checkBox, QSlider * slider );
            void connectCheckBoxToEnabledState( QCheckBox * checkBox, QSpinBox * spinBox );
            void connectCheckBoxToEnabledState( QCheckBox * checkBox, QDoubleSpinBox * dblSpinBox );
            void connectCheckBoxToEnabledState( QCheckBox * checkBox, QLabel * label );

            blobGui::BlobMainWindow * blobMainWindow_;
            Ui::BlobMainWindowUi * mainWindowUi_;
            Ui::CustomZoomDialogUi * zoomDialogUi_;
            blobGui::CustomZoomDialog * zoomDialog_;
            blobGui::BlobMainWindowView * blobMainWindowView_;
            blobGui::SourceGLWidget * sourceGLWidget_;
            blobGui::CalibrationGLWidget * calibrationGLWidget_;
            blobGui::BlobXmlSettings * blobXmlSettings_;
    };
}

#endif
