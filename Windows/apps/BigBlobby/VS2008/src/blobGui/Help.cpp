////////////////////////////////////////////////////////////////////////////////
// Help
//
// PURPOSE: Provides services needed by the Help menu: an About box and an
//          option to open a web browser to an online help page for BigBlobby.
//
// CREATED:  6/27/2013
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
#include "blobGui/Help.h"
#include "blobGui/SourceGLWidget.h"
#include <QString>
#include <QMessagebox>
#include <QPixmap>
#include <QDesktopServices>

using blobGui::Help;

const QString Help::BIGBLOBBY_ICON( ":/QResources/images/BigBlobbyIcon-100x100.png" );
const QUrl Help::BIGBLOBBY_HELP_URL( "https://sites.google.com/site/bigblobbygui4ccv/getting-started" );
const QString Help::ABOUT_BOX_MSG(
    "BigBlobby is a GUI (Graphical User Interface) written with the Qt framework "
    "and placed in front of the free, open source CCV 1.3 blob detection program "
    "for multi-touch surfaces.<br /><br />"

    "<b>BigBlobby version 0.9</b><br />"
    "Written by Joe Weber (joe.weber77@gmail.com)<br />"
    "https://sites.google.com/site/bigblobbygui4ccv/home<br />"
    "Source code is posted on GitHub<br />"
    "GNU Lesser General Public License version 2.1<br /><br />"

    "<b>CCV (Community Core Vision) version 1.3</b><br />"
    "Written by NUI Group: http://ccv.nuigroup.com<br />"
    "GNU Lesser General Public License version 2.1<br /><br />"

    "<b>Qt Framework version 4.7.3</b><br />"
    "Qt Project: http://qt-project.org<br />"
    "GNU Lesser General Public License version 2.1"
);

/***************************************************************************//**
The SourceGLWidget argument is needed because of the openOpenGLInfoDialogBox(), 
which will need access to OpenGL to obtain the OpenGL version number.
*******************************************************************************/
Help::Help( blobGui::SourceGLWidget * sourceGLWidget ) :
  sourceGLWidget_( sourceGLWidget ),
  debug_( false )
{
}

Help::~Help()
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                             Slots for Help Menu
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Opens an About box with the BigBlobby icon and info on BigBlobby and the
Community Core Vision open source code project.  URLs are also given for more 
info on BigBlobby, CCV, and the Qt Framework Project.  The use of the GNU
Lesser General Public License is also mentioned.
*******************************************************************************/
void Help::openAboutBox()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle( "About BigBlobby" );
    messageBox.setIconPixmap( QPixmap( BIGBLOBBY_ICON ) );
    QString msg( ABOUT_BOX_MSG );
    messageBox.setTextFormat( Qt::RichText );
    messageBox.setText( msg );
    messageBox.exec();
}

/***************************************************************************//**
Opens the default web browser to a web page with help info.  If the default web
browser cannot be found, a warning message box will inform the user and give
the URL for BigBlobby online help.
*******************************************************************************/
void Help::openOnlineHelp()
{
    bool success = QDesktopServices::openUrl( BIGBLOBBY_HELP_URL );

    if( !success ) {
        QMessageBox messageBox;
        messageBox.setWindowTitle( "Web Browser Not Found!" );
        messageBox.setIcon( QMessageBox::Warning );

        QString msg( "The default desktop web browser could not be found." );
        msg += "<br />For online help, point a web browser to <br /><br />";
        messageBox.setText( msg + BIGBLOBBY_HELP_URL.toString() );
        messageBox.exec();
    }
}

/***************************************************************************//**
Opens a dialog box with the OpenGL version and info on the graphics card.
A warning will be given if the OpenGL version number is less than 2.0.
*******************************************************************************/
void Help::openOpenGLInfoDialogBox()
{
    QMessageBox messageBox;
    messageBox.setText( getOpenGLInfo() );
    messageBox.setWindowTitle( "OpenGL Info" );
    messageBox.exec();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//              Private Helper Functions for Getting OpenGL Info
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Returns the OpengGL version number along with info on the graphics card.
The message also notes that OpenGL 2.0 or greater is required and gives a
warning message if the version number is less than 2.
*******************************************************************************/
QString Help::getOpenGLInfo()
{
    // Make OpenGL context current.
    sourceGLWidget_->makeCurrent();
    QString version( reinterpret_cast<const char *>(glGetString( GL_VERSION )) );
    QString vendor( reinterpret_cast<const char *>(glGetString( GL_VENDOR )) );
    QString renderer( reinterpret_cast<const char *>(glGetString( GL_RENDERER )) );
    QString info( "OpenGL:              Version " + version + "\n"
                + "Graphics Card:    " + renderer + "\n"
                + "GPU Vendor:       " + vendor + "\n\n"
                + "Community Core Vision 1.3 requires OpenGL 2.0 or higher." );

    if( !isOpenGLv2Supported( version ) ) {
        info += "\n\nYour machine appears to have an earlier version of OpenGL\n"
                "and will probably not be able to run BigBlobby/CCV correctly.";
    }
    return info;
}

/***************************************************************************//**
Returns true if the first character in the version argument is 2 or greater.
Otherwise, returns false.
*******************************************************************************/
bool Help::isOpenGLv2Supported( const QString & version )
{
    QString v = version.trimmed();

    if( v.size() > 0 ) {
        v = v.left( 1 );
        int n = v.toInt();
        return (n >= 2);
    }
    return false;
}
