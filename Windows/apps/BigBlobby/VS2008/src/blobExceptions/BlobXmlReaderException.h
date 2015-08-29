////////////////////////////////////////////////////////////////////////////////
// BlobXmlReaderException.h
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
#ifndef BLOBEXCEPTIONS_BLOBXMLREADEREXCEPTION_H
#define BLOBEXCEPTIONS_BLOBXMLREADEREXCEPTION_H

#include "blobExceptions/BlobIOException.h"

namespace blobExceptions
{
    /***********************************************************************//**
    For reporting errors found when reading an XML file for program settings.

    This class stores several fields of information that should help the user
    find the error and correct it.
    ***************************************************************************/
    class BlobXmlReaderException : public blobExceptions::BlobIOException
    {
        public:
            static const char * DEFAULT_XML_READER_MESSAGE;

            BlobXmlReaderException( const std::string & xmlFilename,
                                    const std::string & docErrors,
                                    int lineNumber,
                                    int colNumber,
                                    const std::string & source,
                                    const std::string & message = DEFAULT_XML_READER_MESSAGE );
            virtual ~BlobXmlReaderException();

            std::string getXmlFilename();
            std::string getDocErrors();
            int getLineNumber();
            int getColNumber();

        private:
            std::string xmlFilename;
            std::string docErrors;
            int lineNumber,
                colNumber;
    };
}

#endif
