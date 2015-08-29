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
#ifndef BLOB_H
#define BLOB_H

#include "ofTypes.h"
#include "ofAppRunner.h"
#include <vector>

namespace blobTracker
{
    class Blob 
    {
        public: 
            // The death count is used in the BlobTracker to determine how many 
            // frames a tracked blob can go missing before being deleted.
            static const int DEATH_COUNT;

            std::vector <ofPoint> pts;     // contour of the blob
            int                   nPts;    // number of pts
		    int					  id;
            int                   deathCount;
	        float                 area;
            float                 areaSum;
            int                   areaFrameCount;
            float                 areaTime;
            int                   averageArea;
            float                 length;
		    float                 angle;
		    float				  maccel;  // distance traveled since last frame
		    float				  age;     // how long the blob has been at war
		    float				  sitting; // how long hes been sitting in the same place
		    float				  downTime;
		    float				  lastTimeTimeWasChecked;
	        ofRectangle           boundingRect;
		    ofRectangle           angleBoundingRect;
		    ofPoint               centroid, 
                                  lastCentroid, 
                                  D,
                                  cameraCentroid;
            float                 sourceIntensity;     // Source image intensity for centroid.
            float                 backgroundIntensity; // Background image intensity for centroid.
		    bool				  simulated;
		    bool                  hole;				

            Blob();
            ~Blob();

            void drawCrosshairs();
            void drawAngularBoundingBox();
            void drawContourPoints();

            void drawContours( float x = 0, 
                               float y = 0, 
                               float inputWidth = ofGetWidth(), 
                               float inputHeight = ofGetHeight(), 
                               float outputWidth = ofGetWidth(), 
                               float outputHeight = ofGetHeight() );

        private:
    };
}

#endif
