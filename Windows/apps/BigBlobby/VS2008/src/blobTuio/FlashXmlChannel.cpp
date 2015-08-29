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
#include "blobTuio/FlashXmlChannel.h"

using blobTuio::FlashXmlChannel;
using blobEnums::TuioProfileEnum;

const blobEnums::TuioProfileEnum::Enum FlashXmlChannel::DEFAULT_TUIO_2D_PROFILE = 
                                     blobEnums::TuioProfileEnum::TUIO_2D_CURSOR;

const double FlashXmlChannel::DEFAULT_CAMERA_WIDTH = 320,
             FlashXmlChannel::DEFAULT_CAMERA_HEIGHT = 240;

FlashXmlChannel::FlashXmlChannel() :
  tcpServer_(),
  frameseq_( 0 ),
  tuioProfile_( DEFAULT_TUIO_2D_PROFILE ),
  tuioProfileName_( blobEnums::TuioProfileEnum::convertToString( tuioProfile_ ) ),
  cameraWidth_( DEFAULT_CAMERA_WIDTH ),
  cameraHeight_( DEFAULT_CAMERA_HEIGHT ),
  cameraArea_( DEFAULT_CAMERA_WIDTH * DEFAULT_CAMERA_HEIGHT )
{
}

FlashXmlChannel::~FlashXmlChannel()
{
}

void FlashXmlChannel::setup( const std::string & localHost, int port )
{
    setLocalHost( localHost );
    setPort( port );
    frameseq_ = 0;
    tcpServer_.setup( port );
}

void FlashXmlChannel::setTuioProfile( blobEnums::TuioProfileEnum::Enum profile )
{
    tuioProfile_ = profile;
    tuioProfileName_ = blobEnums::TuioProfileEnum::convertToString( profile );
}

void FlashXmlChannel::setCameraWidthAndHeight( double width, double height )
{
    cameraWidth_ = (width > 0) ? width : DEFAULT_CAMERA_WIDTH;
    cameraHeight_ = (height > 0) ? height : DEFAULT_CAMERA_HEIGHT;
    cameraArea_ = cameraWidth_ * cameraHeight_;
}

void FlashXmlChannel::sendTUIO( std::map<int, blobTracker::Blob> * blobs )
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
    }
}

void FlashXmlChannel::sendMessage( std::map<int, blobTracker::Blob> * blobs, 
                                   const std::string & profileName,
                                   TuioProfileEnum::Enum profile )
{
    if( blobs->size() == 0 ) {
        sendAliveMessage( profileName );
    }
    else {
        std::string setBlobsMsg;
        std::string aliveBeginMsg = "<MESSAGE NAME=\"" + profileName + "\"><ARGUMENT TYPE=\"s\" VALUE=\"alive\"/>";
        std::string aliveEndMsg = "</MESSAGE>";
        std::string aliveBlobsMsg;

        for( map<int, blobTracker::Blob>::iterator blob = blobs->begin(); blob != blobs->end(); ++blob ) {
            // Omit point (0,0) because this means that we are outside of the calibration grid.
            if( (blob->second.centroid.x == 0) && (blob->second.centroid.y == 0) ) {
                continue;
            }
            switch( profile ) {
                case TuioProfileEnum::TUIO_2D_CURSOR:
		            add2DcurProfile( setBlobsMsg, blob->second );
                    aliveBlobsMsg += "<ARGUMENT TYPE=\"i\" VALUE=\"" + ofToString( blob->second.id ) + "\"/>";
                    break;
                case TuioProfileEnum::TUIO_2D_BLOB:
                    add2DblbProfile( setBlobsMsg, blob->second );
                    aliveBlobsMsg += "<ARGUMENT TYPE=\"i\" VALUE=\"" + ofToString( blob->second.id ) + "\"/>";
                    break;
            }
        }
        string fseq = "<MESSAGE NAME=\"" + profileName + "\">"
          "<ARGUMENT TYPE=\"s\" VALUE=\"fseq\"/>"
          "<ARGUMENT TYPE=\"i\" VALUE=\"" + ofToString( frameseq_ ) + "\"/></MESSAGE>";
        tcpServer_.sendToAll( 
            "<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\"" + ofToString( getPort() ) 
          + "\" TIME=\""  + ofToString( ofGetElapsedTimef() ) + "\">" 
          + setBlobsMsg + aliveBeginMsg + aliveBlobsMsg + aliveEndMsg + fseq 
          + "</OSCPACKET>" );

        //bool debugFrame = (frameseq_ % 240) == 0;

        //if( debugFrame ) {
        //    std::cout << "\n\nSending " << profileName << " for frameseq_ = " << frameseq_ << "\n\n"
        //        << "<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\"" + ofToString( getPort() ) 
        //         + "\" TIME=\""  + ofToString( ofGetElapsedTimef() ) + "\">" 
        //         + setBlobsMsg + aliveBeginMsg + aliveBlobsMsg + aliveEndMsg + fseq 
        //         + "</OSCPACKET>" 
        //        << "\n";
        //}
    }
}

void FlashXmlChannel::sendAliveMessage( const std::string & profileName )
{
    tcpServer_.sendToAll( 
        "<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\"" + ofToString( getPort() ) 
      + "\" TIME=\"" + ofToString(ofGetElapsedTimef()) + "\">"
      + "<MESSAGE NAME=\"" + profileName + "\">"
      + "<ARGUMENT TYPE=\"s\" VALUE=\"alive\"/>"
      + "</MESSAGE>"
      + "<MESSAGE NAME=\"" + profileName + "\">"
      + "<ARGUMENT TYPE=\"s\" VALUE=\"fseq\"/>"
      + "<ARGUMENT TYPE=\"i\" VALUE=\"" + ofToString( frameseq_ ) + "\"/>" 
      + "</MESSAGE>"
      + "</OSCPACKET>" );
}

void FlashXmlChannel::add2DcurProfile( std::string & blobMessage, blobTracker::Blob & blob )
{
    // if sending height and width
    if( includeHeightAndWidth() ) {
        blobMessage += 
            "<MESSAGE NAME=\"/tuio/2Dcur\">"
            "<ARGUMENT TYPE=\"s\" VALUE=\"set\"/>"
            "<ARGUMENT TYPE=\"i\" VALUE=\"" + ofToString( blob.id ) + "\"/>"         // id (blob ID)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.centroid.x ) + "\"/>" // x  (position)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.centroid.y ) + "\"/>" // y  (position)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.D.x ) + "\"/>"        // dX (velocity vector)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.D.y ) + "\"/>"        // dY (velocity vector)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.maccel ) + "\"/>"     // m  (motion acceleration)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.boundingRect.width ) + "\"/>"  // Not part of TUIO specification
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.boundingRect.height ) + "\"/>" // Not part of TUIO specification
          + "</MESSAGE>";
    }
    else {
        blobMessage += 
            "<MESSAGE NAME=\"/tuio/2Dcur\">"
            "<ARGUMENT TYPE=\"s\" VALUE=\"set\"/>"
            "<ARGUMENT TYPE=\"i\" VALUE=\"" + ofToString( blob.id ) + "\"/>"         // id (blob ID)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.centroid.x ) + "\"/>" // x  (position)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.centroid.y ) + "\"/>" // y  (position)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.D.x ) + "\"/>"        // dX (velocity vector)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.D.y ) + "\"/>"        // dY (velocity vector)
          + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.maccel ) + "\"/>"     // m  (motion acceleration)
          + "</MESSAGE>";
    }
}

void FlashXmlChannel::add2DblbProfile( std::string & blobMessage, blobTracker::Blob & blob )
{
    double width = blob.angleBoundingRect.width / cameraWidth_,
           height = blob.angleBoundingRect.height / cameraHeight_,
           area = blob.area / cameraArea_,
           angleInRadians = blob.angle * RADIANS_PER_DEGREE;

     blobMessage += 
         "<MESSAGE NAME=\"/tuio/2Dblb\">"
         "<ARGUMENT TYPE=\"s\" VALUE=\"set\"/>"
         "<ARGUMENT TYPE=\"i\" VALUE=\"" + ofToString( blob.id ) + "\"/>"         // id (blob ID)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.centroid.x ) + "\"/>" // x  (position)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.centroid.y ) + "\"/>" // y  (position)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( angleInRadians ) + "\"/>"  // a  (angle: TUIO spec calls for radians)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( width ) + "\"/>"           // w  (width)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( height ) + "\"/>"          // h  (height)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( area ) + "\"/>"            // f  (area)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.D.x ) + "\"/>"	      // X  (velocity vector)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.D.y ) + "\"/>"	      // Y  (velocity vector)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( 0.0 ) + "\"/>"             // TODO: A (rotation velocity vector)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( blob.maccel ) + "\"/>"	  // m  (motion acceleration)
       + "<ARGUMENT TYPE=\"f\" VALUE=\"" + ofToString( 0.0 ) + "\"/>"	          // TODO: r (rotation acceleration)
       + "</MESSAGE>";
}
