/*  BlobTracker
 *
 *  Created by Ramsin Khoshabeh on 5/4/08.
 *  Copyright 2008 risenparadigm. All rights reserved.
 *
 *  Changelog:
 *  08/15/08  -- Fixed a major bug in the track algorithm
 *
 *  4/27/2015 -- Refactored the track(newBlobs) function, which was originally
 *               about 400 lines of code.  The 18 helper functions that were 
 *               added range in size from a few lines of code to about 30 lines
 *               (at most), which I find easier to read and understand. 
 *                   -J.R. Weber <joe.weber77@gmail.com>
 *
 *  4/29/2015 -- Replaced trackKnn() function with simpler grabNearestNewBlob()
 *               function that does the same thing.
 *                   -J.R. Weber <joe.weber77@gmail.com>
 *
 *  5/4/2015 -- Replaced the data structure used to hold the tracked blobs.
 *              The new data structure, a vector of TrackedBlobHolder objects,
 *              allows each tracked Blob to hold onto a memory of several of
 *              the nearest new Blobs (index number and distance).  This info 
 *              will be used to allow a tracked Blob multiple chances at  
 *              survival if its first choice of new Blob has already been
 *              claimed by another tracked Blob that is closer to the first
 *              choice new Blob.  Deleted the grabNearestNewBlob() function.
 *                  -J.R. Weber <joe.weber77@gmail.com>
 *
 *  5/13/2015 -- If a tracked blob has no match with a new blob, it is no 
 *               longer deleted immediately.  Instead, it gets a few more 
 *               frames grace period before being deleted.  The deathCount 
 *               member variable on class Blob is used to determine how many
 *               frames a tracked blob can go without a match.
 *                   -J.R. Weber <joe.weber77@gmail.com>
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
#ifndef BLOBTRACKER_BLOBTRACKER_H
#define BLOBTRACKER_BLOBTRACKER_H

#include "blobTracker/ContourFinder.h"
#include "blobCalibration/CalibrationUtils.h"
#include <list>
#include <map>
#include <vector>

namespace blobTracker { class TrackedBlobHolder; }
namespace blobTouchEvents { class TouchEvents; }

typedef std::vector<blobTracker::TrackedBlobHolder *>::iterator TrackedBlobIterator;

namespace blobTracker 
{
    class BlobTracker
    {
    public:
        static const double BLOB_DISTANCE_FACTOR;
        static const float AREA_REPORTING_INTERVAL_SECONDS;
        static const int DEBUG_FRAME_INTERVAL,
                         MAX_DIFF_INDEX;

	    BlobTracker();
	    ~BlobTracker();

	    void passInCalibration( CalibrationUtils * calibrationUtils );
        void setCalibrating( bool b );
        void setMaxBlobDistance( int cameraWidth );
	    std::map<int, blobTracker::Blob> * getTrackedBlobs();
        bool hasBlobsInScreenArea();

	    void track( vector<blobTracker::Blob> & newBlobs );

    private:
        void checkFrameCount();

        void buildListOfPotentialMatches( vector<blobTracker::Blob> & newBlobs );
        void sortTrackedBlobsByDistance();
        void processMatches( vector<blobTracker::Blob> & newBlobs );
        void updateKeyParamsOfNewBlob( blobTracker::Blob & newBlob, blobTracker::Blob & trackedBlob );
        void updateTrackedBlob( blobTracker::Blob & trackedBlob, blobTracker::Blob & newBlob );
        void calculateAcceleration( blobTracker::Blob & trackedBlob );
        void calculateAgeAndTimeSitting( blobTracker::Blob & trackedBlob );
        void addContinuingTrackToOutputMap( const blobTracker::Blob & trackedBlob );
        void bubbleUpToMaintainSortedState( int index );
        void deleteTrackedBlob( int index );

        void addNewTracks( vector<blobTracker::Blob> & newBlobs );
        void addToTrackedBlobs( blobTracker::Blob & blob );
        void addNewTrackToOutputMap( const blobTracker::Blob & trackedBlob );

        void sendRAWTouchUpEventIfCalibrating( blobTracker::Blob & blob );
        void sendRAWTouchHeldEventIfCalibrating( blobTracker::Blob & blob );
        void sendRAWTouchMovedEventIfCalibrating( blobTracker::Blob & blob );
        void sendRAWTouchDownEventIfCalibrating( blobTracker::Blob & blob );

        void debugPrintLoserAndWinnerIDs( blobTracker::Blob & loser, blobTracker::Blob & winner );
        void debugPrintAnotherMatchRequestMade();
        void debugPrintListOfPotentialMatches();
        void debugPrintDeathCount( blobTracker::Blob & trackedBlob );
        void debugPrintTrackedBlobDeletion( blobTracker::Blob & trackedBlob );
        void debugPrintNewTrackedBlob( blobTracker::Blob & blob );
        void debugPrintNewTrackedBlobDiffCounts( blobTracker::Blob & blob );

        blobTouchEvents::TouchEvents & touchEvents_;
        CalibrationUtils * calibrationUtils_;
        std::vector<blobTracker::TrackedBlobHolder *> trackedBlobHolders_;
	    std::map<int, blobTracker::Blob> calibratedTrackedBlobs_;
	    int uniqueIdCounter_,
            frameCount_,
	        movementFiltering_,
            elapsedTimeMillis_,
            numberOfTracksLastFrame_,
            numberOfNewBlobs_,
            frameCountForLastDelete_;
        std::vector<int> diffCounts_;
        double maxBlobDistanceSquared_;
	    bool isCalibrating_,
             hasBlobsInScreenArea_,
             debugPrintCurrentFrame_,
             debugPrint_;

    };
}

#endif
