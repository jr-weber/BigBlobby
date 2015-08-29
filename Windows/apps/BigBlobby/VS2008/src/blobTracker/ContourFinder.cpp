/*
*  ContourFinder
*  
*  Created on 2/2/09.
*  Adapted from openframeworks ofxCvContourFinder
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
#include "blobTracker/ContourFinder.h"
#include "blobNormalize/FishEyeGrid.h"
#include "blobBuffers/GrayBuffer.h"

using blobTracker::ContourFinder;

//------------------------------------------------------------------------------
static int qsort_carea_compare( const void* _a, const void* _b) 
{
	int out = 0;
	// pointers, ugh.... sorry about this
	CvSeq* a = *((CvSeq **)_a);
	CvSeq* b = *((CvSeq **)_b);
	// use opencv to calc size, then sort based on size
	float areaa = fabs(cvContourArea(a, CV_WHOLE_SEQ));
	float areab = fabs(cvContourArea(b, CV_WHOLE_SEQ));
	// note, based on the -1 / 1 flip
	// we sort biggest to smallest, not smallest to biggest
	if( areaa > areab )		out = -1;
	else					out =  1;
	return out;
}

//------------------------------------------------------------------------------
ContourFinder::ContourFinder()
{
	myMoments_ = (CvMoments*)malloc( sizeof(CvMoments) );
    debugPrint_ = false;

    setUnmatchedBlobFramesToSkip( blobTracker::Blob::DEATH_COUNT - 1 );
}

//------------------------------------------------------------------------------
ContourFinder::~ContourFinder() 
{
	free( myMoments_ );
}

void ContourFinder::setUnmatchedBlobFramesToSkip( int frames )
{
    unmatchedBlobDeathCount_ = frames + 1;
    //std::cout << "unmatchedBlobDeathCount_ = " << unmatchedBlobDeathCount_ << "\n";
}

int ContourFinder::unmatchedBlobFramesToSkip()
{
    return unmatchedBlobDeathCount_ - 1;
}

vector<blobTracker::Blob> & ContourFinder::getBlobs()
{
    return blobs_;
}

bool ContourFinder::isEmpty()
{
    return (blobs_.size() > 0);
}

//------------------------------------------------------------------------------
int ContourFinder::findContours( ofxCvGrayscaleImage & input,
								 int minArea,
								 int maxArea,
								 int nConsidered,
								 bool bFindHoles,
                                 int frameCount /*= 0*/,
                                 blobBuffers::GrayBuffer * sourceBuffer /*= NULL*/,
                                 blobBuffers::GrayBuffer * backgroundBuffer /*= NULL*/,
                                 bool bUseApproximation /*= true*/ )
{
    //debugPrint_ = ((frameCount % 60) == 0);

    // OpenCV will clobber the image used to find contours, so create a copy.
    if( inputCopy_.width != input.width || inputCopy_.height != input.height ) { 
		inputCopy_.allocate( input.width, input.height );
    }
    inputCopy_ = input;
	blobs_.clear();

	CvSeq * contour_list = NULL;
	contourStorage_ = cvCreateMemStorage( 1000 );
	storage_	= cvCreateMemStorage( 1000 );
	cvFindContours( inputCopy_.getCvImage(), 
                    contourStorage_, 
                    &contour_list,
                    sizeof(CvContour), 
                    bFindHoles ? CV_RETR_LIST : CV_RETR_EXTERNAL, 
                    bUseApproximation ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE );

	CvSeq * contour_ptr = contour_list;
	nCvSeqsFound = 0;

	// put the contours from the linked list, into an array for sorting
	while( (contour_ptr != NULL) ) {
		float area = fabs( cvContourArea( contour_ptr, CV_WHOLE_SEQ ) );

		if( (area > minArea) && (area < maxArea) ) {
            ////if (nCvSeqsFound < TOUCH_MAX_CONTOUR_LENGTH) { // Wrong constant!
            if (nCvSeqsFound < TOUCH_MAX_CONTOURS) { // // Added by JRW on 6/15/2012
				cvSeqBlobs[nCvSeqsFound] = contour_ptr;	 // copy the pointer
                nCvSeqsFound++;
		    }
            else {
                break; // Added by JRW on 6/15/2012
            }
		}
		contour_ptr = contour_ptr->h_next;
	}
	// sort the pointers based on size
	if( nCvSeqsFound > 0 ) {
		qsort( cvSeqBlobs, nCvSeqsFound, sizeof(CvSeq*), qsort_carea_compare);
	}
	// now, we have nCvSeqsFound contours, sorted by size in the array
    // cvSeqBlobs let's get the data out and into our structures that we like
	for( int i = 0; i < MIN( nConsidered, nCvSeqsFound ); ++i ) {
        createBlob( cvSeqBlobs[i], sourceBuffer, backgroundBuffer );
	}
	// Free the storage memory.
	// Warning: do this inside this function otherwise a strange memory leak
	if( contourStorage_ != NULL ) { cvReleaseMemStorage(&contourStorage_); }
	if( storage_ != NULL ) { cvReleaseMemStorage(&storage_); }

	return blobs_.size();
}

void ContourFinder::createBlob( CvSeq * cvSeqBlob, 
                                blobBuffers::GrayBuffer * sourceBuffer, 
                                blobBuffers::GrayBuffer * backgroundBuffer )
{
    blobTracker::Blob blob;
    float area = cvContourArea( cvSeqBlob, CV_WHOLE_SEQ );
    cvMoments( cvSeqBlob, myMoments_ );
    addBoundingRectangle( blob, cvSeqBlob );
    addAngularBoundingRectangle( blob, cvSeqBlob );
    addCentroids( blob );
    addSrcAndBackgroundIntensities( blob, sourceBuffer, backgroundBuffer );

    // assign other parameters
    blob.deathCount = unmatchedBlobDeathCount_;
    float absoluteArea = fabs(area);
    blob.area           = absoluteArea;
    blob.areaSum        = 0;
    blob.areaFrameCount = 0;
    blob.areaTime       = ofGetElapsedTimef();
    blob.averageArea    = absoluteArea;
    blob.hole           = area < 0 ? true : false;
    blob.length 	    = cvArcLength( cvSeqBlob );
    
    // get the points for the blob:
    CvPoint pt;
    CvSeqReader reader;
    cvStartReadSeq( cvSeqBlob, &reader, 0 );
    int length = min( TOUCH_MAX_CONTOUR_LENGTH, cvSeqBlob->total );

    for( int j = 0; j < length; ++j ) {
        CV_READ_SEQ_ELEM( pt, reader );
        blob.pts.push_back( ofPoint( (float)pt.x, (float)pt.y ) );
    }
    blob.nPts = blob.pts.size();
    blobs_.push_back( blob );

    //if( debugPrint_ ) { debugPrint( blob ); }
}

void ContourFinder::addBoundingRectangle( blobTracker::Blob & blob, CvSeq * cvSeqBlob )
{
    CvRect rect	= cvBoundingRect( cvSeqBlob, 0 );
    blob.boundingRect.x = rect.x;
    blob.boundingRect.y = rect.y;
    blob.boundingRect.width  = rect.width;
    blob.boundingRect.height = rect.height;
}

/***************************************************************************//**
The original CCV 1.3 code had width and height criss-crossed (not sure why).
That has been corrected here (11/6/2014). -JRW

The OpenCV cvMinAreaRect2() function uses a scheme where the angle will always
be from 0.0 to -89.9 degrees, and when 90 degrees would be expected, the width 
and height flip and the angle goes to 0.0.  That 90 degree range with 
width-height flipping scheme works because flipping the width and height is the
same thing as a 90 degree rotation.  However, that scheme will generate some 
odd problems when a camera image is mapped to a monitor/projector with a
different aspect ratio.  The non-uniform scaling can cause a noticeable 
change in the dimensions of the angled bounding box when the width and height 
are flipped because the x-scaling does not match the y-scaling.  To avoid an 
odd jitter effect when going from -89.9 degrees to 0 degrees, this function will 
always set the major axis (greater dimension) to be the width and will just 
add 90 degrees instead of flipping the width and height.  This feature has 
been tested with a finger paint app that draws bounding boxes, and the -89.9 to 
+90.0 degree scheme (with no width-height flipping) produces a nicer, smoother 
rotation for oblong blobs.  This feature is best tested using a blob that is 
at least 3 times longer than its width.
*******************************************************************************/
void ContourFinder::addAngularBoundingRectangle( blobTracker::Blob & blob, CvSeq * cvSeqBlob )
{
    CvBox2D32f box = cvMinAreaRect2( cvSeqBlob );
    blob.angleBoundingRect.x = box.center.x;
    blob.angleBoundingRect.y = box.center.y;
    float width = box.size.width,
          height = box.size.height,
          angle = box.angle;

    if( height > width  ) {
        float temp = width;
        width = height;
        height = temp;
        angle += 90.0;
    }
    blob.angleBoundingRect.width  = width;
    blob.angleBoundingRect.height = height;
    blob.angle = angle;

}

void ContourFinder::addCentroids( blobTracker::Blob & blob )
{
    blob.centroid.x = (myMoments_->m10 / myMoments_->m00);
    blob.centroid.y = (myMoments_->m01 / myMoments_->m00);
    blob.lastCentroid.x = 0;
    blob.lastCentroid.y = 0;
    blob.cameraCentroid.x = blob.centroid.x;
    blob.cameraCentroid.y = blob.centroid.y;
}

void ContourFinder::addSrcAndBackgroundIntensities( blobTracker::Blob & blob, 
                                                    blobBuffers::GrayBuffer * sourceBuffer, 
                                                    blobBuffers::GrayBuffer * backgroundBuffer )
{
    if( sourceBuffer != NULL && backgroundBuffer != NULL ) {
        int col = (int)(blob.cameraCentroid.x + 0.5),
            row = (int)(blob.cameraCentroid.y + 0.5);
        blob.sourceIntensity = (float)sourceBuffer->pixel( row, col );
        blob.backgroundIntensity = (float)backgroundBuffer->pixel( row, col );
    }
}

void ContourFinder::debugPrint( blobTracker::Blob & blob )
{
    ofRectangle rect = blob.angleBoundingRect;
    int x = blob.centroid.x,
        y = blob.centroid.y;

    std::cout 
        << std::fixed << setprecision( 1 )
        << "\nContourFinder::debugPrint(blob) called.\n"
        << "    centroid (x, y) = (" << x << ", " << y << ")\n"
        << "    angleBox (x, y) = (" << rect.x << ", " << rect.y << ")\n"
        << "    box: width x height = " << rect.width << " x " << rect.height << "\n"
        << std::fixed << setprecision( 3 )
        << "    angle  = " << blob.angle << "\n"
        << "    source     = " << blob.sourceIntensity << "\n"
        << "    background = " << blob.backgroundIntensity << "\n"
        << "\n";
}
