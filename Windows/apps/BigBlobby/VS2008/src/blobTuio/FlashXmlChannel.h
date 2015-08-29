////////////////////////////////////////////////////////////////////////////////
// FlashXmlChannel
//
// PURPOSE: Uses an ofxTCPServer for sending out Flash XML messages.
//
// CREATED: 5/14/2014 (based on code from TUIO class of CCV 1.3).
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2009 NUI Group/Inc. All rights reserved.  (CCV 1.3 classes)
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
#ifndef BLOBTUIO_FLASHXMLCHANNEL_H
#define BLOBTUIO_FLASHXMLCHANNEL_H

#include "blobTuio/OutputChannel.h"
#include "blobEnums/TuioProfileEnum.h"

namespace blobTuio
{
    class FlashXmlChannel : public OutputChannel
    {
	public:	
        static const blobEnums::TuioProfileEnum::Enum DEFAULT_TUIO_2D_PROFILE;
        static const double DEFAULT_CAMERA_WIDTH,
                            DEFAULT_CAMERA_HEIGHT;

		FlashXmlChannel();
		~FlashXmlChannel();

        virtual void setup( const std::string & localHost, int port );
        void setTuioProfile( blobEnums::TuioProfileEnum::Enum profile );
        void setCameraWidthAndHeight( double width, double height );
		virtual void sendTUIO( std::map<int, blobTracker::Blob> * blobs );

    private:
        void sendMessage( std::map<int, blobTracker::Blob> * blobs, 
                          const std::string & profileName,
                          blobEnums::TuioProfileEnum::Enum profile );
        void sendAliveMessage( const std::string & profileName );

        void add2DcurProfile( std::string & blobMessage, blobTracker::Blob & blob );
        void add2DblbProfile( std::string & blobMessage, blobTracker::Blob & blob );

        ofxTCPServer tcpServer_;
        int frameseq_;
        blobEnums::TuioProfileEnum::Enum tuioProfile_;
        std::string tuioProfileName_;
        double cameraWidth_,
               cameraHeight_,
               cameraArea_;
    };
}

#endif
