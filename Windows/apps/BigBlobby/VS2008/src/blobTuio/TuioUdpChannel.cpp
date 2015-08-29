////////////////////////////////////////////////////////////////////////////////
// TuioUdpChannel
//
// PURPOSE: Uses an ofxOscSender for sending out TUIO UDP messages.
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
#include "blobTuio/TuioUdpChannel.h"

using blobTuio::TuioUdpChannel;
using blobEnums::TuioProfileEnum;

const blobEnums::TuioProfileEnum::Enum TuioUdpChannel::DEFAULT_TUIO_2D_PROFILE = 
                                     blobEnums::TuioProfileEnum::TUIO_2D_CURSOR;

const double TuioUdpChannel::DEFAULT_CAMERA_WIDTH = 320,
             TuioUdpChannel::DEFAULT_CAMERA_HEIGHT = 240;

TuioUdpChannel::TuioUdpChannel() :
  oscSender_(),
  frameseq_( 0 ),
  tuioProfile_( DEFAULT_TUIO_2D_PROFILE ),
  tuioProfileName_( blobEnums::TuioProfileEnum::convertToString( tuioProfile_ ) ),
  cameraWidth_( DEFAULT_CAMERA_WIDTH ),
  cameraHeight_( DEFAULT_CAMERA_HEIGHT ),
  cameraArea_( DEFAULT_CAMERA_WIDTH * DEFAULT_CAMERA_HEIGHT )
{
}

TuioUdpChannel::~TuioUdpChannel()
{
}

void TuioUdpChannel::setup( const std::string & localHost, int port )
{
    setLocalHost( localHost );
    setPort( port );
    frameseq_ = 0;
    oscSender_.setup( localHost, port );
}

void TuioUdpChannel::setTuioProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    tuioProfile_ = profile;
    tuioProfileName_ = blobEnums::TuioProfileEnum::convertToString( profile );
}

void TuioUdpChannel::setCameraWidthAndHeight( double width, double height )
{
    cameraWidth_ = (width > 0) ? width : DEFAULT_CAMERA_WIDTH;
    cameraHeight_ = (height > 0) ? height : DEFAULT_CAMERA_HEIGHT;
    cameraArea_ = cameraWidth_ * cameraHeight_;
}

void TuioUdpChannel::sendTUIO( std::map<int, blobTracker::Blob> * blobs )
{
    ++frameseq_;

    switch( tuioProfile_ ) {
        case TuioProfileEnum::TUIO_2D_CURSOR:  
            sendMessage( blobs, "/tuio/2Dcur", TuioProfileEnum::TUIO_2D_CURSOR );
            break;
        case TuioProfileEnum::TUIO_2D_BLOB: 
            sendMessage( blobs, "/tuio/2Dblb", TuioProfileEnum::TUIO_2D_BLOB );
            break;
        case TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB: 
            sendMessage( blobs, "/tuio/2Dblb", TuioProfileEnum::TUIO_2D_BLOB );
            sendMessage( blobs, "/tuio/2Dcur", TuioProfileEnum::TUIO_2D_CURSOR );
            break;
        default:  
            break;
    }
}

void TuioUdpChannel::sendMessage( std::map<int, blobTracker::Blob> * blobs, 
                                  const std::string & profileName,
                                  TuioProfileEnum::Enum profile )
{
    ofxOscBundle oscBundle;

    ofxOscMessage alive;
	alive.setAddress( profileName );
	alive.addStringArg( "alive" );

	ofxOscMessage fseq;
	fseq.setAddress( profileName );
	fseq.addStringArg( "fseq" );
	fseq.addIntArg( frameseq_ );

    for( map<int, blobTracker::Blob>::iterator blob = blobs->begin(); blob != blobs->end(); ++blob ) {
        if( blob->second.centroid.x == 0 && blob->second.centroid.y == 0 ) {
			continue;  // Skip point outside calibration grid.
        }
        switch( profile ) {
            case TuioProfileEnum::TUIO_2D_CURSOR:
		        add2DcurProfile( oscBundle, blob->second );
		        alive.addIntArg( blob->second.id );
                break;
            case TuioProfileEnum::TUIO_2D_BLOB:
                add2DblbProfile( oscBundle, blob->second );
		        alive.addIntArg( blob->second.id );
                break;
            default:
                break;
        }
    }
    oscBundle.addMessage( alive );
    oscBundle.addMessage( fseq );
    oscSender_.sendBundle( oscBundle );
}

void TuioUdpChannel::add2DcurProfile( ofxOscBundle & oscBundle, blobTracker::Blob & blob )
{
    ofxOscMessage msg;
    msg.setAddress( "/tuio/2Dcur" );
    msg.addStringArg( "set" );
    msg.addIntArg( blob.id);            // id (blob ID)
    msg.addFloatArg( blob.centroid.x ); // x  (position)
    msg.addFloatArg( blob.centroid.y ); // y  (position)
    msg.addFloatArg( blob.D.x );        // dX (velocity vector)
    msg.addFloatArg( blob.D.y );        // dY (velocity vector)
    msg.addFloatArg( blob.maccel );     // m  (motion acceleration)

    if( includeHeightAndWidth() ) {
        msg.addFloatArg( blob.boundingRect.width );	 // wd
        msg.addFloatArg( blob.boundingRect.height ); // ht
    }
    oscBundle.addMessage( msg );
}

/***************************************************************************//**
The /tuio/2Dblb profile includes info on a blob's area.  The area is expressed 
as a fraction of the total camera area, so it will be a value between 0.0 and
1.0.  This value is obtained by dividing the blob.area by the 
camera area.
*******************************************************************************/
void TuioUdpChannel::add2DblbProfile( ofxOscBundle & oscBundle, blobTracker::Blob & blob )
{
    ofxOscMessage msg;
    double width = blob.angleBoundingRect.width / cameraWidth_,
           height = blob.angleBoundingRect.height / cameraHeight_,
           area = blob.area / cameraArea_,
           angleInRadians = blob.angle * RADIANS_PER_DEGREE;

    msg.setAddress( "/tuio/2Dblb" );
    msg.addStringArg( "set" );
    msg.addIntArg( blob.id);             // id (blob ID)
    msg.addFloatArg( blob.centroid.x );  // x  (position)
    msg.addFloatArg( blob.centroid.y );  // y  (position)
    msg.addFloatArg( angleInRadians );	 // a  (angle: TUIO spec calls for radians)
    msg.addFloatArg( width );	         // w  (width)
    msg.addFloatArg( height );           // h  (height)
    msg.addFloatArg( area );             // f  (area)
    msg.addFloatArg( blob.D.x );	     // X  (velocity vector)
    msg.addFloatArg( blob.D.y );	     // Y  (velocity vector)
    msg.addFloatArg( 0.0f );		     // TODO: A (rotation velocity vector)
    msg.addFloatArg( blob.maccel );	     // m  (motion acceleration)
    msg.addFloatArg( 0.0f );			 // TODO: r (rotation acceleration)
    oscBundle.addMessage( msg );
}
