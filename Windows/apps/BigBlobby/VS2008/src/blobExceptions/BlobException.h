////////////////////////////////////////////////////////////////////////////////
// BlobException
//
// Purpose: Superclass for all custom exceptions used by BigBlobby.
//
// Created: 5/18/2011
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
#ifndef BLOBEXCEPTIONS_BLOBEXCEPTION_H
#define BLOBEXCEPTIONS_BLOBEXCEPTION_H

#include <string>

/***************************************************************************//**
Contains custom exception classes written for BigBlobby.

Currently (7/9/2013), these exceptions are only for errors that can occur while 
reading/writing XML files with the BlobXmlSettings class.  This error handling 
was added because the user may want to hand edit the
&quot;data/settings/DefaultBlobParams.xml&quot; file, which makes the
probability of a minor XML error too high to ignore.  If there is an error 
in the DefaultBlobParams.xml file, the user should be informed via a dialog box,
rather than have the program crash or just quietly ignore the error while
substituting a default value.
<br />

The underlying CCV (Community Core Vision) 1.3 blob detection code does not 
appear to use any try-catch style error handling.  The only catch blocks
that show up when searching the project for &quot;catch&quot; are an empty 
catch-block in ofAppGlutWindow.cpp and some functional catch-blocks in 
ofSoundStream.cpp of the OpenFrameworks library (and that class is not used 
by CCV).
*******************************************************************************/
namespace blobExceptions
{
    /***********************************************************************//**
    Superclass for all custom exceptions used by BigBlobby.

    All custom exceptions are expected to have at the very least some kind of
    an error message as well as some information about where the exception was
    thrown from.
    ***************************************************************************/
    class BlobException
    {
        public:
            BlobException( const std::string & source, const std::string & message );
            virtual ~BlobException();

            std::string getSource();
            std::string getMessage();

        private:
            std::string source,
                        message;
    };
}

#endif
