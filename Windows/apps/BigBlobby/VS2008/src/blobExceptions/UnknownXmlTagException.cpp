////////////////////////////////////////////////////////////////////////////////
// UnknownXmlTagException
//
// Purpose: For reporting an unrecognized tag in an XML file.
//
// Created: 6/7/2011
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
#include "blobExceptions/UnknownXmlTagException.h"

using blobExceptions::UnknownXmlTagException;

const std::string UnknownXmlTagException::UNKNOWN_XML_TAG_MESSAGE = "Unrecognized XML tag: ";

/***************************************************************************//**
If the unrecognized XML tag does not have a parent tag that it is nested
within, then an empty string (or &quot;none&quot;) would be fine as an
argument for parentTagName.  The tagSource is expected to be the name of an 
XML file, whereas the source arg is the name of the class and function that 
threw the exception. The message arg should, ideally, be short enough to
display in a dialog box.
*******************************************************************************/
UnknownXmlTagException::UnknownXmlTagException( const std::string & tagName,
                                                const std::string & parentTagName,
                                                const std::string & tagSource,
                                                const std::string & source, 
                                                const std::string & message 
                                                /* = UNKNOWN_XML_TAG_MESSAGE  */ ) :
  BlobException( source, message ),
  tagName( tagName ),
  parentTagName( parentTagName ),
  tagSource( tagSource )
{
}

UnknownXmlTagException::~UnknownXmlTagException()
{
}

/***************************************************************************//**
Returns the name of the unrecognized XML tag.
*******************************************************************************/
std::string UnknownXmlTagException::getTagName()
{
    return tagName;
}

/***************************************************************************//**
Returns the name of the parent tag that the unrecognized XML tag was nested
inside of (or an empty string or &quot;none&quot; if there was no parent tag.
*******************************************************************************/
std::string UnknownXmlTagException::getParentTagName()
{
    return parentTagName;
}

/***************************************************************************//**
Returns the name of the XML file that was being read when the unrecognized XML
tag was found.
*******************************************************************************/
std::string UnknownXmlTagException::getTagSource()
{
    return tagSource;
}
