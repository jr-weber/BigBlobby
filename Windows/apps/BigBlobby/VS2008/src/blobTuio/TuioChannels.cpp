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
#include "blobTuio/TuioChannels.h"
#include "blobUtils/FunctionTimer.h"
#include <sstream>

using blobTuio::TuioChannels;

TuioChannels::TuioChannels() :
  tuioUdpChannelOne_(),
  tuioUdpChannelTwo_(),
  flashXmlChannel_(),
  binaryTcpChannel_(),
  udpTuioProfile_( blobTuio::TuioUdpChannel::DEFAULT_TUIO_2D_PROFILE ),
  flashXmlTuioProfile_( blobTuio::FlashXmlChannel::DEFAULT_TUIO_2D_PROFILE ),
  processTimer_( new blobUtils::FunctionTimer() ),
  localHost_( "localhost" ),
  silentMode_( false ),
  useUdpChannelOne_( true ),
  useUdpChannelTwo_( true ),
  useFlashXmlChannel_( true ),
  useBinaryTcpChannel_( false ),
  channelsNotInitialized_( true )
{
}

TuioChannels::~TuioChannels()
{
    delete processTimer_;
}

void TuioChannels::sendTuio( std::map<int, blobTracker::Blob> * blobs )
{
    processTimer_->startTiming();
    
    if( !silentMode_ ) {
        if( useUdpChannelOne_ )    { tuioUdpChannelOne_.sendTUIO( blobs ); }
        if( useUdpChannelTwo_ )    { tuioUdpChannelTwo_.sendTUIO( blobs ); }
        if( useFlashXmlChannel_ )  { flashXmlChannel_.sendTUIO( blobs ); }
        if( useBinaryTcpChannel_ ) { binaryTcpChannel_.sendTUIO( blobs ); }
    }
    processTimer_->stopTiming();
}

std::string TuioChannels::getOutputSummary()
{
    std::string summary = "Host: " + getLocalHost() + "\n\n";
    
    if( silentMode_ ) {
        summary += "Silent Mode\n(No Output)\n";
    }
    else if( hasAtLeastOneChannelActive() ) {
        summary += "Output:        Port:     TUIO Profile\n";

        if( useUdpChannelOne_ ) {
            summary += "TUIO UDP:   " 
                     + intToStr( getUdpChannelOnePort() ) + ":   "
                     + udpTuioProfile();
        }
        if( useUdpChannelTwo_ ) {
            summary += "TUIO UDP:   " 
                     + intToStr( getUdpChannelTwoPort() ) + ":   "
                     + udpTuioProfile();
        }
        if( useFlashXmlChannel_ ) {
            summary += "Flash XML:   "
                     + intToStr( getFlashXmlChannelPort() ) + ":   "
                     + flashXmlTuioProfile();
        }
        if( useBinaryTcpChannel_ ) {
            summary += "Binary TCP: " 
                     + intToStr( getBinaryTcpChannelPort() ) 
                     + ":   2Dcur\n";
        }
    }
    else {
        summary += "No active output.\n";
    }
    return summary;
}

bool TuioChannels::hasAtLeastOneChannelActive()
{
    return useUdpChannelOne_ 
         | useUdpChannelTwo_ 
         | useFlashXmlChannel_ 
         | useBinaryTcpChannel_;
}

std::string TuioChannels::udpTuioProfile()
{
    std::string profile;
    
    if( udpTuioProfile_ == blobEnums::TuioProfileEnum::TUIO_2D_CURSOR ) {
        profile = "2Dcur\n";
    }
    else if( udpTuioProfile_ == blobEnums::TuioProfileEnum::TUIO_2D_BLOB ) {
        profile = "2Dblb\n";
    }
    else if( udpTuioProfile_ == blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB ) {
        profile = "2Dcur & 2Dblb\n";
    }
    return profile;
}

std::string TuioChannels::flashXmlTuioProfile()
{
    std::string profile;
    
    if( flashXmlTuioProfile_ == blobEnums::TuioProfileEnum::TUIO_2D_CURSOR ) {
        profile = "2Dcur\n";
    }
    else if( flashXmlTuioProfile_ == blobEnums::TuioProfileEnum::TUIO_2D_BLOB ) {
        profile = "2Dblb\n";
    }
    else if( flashXmlTuioProfile_ == blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB ) {
        profile = "2Dcur & 2Dblb\n";
    }
    return profile;
}

std::string TuioChannels::intToStr( int n )
{
    std::stringstream out;
    out << n;
    return out.str();
}

/***************************************************************************//**
Set up will only work the first time it is called during a program run.  If 
called a second time, it remembers that it has already been called, and will
not attempt to reset the servers.  The problem is that the server setup code  
is not currently configured to clean up and shut down first (i.e., the server 
setup appears to have been written based on the assumption that it will be 
called only once at the beginning of a program run).
*******************************************************************************/
void TuioChannels::setupTuio( const std::string & localHost,
                              int udpChannelOnePort,
                              int udpChannelTwoPort,
                              int flashXmlChannelPort,
                              int binaryTcpChannelPort )
{
    if( channelsNotInitialized_ ) {
        channelsNotInitialized_ = false;
        localHost_ = localHost;
        tuioUdpChannelOne_.setup( localHost, udpChannelOnePort );
        tuioUdpChannelTwo_.setup( localHost, udpChannelTwoPort );
        flashXmlChannel_.setup( localHost_, flashXmlChannelPort );
        binaryTcpChannel_.setup( localHost_, binaryTcpChannelPort );
        binaryTcpChannel_.setIncludeHeightAndWidth( true );
    }
}

void TuioChannels::setCameraWidthAndHeight( double width, double height )
{
    tuioUdpChannelOne_.setCameraWidthAndHeight( width, height );
    tuioUdpChannelTwo_.setCameraWidthAndHeight( width, height );
}

void TuioChannels::setUdpProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    udpTuioProfile_ = profile;
    tuioUdpChannelOne_.setTuioProfile( udpTuioProfile_ );
    tuioUdpChannelTwo_.setTuioProfile( udpTuioProfile_ );
}

blobEnums::TuioProfileEnum::Enum TuioChannels::getUdpProfile()
{
    return udpTuioProfile_;
}

void TuioChannels::setFlashXmlTuioProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    flashXmlTuioProfile_ = profile;
    flashXmlChannel_.setTuioProfile( flashXmlTuioProfile_ );
}

blobEnums::TuioProfileEnum::Enum TuioChannels::getFlashXmlProfile()
{
    return flashXmlTuioProfile_;
}

int TuioChannels::getBinaryTcpChannelPort()
{
    return binaryTcpChannel_.getPort();
}

void TuioChannels::setNetworkSilentMode( bool b )
{
    silentMode_ = b;
}

void TuioChannels::setUdpChannelOne( bool b )
{
    useUdpChannelOne_ = b;
}

void TuioChannels::setUdpChannelTwo( bool b )
{
    useUdpChannelTwo_ = b;
}

void TuioChannels::setFlashXmlChannel( bool b )
{
    useFlashXmlChannel_ = b;
}

void TuioChannels::setBinaryTcpChannel( bool b )
{
    useBinaryTcpChannel_ = b;
}

bool TuioChannels::useNetworkSilentMode()
{
    return silentMode_;
}

bool TuioChannels::useUdpChannelOne()
{
    return useUdpChannelOne_;
}

bool TuioChannels::useUdpChannelTwo()
{
    return useUdpChannelTwo_;
}

bool TuioChannels::useFlashXmlChannel()
{
    return useFlashXmlChannel_;
}

bool TuioChannels::useBinaryTcpChannel()
{
    return useBinaryTcpChannel_;
}

std::string TuioChannels::getLocalHost()
{
    return localHost_;
}

int TuioChannels::getUdpChannelOnePort()
{
    return tuioUdpChannelOne_.getPort();
}

int TuioChannels::getUdpChannelTwoPort()
{
    return tuioUdpChannelTwo_.getPort();
}

int TuioChannels::getFlashXmlChannelPort()
{
    return flashXmlChannel_.getPort();
}
double TuioChannels::calculateAverageTime()
{
    return processTimer_->calculateAverageTime();
}
