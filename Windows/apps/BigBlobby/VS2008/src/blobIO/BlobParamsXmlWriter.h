////////////////////////////////////////////////////////////////////////////////
// BlobParamsXmlWriter
//
// Purpose: Reads program settings from a BlobParamsValidator object and writes
//          them to an XML file.
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
#ifndef BLOBIO_BLOBPARAMSXMLWRITER
#define BLOBIO_BLOBPARAMSXMLWRITER

#include "blobIO/DebugPrint.h"
#include <QString>
#include <string>
#include <fstream>

namespace blobIO { class BlobParamsValidator; }

namespace blobIO
{
    /***********************************************************************//**
    Knows how to read program settings from a BlobParamsValidator object and 
    write them to an XML file.

    The name of the file to write is obtained from the validator object.
    The default XML file that is written when the program closes is
    &quot;data/settings/DefaultBlobParams.xml&quot;.  <br />

    Note that calibration settings and camera-specific settings (such as gain
    and exposure) are written by CCV to other XML files in the data directory.
    ***************************************************************************/
    class BlobParamsXmlWriter
    {
        public:
            BlobParamsXmlWriter();
            virtual ~BlobParamsXmlWriter();

            void write( blobIO::BlobParamsValidator * validator );
            void printXmlToStandardOut( blobIO::BlobParamsValidator * validator );

        private:
            // Private Helper Functions for write()
            QString getSettingsAsXml( blobIO::BlobParamsValidator * validator );
            QString getVideoCameraXml( blobIO::BlobParamsValidator * validator );
            QString getBlobsXml( blobIO::BlobParamsValidator * validator );
            QString getBackgroundXml( blobIO::BlobParamsValidator * validator );
            QString getFiltersXml( blobIO::BlobParamsValidator * validator );
            QString getNetworkXml( blobIO::BlobParamsValidator * validator );
            QString getExperimentalXml( blobIO::BlobParamsValidator * validator );
            QString createXmlFromBool( QString tag, bool b );
            QString createXmlFromInt( QString tag, int n );
            QString createXmlFromDouble( QString tag, double n );
            QString createXmlFromString( QString tag, QString theValue );

            // Debugging Utility that Writes to Standard Out
            blobIO::DebugPrint debug_;
    };
}

#endif
