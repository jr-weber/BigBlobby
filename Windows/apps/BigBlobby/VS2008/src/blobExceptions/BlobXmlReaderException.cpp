////////////////////////////////////////////////////////////////////////////////
// BlobXmlReaderException.cpp
//
// Purpose: For reporting an error found when reading an XML file for program 
//          settings.
//
// Created: 5/23/2011
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
#include "blobExceptions/BlobXmlReaderException.h"

using blobExceptions::BlobXmlReaderException;

const char * BlobXmlReaderException::DEFAULT_XML_READER_MESSAGE = "Error reading XML file:  ";

/***************************************************************************//**
In addition to the name of the XML file, the constructor expects any available
info on the nature of the error along with the line and column number where the
error occurred.

The source arg is for the name of the class and function that threw the 
exception (generally only of interest to a programmer), while the message should
be a short message suitable for a dialog box to be presented to the user.
*******************************************************************************/
BlobXmlReaderException::BlobXmlReaderException( const std::string & xmlFilename,
                                                const std::string & docErrors,
                                                int lineNumber,
                                                int colNumber,
                                                const std::string & source,
                                                const std::string & message 
                                                /* = DEFAULT_XML_READER_MESSAGE */ ) :
  BlobIOException( source, message ),
  xmlFilename( xmlFilename ),
  docErrors( docErrors ),
  lineNumber( lineNumber ),
  colNumber( colNumber )
{
}

BlobXmlReaderException::~BlobXmlReaderException()
{
}

/***************************************************************************//**
Returns the name of the XML file that was being written when the error occurred.
*******************************************************************************/
std::string BlobXmlReaderException::getXmlFilename()
{
    return xmlFilename;
}

/***************************************************************************//**
Returns the errors reported while trying to create a Document object.
*******************************************************************************/
std::string BlobXmlReaderException::getDocErrors()
{
    return docErrors;
}

/***************************************************************************//**
Returns the line number where the error occurred.
*******************************************************************************/
int BlobXmlReaderException::getLineNumber()
{
    return lineNumber;
}

/***************************************************************************//**
Returns the column number where the error occurred.
*******************************************************************************/
int BlobXmlReaderException::getColNumber()
{
    return colNumber;
}
