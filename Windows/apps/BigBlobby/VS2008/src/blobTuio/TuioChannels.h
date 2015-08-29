////////////////////////////////////////////////////////////////////////////////
// TuioChannels
//
// PURPOSE: Holds the TUIO objects used for sending TUIO messages (UDP, 
//          Flash XML, or Binary TCP).
//
// CREATED: 5/13/2014 (based on code from TUIO class of CCV 1.3).
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
#ifndef BLOBTUIO_TUIOCHANNELS_H
#define BLOBTUIO_TUIOCHANNELS_H

#include "blobTuio/TuioUdpChannel.h"
#include "blobTuio/FlashXmlChannel.h"
#include "blobTuio/BinaryTcpChannel.h"
#include "blobEnums/TuioProfileEnum.h"
#include <string>

namespace blobUtils { class FunctionTimer; }

/***************************************************************************//** 
Contains classes for sending TUIO messages.
*******************************************************************************/
namespace blobTuio 
{
    /***********************************************************************//**
    Holds the TUIO objects used for sending TUIO messages (UDP, Flash XML, or 
    Binary TCP).
    ***************************************************************************/
    class TuioChannels
    {
    public:
        TuioChannels();
        ~TuioChannels();

        void sendTuio( std::map<int, blobTracker::Blob> * blobs );
        std::string getOutputSummary();

        void setupTuio( const std::string & localHost,
                        int udpChannelOnePort,
                        int udpChannelTwoPort,
                        int flashXmlChannelPort,
                        int binaryTcpChannelPort );

        void setCameraWidthAndHeight( double width, double height );
        void setUdpProfile( blobEnums::TuioProfileEnum::Enum profile );
        blobEnums::TuioProfileEnum::Enum getUdpProfile();
        void setFlashXmlTuioProfile( blobEnums::TuioProfileEnum::Enum profile );
        blobEnums::TuioProfileEnum::Enum getFlashXmlProfile();

        void setNetworkSilentMode( bool b );
        void setUdpChannelOne( bool b );
        void setUdpChannelTwo( bool b );
        void setFlashXmlChannel( bool b );
        void setBinaryTcpChannel( bool b );

        bool useNetworkSilentMode();
        bool useUdpChannelOne();
        bool useUdpChannelTwo();
        bool useFlashXmlChannel();
        bool useBinaryTcpChannel();

        std::string getLocalHost();
        int getUdpChannelOnePort();
        int getUdpChannelTwoPort();
        int getFlashXmlChannelPort();
        int getBinaryTcpChannelPort();

        double calculateAverageTime();

    private:
        std::string udpTuioProfile();
        std::string flashXmlTuioProfile();
        std::string intToStr( int n );
        bool hasAtLeastOneChannelActive();

        blobTuio::TuioUdpChannel tuioUdpChannelOne_,
                                 tuioUdpChannelTwo_;
        blobTuio::FlashXmlChannel flashXmlChannel_;
        blobTuio::BinaryTcpChannel binaryTcpChannel_;
        blobEnums::TuioProfileEnum::Enum udpTuioProfile_,
                                         flashXmlTuioProfile_;
        blobUtils::FunctionTimer * processTimer_;
        std::string localHost_;
        bool silentMode_,
             useUdpChannelOne_,
             useUdpChannelTwo_,
             useFlashXmlChannel_,
             useBinaryTcpChannel_,
             channelsNotInitialized_;
    };
}

#endif
