////////////////////////////////////////////////////////////////////////////////
// BlobParamsXmlReader
//
// Purpose: Knows how to read a set of program settings from an XML file and 
//          return a BlobParamsValidator object.
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
#ifndef BLOBIO_BLOBPARAMSXMLREADER
#define BLOBIO_BLOBPARAMSXMLREADER

#include "blobExceptions/BlobParamsException.h"
#include "blobExceptions/UnknownXmlTagException.h"
#include "blobIO/DebugPrint.h"
#include <QtCore/QByteArray>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <string>
#include <vector>

namespace blobIO { class BlobParamsValidator; }

namespace blobIO
{
    /***********************************************************************//**
    Knows how to read program settings from an XML file and return a 
    BlobParamsValidator object.

    The XML file given to the read() function will be be parsed for the tags
    seen in the &quot;data/settings/DefaultBlobParams.xml&quot; file.  An
    exception will be thrown if the file cannot be found, if a tag is 
    unrecognized, or if the value for a tag cannot be converted to the 
    correct data type (usually an int or a bool).
    <br />

    Note that calibration settings and camera-specific settings (such as gain
    and exposure) are stored by CCV in other files that are read elsewhere in 
    the program.
    ***************************************************************************/
    class BlobParamsXmlReader
    {
        public:
            static const std::string ASTERISKS_FOR_BANNER;

            BlobParamsXmlReader();
            ~BlobParamsXmlReader();
            blobIO::BlobParamsValidator * read( const std::string & xmlFile );
            
            bool hasUnknownXmlTagExceptions();
            std::vector<blobExceptions::UnknownXmlTagException> unknownXmlTagExceptions();

            bool hasBlobParamsExceptions();
            std::vector<blobExceptions::BlobParamsException> blobParamsExceptions();

        private:
            // Private Helper Functions for read()
            blobIO::BlobParamsValidator * extractParams( QDomDocument & doc, const std::string & xmlFile );
            void storeParams( QDomElement element, blobIO::BlobParamsValidator * validator );
            void storeCameraParams( QDomNode & node, blobIO::BlobParamsValidator * validator );
            void storeBlobParams( QDomNode & node, blobIO::BlobParamsValidator * validator );
            void storeBackgroundParams( QDomNode & node, blobIO::BlobParamsValidator * validator );
            void storeFilterParams( QDomNode & node, blobIO::BlobParamsValidator * validator );
            void storeNetworkParams( QDomNode & node, blobIO::BlobParamsValidator * validator );
            void storeExperimentalParams( QDomNode & node, blobIO::BlobParamsValidator * validator );

            std::vector<blobExceptions::UnknownXmlTagException> unknownXmlTagExceptions_;
            std::vector<blobExceptions::BlobParamsException> blobParamsExceptions_;

            // Debugging Utility that Writes to Standard Out
            blobIO::DebugPrint debug_;
    };
}

#endif
