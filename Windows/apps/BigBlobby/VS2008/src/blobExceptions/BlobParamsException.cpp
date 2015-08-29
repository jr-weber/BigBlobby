////////////////////////////////////////////////////////////////////////////////
// BlobParamsException.h
//
// Purpose: For reporting that a string could not be converted to a legal
//          value for a BlobParamsValidator attribute.
//
// Created: 6/18/2011
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
#include "blobExceptions/BlobParamsException.h"

using blobExceptions::BlobParamsException;

const std::string BlobParamsException::DEFAULT_PARAM_MESSAGE = "Blob parameter not valid: ";

/***************************************************************************//**
The paramName and paramValue args refer to the name and value for an XML tag.

The expectedValue arg could be a short statement about a range of values that
would be acceptable.  The paramsSource arg is expected to be the name of an
XML file, whereas the source arg is the name of the class and function that
threw the exception.  The message arg should, ideally, be short enough to 
display in a dialog box.
*******************************************************************************/
BlobParamsException::BlobParamsException( const std::string & paramName, 
                                          const std::string & paramValue, 
                                          const std::string & expectedValue,
                                          const std::string & paramsSource,
                                          const std::string & source, 
                                          const std::string & message 
                                          /* = DEFAULT_PARAM_MESSAGE  */ ) :
  BlobException( source, message ),
  paramName( paramName ),
  paramValue( paramValue ),
  expectedValue( expectedValue ),
  paramsSource( paramsSource )
{
}

BlobParamsException::~BlobParamsException()
{
}

/***************************************************************************//**
Returns the name of an XML tag for a program setting.
*******************************************************************************/
std::string BlobParamsException::getParamName()
{
    return paramName;
}

/***************************************************************************//**
Returns the value for an XML tag for a program setting.
*******************************************************************************/
std::string BlobParamsException::getParamValue()
{
    return paramValue;
}

/***************************************************************************//**
Returns a short statement about what would be an acceptable value or range of
values for a program setting.
*******************************************************************************/
std::string BlobParamsException::getExpectedValue()
{
    return expectedValue;
}

/***************************************************************************//**
Returns the name of the XML file that was being read when the exception
occurred.
*******************************************************************************/
std::string BlobParamsException::getParamsSource()
{
    return paramsSource;
}
