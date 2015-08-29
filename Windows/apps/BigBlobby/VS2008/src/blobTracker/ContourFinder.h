/*
* ContourFinder
*
* Finds white blobs in binary images and identifies
* centroid, bounding box, area, length and polygonal contour
* The result is placed in a vector of Blob objects.
*
* Created on 2/2/09.
* Adapted from openframeworks ofxCvContourFinder
*
* The original code has been broken up by using more helper functions, but
* is otherwise mostly the original algorithm.
*     -J.R.Weber <joe.weber77@gmail.com>
*/
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
#ifndef CONTOUR_FINDER_H
#define CONTOUR_FINDER_H

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "blobTracker/Blob.h"

#define TOUCH_MAX_CONTOURS			128
#define TOUCH_MAX_CONTOUR_LENGTH	1024

namespace blobNormalize { class FishEyeGrid; }
namespace blobBuffers { class GrayBuffer; }

namespace blobTracker
{
    class ContourFinder 
    {
    public:
        ContourFinder();
        ~ContourFinder();
	    int findContours( ofxCvGrayscaleImage& input,
                          int minArea, 
                          int maxArea,
                          int nConsidered, 
                          bool bFindHoles,
                          int frameCount = 0,
                          blobBuffers::GrayBuffer * sourceBuffer = NULL,
                          blobBuffers::GrayBuffer * backgroundBuffer = NULL,
                          bool bUseApproximation = true );
                          // approximation = don't do points for all 
                          // points of the contour, if the contour runs
                          // along a straight line, for example...
        blobTracker::Blob getBlob(int num);
        vector<blobTracker::Blob> & getBlobs();
        bool isEmpty();
        void setUnmatchedBlobFramesToSkip( int frames );
        int unmatchedBlobFramesToSkip();

    protected:
        ofxCvGrayscaleImage inputCopy_;
        CvMemStorage * contourStorage_;
        CvMemStorage * storage_;
        CvMoments * myMoments_;

        // internally, we find cvSeqs, they will become blobs.
        CvSeq * cvSeqBlobs[TOUCH_MAX_CONTOURS];
        int nCvSeqsFound;

    private:
        void createBlob( CvSeq * cvSeqBlob, 
                         blobBuffers::GrayBuffer * sourceBuffer, 
                         blobBuffers::GrayBuffer * backgroundBuffer );
        void addBoundingRectangle( blobTracker::Blob & blob, CvSeq * cvSeqBlob );
        void addAngularBoundingRectangle( blobTracker::Blob & blob, CvSeq * cvSeqBlob );
        void addCentroids( blobTracker::Blob & blob );
        void addSrcAndBackgroundIntensities( blobTracker::Blob & blob, 
                                             blobBuffers::GrayBuffer * sourceBuffer, 
                                             blobBuffers::GrayBuffer * backgroundBuffer );
        void debugPrint( blobTracker::Blob & blob );

        vector <blobTracker::Blob> blobs_;
        int unmatchedBlobDeathCount_;
        bool debugPrint_;
    };
}

#endif
