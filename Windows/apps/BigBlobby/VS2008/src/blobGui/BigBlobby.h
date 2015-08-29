////////////////////////////////////////////////////////////////////////////////
// BigBlobby
//
// PURPOSE:  Creates an instance of BlobMainWindow, a QMainWindow subclass,
//           along with the timer that will control the frame rate for the
//           camera images used for blob (fingertip) detection.
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
#ifndef BLOBGUI_BIGBLOBBY_H
#define BLOBGUI_BIGBLOBBY_H

#include "blobGui/BlobMainWindow.h"
#include <QObject>
#include <QTimer>
#include <QTime>

/***************************************************************************//** 
Contains classes for the Qt GUI placed in front of the CCV (Community Core 
Vision) blob detection program.  When the program first starts, main() in the
main.cpp file will create an instance of BigBlobby, which will set up and 
initialize an instance of BlobMainWindow, a QMainWindow subclass and the 
central class of the GUI.<br />

BlobMainWindow will create the QGLWidgets needed for rendering the video camera
source image and processed images, as well as the controls for user input.  
After creating the image widgets and GUI controls, the BlobMainWindow will then 
call on the BlobSignalsToSlots to establish most of the connections from GUI
controls to slots (public functions) on the BlobMainWindow and its helper
classes.<br />

The most important class created by the BlobMainWindow is the SourceGLWidget
class.  In addition to serving as the rendering surface for the source image
from the camera, the SourceGLWidget also creates and operates an instance of
the CoreVision class.  The CoreVision class provides access to the CCV 1.3
camera manipulation, calbration, blob detection, and TUIO server functionality.
The CCV code called on by the CoreVision class is found in the addons directory,
and the OpenFrameworks code used by CCV is found in the lib directory.  The
addons and lib directories are in the same folder as the apps directory that
contains this source code (apps/BigBlobby/VS2008/src).
*******************************************************************************/
namespace blobGui 
{
    /***********************************************************************//**
    Creates an instance of BlobMainWindow, a QMainWindow subclass, along with 
    the timer that will control the frame rate for the camera images used for 
    blob (fingertip) detection.  Before starting the timer, the function to
    load settings from XML files should be called.
    ***************************************************************************/
    class BigBlobby : public QObject
    {
        Q_OBJECT

    public:
        static const int FPS;
        static const double FRAME_TIME_SECS;

        BigBlobby();
        ~BigBlobby();

        void loadSettingsFromXmlFile();
        void setUpTimerForFrameCalls();
        void startSimpleMessageServer();

    public slots: 
        void frame();
        void showGui();

    private:
        blobGui::BlobMainWindow blobMainWindow;
        QTimer frameTimer;
        QTime frameClock;
        double lastFrameTime,
               currentFrameTime,
               deltaFrameTime;
    };
}

#endif
