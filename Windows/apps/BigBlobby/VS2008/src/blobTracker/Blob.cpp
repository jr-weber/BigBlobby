/* Blob
 *
 * A blob is a homogenous patch represented by a polygonal contour.
 * Typically a blob tracker uses the contour to figure out the blob's
 * persistence and "upgrades" it with ids and other temporal information.
 */
/*
Copyright (c) 2009 NUI Group/Inc. All rights reserved.
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
#include "blobTracker/Blob.h"
#include "ofGraphics.h"

using blobTracker::Blob;

const int Blob::DEATH_COUNT = 11; // Skips 10 frames before killing unmatched blob.

Blob::Blob() :
  pts(),
  nPts( 0 ),
  id( -1 ),
  deathCount( DEATH_COUNT ),
  area( 0.0f ),
  areaSum( 0.0f ),
  areaFrameCount( 0 ),
  areaTime( ofGetElapsedTimef() ),
  averageArea( 0 ),
  length( 0.0f ),
  angle( 0.0f ),
  maccel( 0.0f ),
  age( 0.0f ),
  sitting( 0.0f ),
  downTime( 0.0f ),
  lastTimeTimeWasChecked( ofGetElapsedTimeMillis() ),
  boundingRect(),
  angleBoundingRect(),
  centroid(), 
  lastCentroid(), 
  D(),
  cameraCentroid(),
  sourceIntensity( 0.0f ),
  backgroundIntensity( 0.0f ),
  simulated( false ),
  hole( false )		
{
}

Blob::~Blob()
{
}

void Blob::drawCrosshairs()
{
    glPushMatrix();
    float x = angleBoundingRect.x,
         y = angleBoundingRect.y,
         width = angleBoundingRect.width,
         height = angleBoundingRect.height;              

    glLineWidth( 1.0 );
    ofFill();
    ofSetColor( 0x0099FF );

    glTranslatef( x, y, 0.0f );
    glRotatef( angle, 0.0f, 0.0f, 1.0f );
    glTranslatef( -x, -y, 0.0f );  

    ofRect( x, y - height, 1.0f, height * 2.0f );
    ofRect( x - width, y, width * 2.0f, 1.0f );
    glPopMatrix();
}

void Blob::drawAngularBoundingBox()
{
    glPushMatrix();
    float x = angleBoundingRect.x,
          y = angleBoundingRect.y,
          width = angleBoundingRect.width,
          height = angleBoundingRect.height,
          halfWidth = width / 2.0f,
          halfHeight = height / 2.0f;              

    ofNoFill();
    glLineWidth( 2.0 );
    ofSetColor( 0xFFFF00 );

    glTranslatef( x, y, 0.0f );
    glRotatef( angle, 0.0f, 0.0f, 1.0f );
    glTranslatef( -x, -y, 0.0f );  

    ofRect( x - halfWidth, y - halfHeight, width, height );
    glPopMatrix();
}

void Blob::drawContourPoints()
{
    ofNoFill();
    glLineWidth( 2.0 );
    ofSetColor( 0xFF0099 );
    ofBeginShape();

    for (int i = 0; i < nPts; i++){
        ofVertex( pts[i].x, pts[i].y );
    }
    ofEndShape( true );	
    glLineWidth( 1.0 );
}

void Blob::drawContours( float x, 
                         float y, 
                         float inputWidth, 
                         float inputHeight, 
                         float outputWidth, 
                         float outputHeight )
{
    glPushMatrix();
    glTranslatef( x + angleBoundingRect.x/inputWidth * outputWidth, 
                  y + angleBoundingRect.y/inputHeight * outputHeight, 
                  0.0f );
    //glRotatef( -angle, 0.0f, 0.0f, 1.0f );
    glRotatef( angle, 0.0f, 0.0f, 1.0f );
    glTranslatef( -(x + angleBoundingRect.x/inputWidth * outputWidth), 
                  -(y + angleBoundingRect.y/inputHeight * outputHeight),
                  0.0f );                

    //draw cross-hairs
    glLineWidth( 1.0 );
    ofFill();
    ofSetColor(0x0099FF);

    // Vertical Plus
    ofRect( x + (angleBoundingRect.x/inputWidth) * outputWidth, 
            y + ((angleBoundingRect.y - angleBoundingRect.height)/inputHeight) * outputHeight, 
            1, 
            (angleBoundingRect.height * 2)/inputHeight * outputHeight );

    // Horizontal Plus
    ofRect( x + ((angleBoundingRect.x - angleBoundingRect.width)/inputWidth) * outputWidth,
            y + (angleBoundingRect.y/inputHeight) * outputHeight, 
            (angleBoundingRect.width * 2)/inputWidth * outputWidth, 
            1 );

    glPopMatrix();

    //draw contours
    ofNoFill();
    glLineWidth( 2.0 );
    ofSetColor(0xFF0099);
    ofBeginShape();
    for (int i = 0; i < nPts; i++){
        ofVertex(x + pts[i].x/inputWidth * outputWidth, y + pts[i].y/(inputHeight) * outputHeight);
    }
    ofEndShape(true);	
    glLineWidth( 1.0 );
}
