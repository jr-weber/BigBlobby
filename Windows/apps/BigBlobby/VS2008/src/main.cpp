////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
// PURPOSE:  Creates and runs an instance of the BigBlobby class, which uses
//           a Qt GUI in front of the Community Core Vision open-source blob 
//           detection program.  In this context, "blobs" is a reference to the
//           images produced by fingertips moving on the surface of a multitouch
//           table.
//
// CREATED:  4/13/2011
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)

Copyright (c) 2011, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)
Copyright (c) 2009 NUI Group/Inc. All rights reserved. (modified CCV 1.3 classes)
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
#include "blobGui/BigBlobby.h"
#include <QApplication>

/** @file */ 

/***************************************************************************//**
Creates and runs an instance of the BigBlobby class, which uses a Qt GUI in
front of the Community Core Vision open-source blob detection program.  
 
In this context, "blobs" is a reference to the images produced by fingertips 
moving on the surface of a multitouch table.
<br />

Before showing the main window of the GUI, the BlobDetector is called on to
load setting from XML files and to set up a timer for frame calls.
 ******************************************************************************/
int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    blobGui::BigBlobby bigBlobby;
    bigBlobby.loadSettingsFromXmlFile();
    bigBlobby.setUpTimerForFrameCalls();
    bigBlobby.startSimpleMessageServer();
    bigBlobby.showGui();
    return app.exec();
}

/***************************************************************************//**
@mainpage   BigBlobby

<p>
The BigBlobby GUI was written using the open source Qt framework (Qt 4.7.3)
(http://qt-project.org), and has been placed in front of the existing 
CCV 1.3 program with only minimal modifications to the existing CCV 
code. The original CCV 1.3 GUI used GLUT (OpengGL Utilities Library) as 
a GUI. GLUT was only intended as a very basic GUI for OpenGL tutorials, 
and does not have many of the advanced features of a professional GUI 
framework such as Qt. In particular, the GLUT GUI for CCV is not 
resizable, and does not have any kind of zoom control for the images it 
displays. While writing BigBlobby, those kind of features were fairly 
straight-forward to implement with the Qt framework, along with a set of 
menu controls and spin boxes that are synchronized with sliders. 
</p>

<p>
The BigBlobby GUI should also be relatively easy to modify in the future 
because it uses Qt Designer 
(http://qt-project.org/doc/qt-4.8/designer-to-know.html). Qt Designer is 
a visual GUI creation tool that allows a developer to drag-and-drop 
widgets (buttons, sliders, spin boxes, labels, text areas, menus, 
dockable panels, etc.) to rapidly create a sophisticated GUI. Like CCV 
and Qt, BigBlobby is distributed under the GNU Lesser General Public 
License version 2.1, so you are free to modify and redistribute 
BigBlobby. 
</p>

<p>
Currently, the BigBlobby source code is built with Visual Studio 2008 
for C++, and only runs on the Windows operating system.  To build 
BigBlobby with Visual Studio 2008, you must first install the Qt 4.7.3
(or higher) library from http://qt-project.org.  You should also install 
the Qt Visual Studio Add-in.
</p>

<h2>Overview of the BigBlobby GUI</h2>

<p>
When the program starts up, the main() function in main.cpp creates an 
instance of the QApplication class (of the Qt framework) and an instance of the 
BigBlobby class. BigBlobby is called on to load general program settings 
from an XML file and to start a timer that runs the frame calls for 
blob detection. BigBlobby is then asked to show the main window of the 
GUI before the exec() function of the QApplication is called to start 
the Qt event loop. The BigBlobby class itself is fairly simple. Its 
primary responsibilities are to create an instance of the main window of 
the GUI, BlobMainWindow, and to create the timer object that is used to 
repeatedly call on the frame() function of the BlobMainWindow. 
</p>

<p>
An overview of the GUI and blob detection classes created by the 
BlobMainWindow is shown in the UML class diagram of <b>Figure 1</b>. In 
UML notation, a line with an arrowhead indicates an important 
association between 2 classes, while a line with a solid black diamond 
at one end indicates an ownership type of association. For example, 
BigBlobby is the owner of the BlobMainWindow, which is in turn the owner 
of SourceGLWidget, which is in turn the owner of the CoreVision class that 
does the actual blob detection by calling on CCV code. 
</p>

<p>
<img src="uml/BigBlobby.png" alt="UML for BigBlobby GUI" />
</p>
<center><h3>Figure 1. UML Diagram for BigBlobby GUI Classes</h3></center>

<p>
Only classes written specifically for the BigBlobby program are shown in 
<b>Figure 1</b>. Classes belonging to the Qt framework are not shown, as 
they would be too numerous. Important inheritance relationships to 
be aware of are that BlobMainWindow is a subclass of the Qt QMainWindow 
class, and the SourceGLWidget, FilterGLWidget, and CalibrationGLWidget 
classes are all subclasses of the Qt QGLWidget class, which gives them 
access to OpenGL rendering routines. The SourceGLWidget class that owns 
the CoreVision object is used as a surface to draw the video camera 
Source image shown in the GUI, while the final Tracked image is drawn on 
a simpler FilterGLWidget, as are the Background, Smoothing, Highpass, 
and Amplify images. 
</p>

<h3>Access to CCV Code</h3>

<p>
The central class in CCV is ofxNCoreVision. Its update() function, which 
is called in a continuous loop, obtains the camera image and hands it 
over to a ProcessFilters object for processing. The processed image is 
then given to a ContourFinder object, which in turn is handed over to a 
BlobTracker object. Finally, blobs found by the BlobTracker are handed 
over to a TUIO object for broadcasting.  This order of operations is
summarized in <b>Figure 2</b>.
</p>

<p>
<img src="uml/ofxNCoreVision.png" alt="Order of CCV Operations" />
</p>
<center>
<h3>Figure 2. Order of Operations in the CCV ofxNCoreVision::update() Function</h3>
</center>

<p>
Surprisingly, the ofxNCoreVision class also creates the GLUT GUI
(class ofxGui), and sets it up, rather than the other way around. For the 
purpose of replacing the GUI (by using Qt instead of GLUT), it would 
have been better if the ofxGui knew about ofxNCoreVision, but 
ofxNCoreVision did not know anything about the GLUT GUI code. Because of 
this issue, to use a Qt framework GUI, it was necessary to duplicate the 
ofxNCoreVision class (renamed as blobCore::CoreVision), and weed out 
GLUT GUI code, including the ofxNCoreVision::setUpControls() and 
ofxNCoreVision::handleGui() functions found in the CCV gui.h file. 
</p>

<p>
On a related issue, the CCV ProcessFilters.h file contains a single 
draw() function that calls on each image (Source, Background, Smoothing, 
Highpass, Amplify, and Tracked) to render itself at a specific 
location on the GLUT GUI (using OpenGL rendering). To allow BigBlobby to 
use the ProcessFilters class almost &quot;as is&quot;, 
blobCore::BlobFilters was created as a subclass of the CCV 
ProcessFilters class, and functions were added for drawing each image 
separately. Separate draw() functions allow each image to be rendered 
on its own Qt QGLWidget object. The blobCore::BlobFilters class also 
provides a convienient place to put public setter and getter functions 
for filter parameters.
</p>

<p>
The blobCore::BlobCalibration class of BigBlobby is a slightly modified 
copy of the CCV Calibration class. The only real issue here is that the 
keyboard input needed during calibration is now coming from the Qt GUI, 
not from GLUT. To prevent BlobCalibration from directly knowing anything 
about the Qt GUI, the keyboard input detected by Qt classes is converted 
to an enumeration defined in the BigBlobby blobEnums::KeyEnum.h file. 
That enum is then used as an argument to the blobCore::CoreVision 
keyPressed() function, which passes the info on to the 
blobCore::BlobCalibration object. The touch events used in the 
BlobCalibration class are still coming from CCV code, so no changes were 
needed there. 
</p>

<p>
As an important design decision, the blobCore::CoreVision class is 
intended as the only way that the BigBlobby GUI classes should access 
CCV code.  That is why the CoreVision class provides facade-type 
functions for controlling its helper classes, rather than exposing 
any of its helper classes directly.  Moreover, to maintain a good 
separation between view (GUI) and underlying model (blob detection), 
the CoreVision class and its helper classes should not know anything 
about the Qt GUI classes or anything else in the Qt library. 
</p>

<h3>Qt Designer and BlobMainWindow</h3>

<p>
The main window of BigBlobby was created primarily using Qt Designer, 
which comes bundled with the free, open source Qt library. Qt Designer 
is a visual tool that that allows a developer to drag-and-drop widgets 
(buttons, labels, spin boxes, sliders, text areas, menus, menu bars, 
dockable widgets, <i>etc</i>.) to rapidly create a sophisticated GUI. To 
modify the BigBlobby GUI, use Qt Designer to open the BlobMainWindow.ui 
file found in the src/blobGui/BlobMainWindow.ui file. The ui file is 
actually an XML file that will be used by the Qt meta-object compiler to 
generate a BlobMainWindowUi object. The BlobMainWindow constructor 
creates an instance of the BlobMainWindowUi object, initializes it, and 
adds a few more components to it (the custom QGLWidget subclasses used 
for rendering images). 
</p>

<p>
After creating several additional helper classes, including the 
SourceGLWidget class that holds the CoreVision object, the very last thing 
the BlobMainWindow constructor does is create a class called 
BlobSignalsToSlots. The makeConnections() function of BlobSignalsToSlots 
is responsible for nearly all of the connections from GUI components 
(buttons, sliders, spin boxes, menu options, <i>etc</i>.) to slots 
(public functions) on BlobMainWindow or one of its helper classes. The 
Qt &quot;signals to slots&quot; concept is similar to the idea of 
listeners in Java or other languages, but tends to be a bit more 
flexible, as any signal can be connected to any slot, as long as the 
type and number of arguments is the same (see the Qt documentation for 
details). 
</p>

<p>
The BlobMainWindow also creates a few helper classes for responding to 
the View menu at the top of the GUI. The BlobMainWindowView object 
contains code for zooming in on either the main panel (Source and 
Tracked images) or the filters panel (Background, Smoothing, Highpass, 
and Amplify images). Initially, I placed the zooming code in the 
BlobMainWindow itself, but the class was simply getting too large, so 
all zooming related code was moved into BlobMainWindowView. To allow the 
user to select a custom zoom size not found in the View menu, there is a 
CustomZoomDialog class, which also happens to use Qt Designer to create 
the actual UI object. There is also a Help class that responds to the 
Help menu by opening an &quot;About...&quot; box with program credits 
(including CCV and Qt URLs) or opening a web browser to online help for 
BigBlobby/CCV. The Help class can also provide info on the current 
machine's GPU and OpenGL version. BlobMainWindow also creates an 
instance of BlobXmlSettings for operating the XML reader/writer classes 
that are discussed right below. 
</p>

<h3>XML File Input/Output/Validation and Error Handling</h3>

<p>
When the BigBlobby/CCV program first starts up, many of the program 
settings, including all of the numbers shown in the GUI, are read in 
from an XML configuration file, data/settings/DefaultBlobParams.xml. 
When the program is terminated by selecting Quit from the File menu or 
clicking on the close box of the main window, the current program 
settings are automatically written back out to the same file. Because 
this file can be hand edited, there is an obvious potential for errors. 
The exception classes shown in <b>Figure 3</b> are for handling those 
errors. 
</p>

<p>
<img src="uml/BlobExceptions.png" alt="UML for Exception Classes" />
</p>
<center><h3>Figure 3. UML Diagram for Exception Classes</h3></center>

<p>
The XML reader and writer classes, BlobParamsXmlReader and 
BlobParamsXmlWriter, are operated by the BlobXmlSettings class, which is 
a helper class for BlobMainWindow (see <b>Figure 1</b>). If the 
DefaultBlobParams.xml file is read successfully, a BlobParamsValidator 
object is handed back by the XML reader, and the parameters held by the 
validator object will be used to update both the GUI and the underlying 
CoreVision object that does the blob detection. When the program closes 
normally, the BlobXmlSettings object will obtain a validator object with 
the current CoreVision settings and pass it to the XML writer. In this 
case, the validator is simply a convenient way to pass around a bundle 
of parameters, no validation needs to be performed. 
</p>

<p>
If the XML reader, writer, or validator throws an exception, the 
exception will be caught by a try-catch block in BlobXmlSettings, and a 
dialog box will be used to inform the user of the problem. If the 
exception was thrown by the reader or validator when the program first 
opens, a default BlobXmlSettings object with hard-coded 
&quot;factory&quot; default settings will be used after warning the 
user. The user will also be informed that the File menu has an option to 
Exit (without saving) so that the DefaultBlobParams.xml file does not get 
modified.  This Exit (without saving) option is provided in case the user 
wants to fix the problem by hand editing, rather than accept the 
&quot;factory&quot; default settings.
</p>

<p>
The classes shown in <b>Figure 3</b> are &quot;structured&quot; 
exceptions, meaning that they store individual fields of information 
that can be presented to the user (in addition to the short message that 
all exceptions usually hold). If a BlobXmlReaderException is thrown, the 
dialog box will include the line number and column number of the error 
along with the filename and a message as to the nature of the error. If 
the validator throws an UnknownXmlTagException, the dialog box will 
include the name of the unrecognized XML tag along with the name of its 
parent tag. If the validator finds an obviously illegal value, a 
BlobParamsException is thrown, and the dialog box will show the illegal 
value and a statement on the expected value. In this context, examples 
of &quot;obviously illegal&quot; values would be something other than 
&quot;true&quot; or &quot;false&quot; where a boolean value is expected, 
or a negative number where only a positive integer or zero would make 
sense (imageTheshold, maxNumberBlobs, or any of the numeric filter 
settings). If a FileNotFound exception is thrown, the dialog box will 
inform the user of the path and name of the missing file. 
</p>

<h3>Where Exceptions are Not Used</h3>

<p> 
As soon as a screen calibration is finished by the CCV calibration code, 
the CCV CalibrationUtils class immediately saves the results out to an XML 
file, data/settings/calibration/calibration.xml. This XML file is also 
read in by the CCV CalibrationUtils class when the BlobCalibration class 
is initialized at program start up. This file is not intended to be 
hand-edited, and none of the error handling discussed above is applied.  
</p> 

<p> 
For the PS3 (Play Station 3) camera, the destructor of the CCV ofxPS3 
class saves out settings such as exposure and gain to the 
data/settings/cameras/cameras.xml file. This file is also read in by the 
ofxPS3 initialization routine. This is handled by the PS3 driver code, 
and does not appear to have any exception handling. Looking through the 
CCV code, there appear to be no try-catch blocks anywhere, except for an 
empty catch block in the ofAppGlutWindow.cpp file, and a few catch 
blocks in the OpenFrameworks ofSoundStream.cpp file that print error 
messages, but ofSoundStream.cpp does not appear to be used by CCV. 
</p> 

*******************************************************************************/

/*******************************************************************************
Note on using the Doxygen-generated API from within an SVN repository:

If the Doxygen-generated API is added to an SVN repository, the files will be
regarded as plain text by default, so in a web browser pointed at a repository
file, the html text would be seen, rather than the API in the form of a web
page.  To make the Doxygen-generated files in the repository show up as a proper
API web page, the mime types need to be set when uploaded through SVN.  
Automatically setting the mime-type can be accomplished by a few minor
changes to an SVN config file.

If you are using Tortoise SVN on Windows, right-click on your project to get a
context menu and then select TortoiseSVN --> Settings.  When the Settings dialog
box pops open, click on General (should be near the top left of the box), and
then click on the Edit button (should be near the bottom right of the box).
Clicking on Edit should open up a file named config.  You mightm want to make 
a backup copy of this file before modifying it.

If the line "enable-auto-props = yes" is commented out, uncomment it.
Then look for the section starting with "[auto-props]".  Add the
following lines to that section:

*.html = svn:mime-type=text/html
*.css = svn:mime-type=text/css
*.js = svn:mime-type=text/javascript
*.txt = svn:mime-type=text/plain;svn:eol-style=native
*.png = svn:mime-type=image/png
*.jpg = svn:mime-type=image/jpeg
*.pdf = svn:mime-type=application/pdf
*.jpeg = svn:mime-type=image/jpg
*.tiff = svn:mime-type=image/tiff
*.tif = svn:mime-type=image/tiff

This change will only apply to files as they are commited to the repository,
so if you have already committed some Doxygen files, you might need to delete
them and then add them back to get the mime type set correctly.  If you now
point a web browser to the docs/html/index.html file in the SVN repository,
you should see the Doxygen Main Page as a web page, rather than the source text.
*******************************************************************************/
