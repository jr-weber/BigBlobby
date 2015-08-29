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
#include "blobGui/BigBlobby.h"
#include "blobGui/BlobXmlSettings.h"

using blobGui::BigBlobby;

const int BigBlobby::FPS = 125;
const double BigBlobby::FRAME_TIME_SECS = 1.0 / BigBlobby::FPS;

/***************************************************************************//**
The constructor creates the BlobMainWindow along with the timer and clock that
will be needed for calling the frame() function.
*******************************************************************************/
BigBlobby::BigBlobby() :
  blobMainWindow(),
  frameTimer(),
  frameClock(),
  lastFrameTime( 0.0 ),
  currentFrameTime( 0.0 ),
  deltaFrameTime( 0.0 )
{
}

BigBlobby::~BigBlobby()
{
}

/***************************************************************************//**
Calls on the BlobMainWindow to use its BlobXmlSettings helper object to load
program settings from an XML file in the data/settings directory.  The settings
will be used to update both the GUI and the CoreVision class, which runs the 
actual image processing and blob detection code of CCV 1.3.
<br />

ERROR HANDLING:<br />
If an error occurs while attempting to read and parse the default XML file,  
a dialog box will warn the user of the error, and a set of 
&quot;factory default&quot; settings will be used instead.
*******************************************************************************/
void BigBlobby::loadSettingsFromXmlFile()
{
    blobMainWindow.loadDefaultSettingsFromXmlFile();
}

/***************************************************************************//**
Connects the timer object to the frame() function and then starts the timer.
The value of FRAME_TIME_SECS determines how often the frame() function will
be called.  This function also starts the clock that will be used to calculate
the actual time difference between function calls.
*******************************************************************************/
void BigBlobby::setUpTimerForFrameCalls()
{
    connect( &frameTimer,
             SIGNAL( timeout() ),
             this,
             SLOT( frame() ) );

    frameTimer.setSingleShot( false );
    frameTimer.start( (int)(1000 * FRAME_TIME_SECS) );
    frameClock.start();
}

/***************************************************************************//**
Calls on the frame(deltaFrameTime) function of the BlobMainWindow.  Each time
frame() is called, it compares the current clock time to the time for the
previous call to frame() and uses the difference as the deltaFrameTime argument
that is plugged in to the BlobMainWindow frame() function.
*******************************************************************************/
void BigBlobby::frame()
{
    // Convert milliseconds to seconds and calculate delta.
    currentFrameTime = frameClock.elapsed() / 1000.0;
    deltaFrameTime = currentFrameTime - lastFrameTime;

    // Main window will call on frame() of gl widgets.
    blobMainWindow.frame( deltaFrameTime );

    // Remember last frame time.
    lastFrameTime = currentFrameTime;
}

/***************************************************************************//**
Calls on the show() function of the BlobMainWindow.
*******************************************************************************/
void BigBlobby::showGui()
{
    blobMainWindow.show();
}

void BigBlobby::startSimpleMessageServer()
{
    blobMainWindow.startSimpleMessageServer();
}
