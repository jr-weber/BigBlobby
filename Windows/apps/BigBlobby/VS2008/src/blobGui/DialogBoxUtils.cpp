////////////////////////////////////////////////////////////////////////////////
// DialogBoxUtils
//
// Purpose: This utility class knows how to extract info from any subclass 
//          of BlobException and format the info for presentation in a modal 
//          warning-style QMessageBox.
//
// Created:  6/6/2011
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
#include "blobGui/DialogBoxUtils.h"
#include <QMessageBox>

using blobGui::DialogBoxUtils;

DialogBoxUtils::DialogBoxUtils()
{
}

DialogBoxUtils::~DialogBoxUtils()
{
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the BlobException.
*******************************************************************************/
void DialogBoxUtils::warnUser( blobExceptions::BlobException e )
{
    QString title( "Warning: BlobException" );
    QString message( e.getMessage().c_str() );
    openWarningMessageBox( title, message );
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the UnknownXmlTagException.
*******************************************************************************/
void DialogBoxUtils::warnUser( blobExceptions::UnknownXmlTagException e )
{
    QString title( "Warning: UnknownXmlTagException" );
    std::string message = e.getMessage() + "\n";
    message += "Unknown Tag = <" + e.getTagName() + ">\n";
    message += "Parent Tag = <" + e.getParentTagName() + ">\n\n";
    message += "File:  " + e.getTagSource();
    openWarningMessageBox( title, QString( message.c_str() ) );
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the BlobParamsException.
*******************************************************************************/
void DialogBoxUtils::warnUser( blobExceptions::BlobParamsException e )
{
    QString title( "Warning" );
    std::string message = e.getMessage() + "\n";
    message += "Tag = <" + e.getParamName() + ">\n";
    message += "Expected value = " + e.getExpectedValue() + "\n";
    message += "Actual value = " + e.getParamValue() + "\n\n";
    message += "File:  " + e.getParamsSource();
    openWarningMessageBox( title, QString( message.c_str() ) );
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the BlobIOException.
*******************************************************************************/
void DialogBoxUtils::warnUser( blobExceptions::BlobIOException e )
{
    QString title( "Warning" );
    QString message( e.getMessage().c_str() );
    openWarningMessageBox( title, message );
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the FileNotFoundException.
*******************************************************************************/
void DialogBoxUtils::warnUser( blobExceptions::FileNotFoundException e )
{
    QString title( "Warning" );
    QString message( (e.getMessage() + e.getFilename()).c_str() );
    openWarningMessageBox( title, message );
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the BlobXmlReaderException.
*******************************************************************************/
void DialogBoxUtils::warnUser( blobExceptions::BlobXmlReaderException e )
{
    QString title( "Warning" );
    QString message( (e.getMessage() + e.getXmlFilename()).c_str() );
    message.append( "\nLine = " + QString::number( e.getLineNumber() ) );
    message.append( " and column = " + QString::number( e.getColNumber() ) + "\n" );
    message.append( e.getDocErrors().c_str() );
    openWarningMessageBox( title, message );
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the BlobXmlWriterException.
*******************************************************************************/
void DialogBoxUtils::warnUser( blobExceptions::BlobXmlWriterException e )
{
    QString title( "Warning" );
    QString message( (e.getMessage() + e.getXmlFilename()).c_str() );
    openWarningMessageBox( title, message );
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the list of UnknownXmlTagExceptions.
*******************************************************************************/
void DialogBoxUtils::warnUser( std::vector<blobExceptions::UnknownXmlTagException> & errors )
{
    int length = errors.size();
    QString title( "Warning" );

    if( length > 0 ) {
        std::string message = "Error reading file '";
        message += errors.at(0).getTagSource() + "'\n";
        message += (length == 1) ? "Invalid XML tag:\n\n" : "Invalid XML tags:\n\n";

        for( int i = 0; i < length; ++i ) {
            message += "Unknown Tag = <" + errors.at(i).getTagName() + ">\n";
            message += "Parent Tag = <" + errors.at(i).getParentTagName() + ">\n\n";
        }
        message += "Invalid tags are ignored.";
        openWarningMessageBox( title, QString( message.c_str() ) );
    }
}

/***************************************************************************//**
Opens a QMessageBox that displays info from the list of BlobParamsExceptions.
*******************************************************************************/
void DialogBoxUtils::warnUser( std::vector<blobExceptions::BlobParamsException> & errors )
{
    int length = errors.size();
    QString title( "Warning" );

    if( length > 0 ) {
        std::string message = "Error reading file '";
        message += errors.at(0).getParamsSource() + "'\n";
        message += (length == 1) ? "Invalid tag value:\n\n" : "Invalid tag values:\n\n";

        for( int i = 0; i < length; ++i ) {
            message += "Tag = <" + errors.at(i).getParamName() + ">\n";
            message += "Expected value = " + errors.at(i).getExpectedValue() + "\n";
            message += "Actual value = " + errors.at(i).getParamValue() + "\n\n";
        }
        message += "A default value was used instead.";
        openWarningMessageBox( title, QString( message.c_str() ) );
    }
}

/***************************************************************************//**
Displays the message in a QMessageBox.
*******************************************************************************/
void DialogBoxUtils::warnUser( const QString & message )
{
    QString title( "Warning" );
    openWarningMessageBox( title, message );
}

/*******************************************************************************
This private helper function expects a title that will go at the top of the 
dialog box and a message that is already formatted.  

The message can include newline characters.  A warning icon will be displayed
next to the message, and the dialog box is modal (blocks all user input to the
GUI until the user clicks the OK button to closes the dialog box).
*******************************************************************************/
void DialogBoxUtils::openWarningMessageBox( QString title, QString message )
{
    QMessageBox messageBox;
    messageBox.setWindowTitle( title );
    messageBox.setText( message );
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.exec();
}
