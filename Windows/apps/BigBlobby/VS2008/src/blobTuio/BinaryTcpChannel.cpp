////////////////////////////////////////////////////////////////////////////////
// BinaryTcpChannel
//
// PURPOSE: Uses an ofxTCPServer for sending out Binary TCP messages.
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
#include "blobTuio/BinaryTcpChannel.h"

using blobTuio::BinaryTcpChannel;

BinaryTcpChannel::BinaryTcpChannel() :
  tcpServer_(),
  frameseq_( 0 )
{
}

BinaryTcpChannel::~BinaryTcpChannel()
{
}

void BinaryTcpChannel::setup( const std::string & localHost, int port )
{
    setLocalHost( localHost );
    setPort( port );
    frameseq_ = 0;
    tcpServer_.setup( port );
}

void BinaryTcpChannel::sendTUIO( std::map<int, blobTracker::Blob> * blobs )
{
    ++frameseq_;
    unsigned char buf[1024 * 8];
    unsigned char *p = buf;

    // Add "CCV" as a data header
	*p++ = 'C';	
    *p++ = 'C';	
    *p++ = 'V';	
    *p++ = '\0';
		
    if( blobs->size() == 0 ) {
		memset( p, 0, 4 );
        p += 4;
	} 
    else {
        int count = 0;
        map<int, blobTracker::Blob>::iterator blob;

        // count the blobs that are non (0,0)
        for( blob = blobs->begin(); blob != blobs->end(); blob++ ) {
            // omit point (0,0) since this means that we are outside of the range
            if( blob->second.centroid.x == 0 && blob->second.centroid.y == 0 ) {
                continue;
            }
            count++;
        }
        // send blob count first
        memcpy( p, &count, 4 );	
        p += 4;

        // send blob information
        for( blob = blobs->begin(); blob != blobs->end(); blob++ ) {
            // omit point (0,0) since this means that we are outside of the range
            if( blob->second.centroid.x == 0 && blob->second.centroid.y == 0 ) {
                continue;
            }
            memcpy( p, &blob->second.id, 4 );
            p += 4;
            memcpy( p, &blob->second.centroid.x, 4 );					
            p += 4;
            memcpy( p, &blob->second.centroid.y, 4 );					
            p += 4;
            memcpy( p, &blob->second.D.x, 4 );						
            p += 4;
            memcpy( p, &blob->second.D.y, 4 );						
            p += 4;
            memcpy( p, &blob->second.maccel, 4 );						
            p += 4;

            if( includeHeightAndWidth() ) {
                memcpy( p, &blob->second.boundingRect.width, 4 );		
                p += 4;
                memcpy( p, &blob->second.boundingRect.height, 4 );	
                p += 4;
            }
        }
    }
    // send blob data to clients
    tcpServer_.sendRawBytesToAll( (const char*)buf, p-buf );
}
