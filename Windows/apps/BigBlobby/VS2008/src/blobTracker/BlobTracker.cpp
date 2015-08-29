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
#include "blobTracker/BlobTracker.h"
#include "blobTracker/TrackedBlobHolder.h"
#include "blobTouchEvents/TouchEvents.h"
#include <iomanip>
#include <cmath>

using blobTracker::BlobTracker;

// The max distance allowed for a tracked blob to new blob match  
// will be determined as a fraction of the camera width.
const double BlobTracker::BLOB_DISTANCE_FACTOR = 0.2;
const float BlobTracker::AREA_REPORTING_INTERVAL_SECONDS = 0.5f;
const int BlobTracker::DEBUG_FRAME_INTERVAL = 240;
const int BlobTracker::MAX_DIFF_INDEX = 11;

static bool is_first_trackedBlob_lesser( const blobTracker::TrackedBlobHolder * first, 
                                         const blobTracker::TrackedBlobHolder * second )
{
    return (first->firstChoiceDistance() < second->firstChoiceDistance());
}

BlobTracker::BlobTracker() :
  touchEvents_( blobTouchEvents::TouchEvents::instance() ),
  calibrationUtils_( NULL ),
  trackedBlobHolders_(),
  calibratedTrackedBlobs_(),
  uniqueIdCounter_( 0 ),
  frameCount_( 0 ),
  movementFiltering_( 0 ),
  elapsedTimeMillis_( 0 ),
  numberOfTracksLastFrame_( 0 ),
  numberOfNewBlobs_( 0 ),
  frameCountForLastDelete_( 0 ),
  diffCounts_( MAX_DIFF_INDEX + 1, 0 ),
  maxBlobDistanceSquared_( 0.0 ),
  isCalibrating_( false ),
  hasBlobsInScreenArea_( false ),
  debugPrintCurrentFrame_( false ),
  debugPrint_( false )
{
    setMaxBlobDistance( 320 );
}

BlobTracker::~BlobTracker()
{
}

void BlobTracker::passInCalibration( CalibrationUtils * calibrationUtils ) 
{
    calibrationUtils_ = calibrationUtils;
}

void BlobTracker::setCalibrating( bool b )
{
    isCalibrating_ = b;
}

/***************************************************************************//**
The max distance between a tracked blob and a new blob will be calculated as a 
fraction of the camera image width.  The actual value stored will be the square 
of the distance because the TrackedBlobHolder uses squares of distances for 
comparison purposes (to avoid having to calculate square roots when using the 
Pythagorean theorem).
*******************************************************************************/
void BlobTracker::setMaxBlobDistance( int cameraWidth )
{
    double blobDistance = cameraWidth * BLOB_DISTANCE_FACTOR;
    maxBlobDistanceSquared_ = blobDistance * blobDistance;
}

std::map<int, blobTracker::Blob> * BlobTracker::getTrackedBlobs()
{
    return &calibratedTrackedBlobs_;
}

/***************************************************************************//**
Returns true if there are no blobs or if there are only blobs with (0, 0) as
their centroid position.  The CalibrationUtil::cameraToScreenPosition() function
assigns a (0, 0) centroid position to blobs that were detected, but are outside
of the calibration grid.  The TUIO server will not send out blobs that have 
a (0, 0) position.
*******************************************************************************/
bool BlobTracker::hasBlobsInScreenArea()
{
    return hasBlobsInScreenArea_;
}

/***************************************************************************//**
Assigns IDs to each new blob found by the ContourFinder.  New blobs will be 
matched to tracked blobs from the last frame if possible.  Tracked blobs that
cannot be matched will be deleted.

NOTE: Each new blob from the ContourFinder should have an id of -1, which 
indicates that the new blob has not been assigned to a track yet.
*******************************************************************************/
void BlobTracker::track( vector<blobTracker::Blob> & newBlobs )
{
    if( debugPrint_ ) { ++frameCount_; }
    numberOfTracksLastFrame_ = trackedBlobHolders_.size();
    numberOfNewBlobs_ = newBlobs.size();
    hasBlobsInScreenArea_ = false;

    ////checkFrameCount();
    buildListOfPotentialMatches( newBlobs );
    sortTrackedBlobsByDistance();
    processMatches( newBlobs );
    addNewTracks( newBlobs );
}

/***************************************************************************//**
If debugPrint_ is set to true, then this function will check for frame 
numbers evenly divisible by the DEBUG_FRAME_INTERVAL constant and set 
debugPrintCurrentFrame_ to true.  This allows for printing frame info every 
few seconds, rather than on a continuous basis that would bog down the program.
*******************************************************************************/
void BlobTracker::checkFrameCount()
{
    if( debugPrint_ ) {
        debugPrintCurrentFrame_ = ((frameCount_ % DEBUG_FRAME_INTERVAL) == 0);
    }
}

/***************************************************************************//**
Each tracked blob from the last frame will identify several nearby new blobs
as potential matches.  A TrackedBlobHolder object is used to store info on a 
tracked blob and the index and distance of its nearest MAX_NUMBER_OF_MATCHES
new blobs.  

NOTE: If a Blob has no potential matches (due to great distance or an empty 
list of new blobs), the processMatches() function will decrement that blob's 
deathCount and possibly delete it.
*******************************************************************************/
void BlobTracker::buildListOfPotentialMatches( vector<blobTracker::Blob> & newBlobs )
{
    int length = trackedBlobHolders_.size();
    
    for( int i = 0; i < length; ++i ) {
        blobTracker::TrackedBlobHolder * trackedBlobHolder = trackedBlobHolders_[i];
        trackedBlobHolder->setBestMatches( newBlobs, maxBlobDistanceSquared_ );
    }
}

/***************************************************************************//**
Sorts the vector of TrackedBlobHolder objects in ascending order for the 
distance to the first choice new blob match.  If a tracked blob has no potential
matches, its firstChoiceDistance() function will return an unrealistically 
large value that will cause the tracked blob to sort to the end of the vector.
*******************************************************************************/
void BlobTracker::sortTrackedBlobsByDistance()
{
    std::sort( trackedBlobHolders_.begin(), 
               trackedBlobHolders_.end(), 
               is_first_trackedBlob_lesser );

    ////if( debugPrintCurrentFrame_ ) { debugPrintListOfPotentialMatches(); }
}

/***************************************************************************//**
Cranks through the tracked blobs and tries to match each one with its first 
choice of new blob.  If a tracked blob has no matches, its deathCount will be 
decremented.  When the deathCount reaches zero, the tracked blob is deleted.
The concept here is that an unmatched tracked blob should linger for a few more 
frames before being deleted, just in case the tracked blob show up again.  This 
approach reduces the number of dropped blob IDs due to a finger moving a little 
too fast and fading out for just a few frames.
*******************************************************************************/
void BlobTracker::processMatches( vector<blobTracker::Blob> & newBlobs )
{
    elapsedTimeMillis_ = ofGetElapsedTimeMillis();

    for( int i = 0; i < trackedBlobHolders_.size(); ++i ) {
        blobTracker::TrackedBlobHolder * trackedBlobHolder = trackedBlobHolders_[i];

        if( trackedBlobHolder->hasMatchRequest() ) {
            int indexOfNewBlob = trackedBlobHolder->firstChoiceIndex();
            blobTracker::Blob & newBlob = newBlobs[indexOfNewBlob];

            if( newBlob.id == -1 ) {
                // New blob is not already taken, so a match is made.
                blobTracker::Blob & trackedBlob = trackedBlobHolder->trackedBlob();
                updateKeyParamsOfNewBlob( newBlob, trackedBlob );
                updateTrackedBlob( trackedBlob, newBlob );
            }
            else { // New blob is already taken, so check for next choice.
                //debugPrintLoserAndWinnerIDs( trackedBlobHolder->trackedBlob(), newBlob );
                //debugPrintListOfPotentialMatches();
                trackedBlobHolder->deleteCurrentFirstChoice();

                if( trackedBlobHolder->hasMatchRequest() ) {
                    //debugPrintAnotherMatchRequestMade();
                    bubbleUpToMaintainSortedState( i );
                }
                --i; // Whether empty or using next choice, evaluate again.
            }
        }
        else { // The tracked blob has no matches, so decrement the death count.
            blobTracker::Blob & trackedBlob = trackedBlobHolder->trackedBlob();
            --trackedBlob.deathCount;
            debugPrintDeathCount( trackedBlob );
            
            if( trackedBlob.deathCount <= 0 ) {
                // The tracked blob has gone missing too many frames.  Kill it.
                if( debugPrint_ ) { frameCountForLastDelete_ = frameCount_; }
                debugPrintTrackedBlobDeletion( trackedBlob );
                deleteTrackedBlob( i );
                --i;
            }
        }
    }
}

/***************************************************************************//**
Copies the blob ID as well as several other parameters from the tracked blob 
to the new blob.
*******************************************************************************/
void BlobTracker::updateKeyParamsOfNewBlob( blobTracker::Blob & newBlob, 
                                            blobTracker::Blob & trackedBlob )
{
    newBlob.id = trackedBlob.id;
    newBlob.age = trackedBlob.age;
    newBlob.areaSum = trackedBlob.areaSum;
    newBlob.areaFrameCount = trackedBlob.areaFrameCount;
    newBlob.areaTime = trackedBlob.areaTime;
    newBlob.averageArea = trackedBlob.averageArea;
    newBlob.sitting = trackedBlob.sitting;
    newBlob.downTime = trackedBlob.downTime;
    newBlob.lastTimeTimeWasChecked = trackedBlob.lastTimeTimeWasChecked;
}

/***************************************************************************//**
Uses old and new centroid positions to determine acceleration or time sitting.
May send out RAW (camera space) touch held or touch moved events, which the 
BlobCalibration track will listen for if in calibration mode.
*******************************************************************************/
void BlobTracker::updateTrackedBlob( blobTracker::Blob & trackedBlob, blobTracker::Blob & newBlob )
{
    // Update tracked blob and remember old centroid.
    ofPoint tempLastCentroid = trackedBlob.centroid;
    trackedBlob = newBlob;
    trackedBlob.lastCentroid = tempLastCentroid;
    calculateAcceleration( trackedBlob );
    calculateAgeAndTimeSitting( trackedBlob );

    //if blob has been 'holding/sitting' for 1 second send a held event
    if( trackedBlob.sitting > 1.0f ) {
        sendRAWTouchHeldEventIfCalibrating( trackedBlob );
        addContinuingTrackToOutputMap( trackedBlob );
        trackedBlob.sitting = -1; // Held event only happens once, so set to -1.
    } 
    else { // Must be moving.
        sendRAWTouchMovedEventIfCalibrating( trackedBlob );
        addContinuingTrackToOutputMap( trackedBlob );
    }
    // The last lastTimeTimeWasChecked is updated at the end 
    // after all acceleration values are calculated. -AlexP
    trackedBlob.lastTimeTimeWasChecked = elapsedTimeMillis_;
}

/***************************************************************************//**
Calculates acceleration based on centroid change and uses movement filter:

AlexP
Filter the blob position based on the movementFiltering_ value.
The range of values is [0, 15], so we will have that many 
filtering steps.  Here we have a weighted low-pass filter adaptively
adjust the blob position filtering strength based on blob movement
http://www.wolframalpha.com/input/?i=plot+1/exp(x/15)+and+1/exp(x/10)+and+1/exp(x/5)+from+0+to+100
*******************************************************************************/
void BlobTracker::calculateAcceleration( blobTracker::Blob & trackedBlob )
{
    // Get the change in centroid position and calculate acceleration.
    ofPoint tD;
    tD.set( trackedBlob.centroid.x - trackedBlob.lastCentroid.x, 
            trackedBlob.centroid.y - trackedBlob.lastCentroid.y );
    float posDelta = sqrtf((tD.x*tD.x)+(tD.y*tD.y));
    float a = 1.0f - 1.0f / expf(posDelta / (1.0f + (float)movementFiltering_*10));
    trackedBlob.centroid.x = a * trackedBlob.centroid.x + (1-a) * trackedBlob.lastCentroid.x;
    trackedBlob.centroid.y = a * trackedBlob.centroid.y + (1-a) * trackedBlob.lastCentroid.y;

    // Get the Differences in position.
    trackedBlob.D.set( trackedBlob.centroid.x - trackedBlob.lastCentroid.x, 
                       trackedBlob.centroid.y - trackedBlob.lastCentroid.y );

    // Calculate the acceleration again.
    tD = trackedBlob.D;
    trackedBlob.maccel = sqrtf( (tD.x* tD.x) + (tD.y*tD.y)) 
                    / (elapsedTimeMillis_ - trackedBlob.lastTimeTimeWasChecked ); 
}

/***************************************************************************//**
In addition to calculating the tracked blobs age and time sitting, the average 
blob area (for display in GUI) is calculated every so many seconds using the 
value in the AREA_REPORTING_INTERVAL_SECONDS constant. -Joe Weber
*******************************************************************************/
void BlobTracker::calculateAgeAndTimeSitting( blobTracker::Blob & trackedBlob )
{
    float t = ofGetElapsedTimef();
    trackedBlob.age = t - trackedBlob.downTime;
    trackedBlob.areaSum += trackedBlob.area;
    trackedBlob.areaFrameCount += 1;
    float elapsedAreaTime = t - trackedBlob.areaTime;

    // Update average area at the time interval AREA_REPORTING_INTERVAL_SECONDS.
    if( elapsedAreaTime > AREA_REPORTING_INTERVAL_SECONDS && trackedBlob.areaFrameCount > 0 ) {
        trackedBlob.averageArea = (int)(trackedBlob.areaSum / trackedBlob.areaFrameCount + 0.5);
        trackedBlob.areaSum = trackedBlob.area;
        trackedBlob.areaFrameCount = 0;
        trackedBlob.areaTime = t;
    }
    // Set sitting (held length)
    if( trackedBlob.maccel < 7 ) {	
        //1 more frame of sitting
        if( trackedBlob.sitting != -1 ) {
            trackedBlob.sitting = t - trackedBlob.downTime;
        }
    }
    else {
        trackedBlob.sitting = -1;
    }
}

/***************************************************************************//**
Uses the CalibrationUtils object to adjust the centroid and bounding box of the 
tracked blob before adding the map of calibrated tracked blobs.
*******************************************************************************/
void BlobTracker::addContinuingTrackToOutputMap( const blobTracker::Blob & trackedBlob )
{
    blobTracker::Blob calibratedBlob = trackedBlob;

    // Calibrate bounding box and centroid.
    calibrationUtils_->transformDimension( calibratedBlob.boundingRect.width, calibratedBlob.boundingRect.height );
    calibrationUtils_->cameraToScreenPosition( calibratedBlob.centroid.x, calibratedBlob.centroid.y );
    calibrationUtils_->cameraToScreenPosition( calibratedBlob.lastCentroid.x, calibratedBlob.lastCentroid.y );
    calibratedBlob.D.set( trackedBlob.centroid.x - trackedBlob.lastCentroid.x, trackedBlob.centroid.y - trackedBlob.lastCentroid.y );

    // Calibrated acceleration
    ofPoint tD = calibratedBlob.D;
    calibratedBlob.maccel = sqrtf((tD.x*tD.x)+(tD.y*tD.y)) / (elapsedTimeMillis_ - calibratedBlob.lastTimeTimeWasChecked);
    calibratedBlob.lastTimeTimeWasChecked = elapsedTimeMillis_;

    // Add to map of calibrated tracked blobs.
    calibratedTrackedBlobs_[calibratedBlob.id] = calibratedBlob;

    if( calibratedBlob.centroid.x > 0 || calibratedBlob.centroid.y > 0 ) { 
        hasBlobsInScreenArea_ = true; 
    }
}

/***************************************************************************//**
The index number must be to a valid trackedBlobHolder in the trackedBlobHolders 
vector, and the rest of the vector after that element must already be sorted in 
ascending order for the distance from each tracked blob to its first choice new
blob.  If the tracked blob at the element of interest has a distance less then 
the next element, then this function does not need to do anything.  However, 
if the element is not sorted, it will "bubble upwards" in the vector until it 
can be inserted before a tracked blob with a greater distance (or the end of the
vector is reached).
*******************************************************************************/
void BlobTracker::bubbleUpToMaintainSortedState( int index )
{
    TrackedBlobIterator iterPrev = trackedBlobHolders_.begin() + index,
                        iterCur = iterPrev + 1;

    while( iterCur != trackedBlobHolders_.end() ) {
        if( (*iterPrev)->firstChoiceDistance() <= (*iterCur)->firstChoiceDistance() ) {
            break; // Already sorted.
        }
        else { // Need to swap.
            blobTracker::TrackedBlobHolder * temp = *iterPrev;
            *iterPrev = *iterCur;
            *iterCur = temp;
        }
        ++iterPrev;
        ++iterCur;
    }
}

/***************************************************************************//**
If calibration is in-progress, a RAW (camera-space) touch event will be sent 
out before the tracked blob is deleted.
*******************************************************************************/
void BlobTracker::deleteTrackedBlob( int index )
{
    if( index >= 0 && index < trackedBlobHolders_.size() ) {
        TrackedBlobIterator iter = trackedBlobHolders_.begin() + index;
        blobTracker::TrackedBlobHolder * trackedBlobHolder = *iter;
        sendRAWTouchUpEventIfCalibrating( trackedBlobHolder->trackedBlob() );
        calibratedTrackedBlobs_.erase( trackedBlobHolder->trackedBlobId() );
        trackedBlobHolders_.erase( iter );
        delete trackedBlobHolder;
    }
}

/***************************************************************************//**
When this function is called, every blob should be either labeled with a tracked
id or have an id of -1.  If the id is -1, a new track must be made, and the 
tracked blob needs to be added to the map of calibrated tracked blobs, as well 
as to the vector of tracked blobs that will be needed for the next frame.
*******************************************************************************/
void BlobTracker::addNewTracks( vector<blobTracker::Blob> & newBlobs )
{
	for( int i = 0; i < numberOfNewBlobs_; ++i ) {
		if( newBlobs[i].id == -1 ) {
            blobTracker::Blob & unmatchedBlob = newBlobs[i];
			unmatchedBlob.id = uniqueIdCounter_++;
			unmatchedBlob.downTime = ofGetElapsedTimef();
            addToTrackedBlobs( unmatchedBlob );
            sendRAWTouchDownEventIfCalibrating( unmatchedBlob );

            //debugPrintNewTrackedBlob( unmatchedBlob );
            debugPrintNewTrackedBlobDiffCounts( unmatchedBlob );
            addNewTrackToOutputMap( unmatchedBlob );
		}
	}
}

/***************************************************************************//**
Creates a new TrackedBlobHolder object that stores info on the blob given as an arg.
*******************************************************************************/
void BlobTracker::addToTrackedBlobs( blobTracker::Blob & blob )
{
    blobTracker::TrackedBlobHolder * blobMatches = new blobTracker::TrackedBlobHolder();
    blobMatches->setTrackedBlob( blob );
    trackedBlobHolders_.push_back( blobMatches );
}

/***************************************************************************//**
Copies the blob and converts the centroid and bounding box from camera space 
to screen space before adding it to the map of tracked blobs that servers as 
the final output from the tracking algorithm.
*******************************************************************************/
void BlobTracker::addNewTrackToOutputMap( const blobTracker::Blob & trackedBlob )
{
    blobTracker::Blob calibratedBlob = trackedBlob;
    calibrationUtils_->transformDimension( calibratedBlob.boundingRect.width, 
                                           calibratedBlob.boundingRect.height);
    calibrationUtils_->cameraToScreenPosition( calibratedBlob.centroid.x,
                                               calibratedBlob.centroid.y );
    calibratedTrackedBlobs_[calibratedBlob.id] = calibratedBlob;

    if( calibratedBlob.centroid.x > 0 || calibratedBlob.centroid.y > 0 ) { 
        hasBlobsInScreenArea_ = true; 
    }
}

void BlobTracker::sendRAWTouchUpEventIfCalibrating( blobTracker::Blob & blob )
{
    if( isCalibrating_ ) { 
        touchEvents_.RAWmessengerBlob = blob;
        touchEvents_.notifyRAWTouchUp( NULL );
    }
}

void BlobTracker::sendRAWTouchHeldEventIfCalibrating( blobTracker::Blob & blob )
{
    if( isCalibrating_ ) { 
        touchEvents_.RAWmessengerBlob = blob;
        touchEvents_.notifyRAWTouchHeld( NULL );
    }
}

void BlobTracker::sendRAWTouchMovedEventIfCalibrating( blobTracker::Blob & blob )
{
    if( isCalibrating_ ) { 
        touchEvents_.RAWmessengerBlob = blob;
        touchEvents_.notifyRAWTouchMoved( NULL );
    }
}

void BlobTracker::sendRAWTouchDownEventIfCalibrating( blobTracker::Blob & blob )
{
    if( isCalibrating_ ) { 
        touchEvents_.RAWmessengerBlob = blob;
        touchEvents_.notifyRAWTouchDown( NULL );
    }
}

void BlobTracker::debugPrintLoserAndWinnerIDs( blobTracker::Blob & loser, blobTracker::Blob & winner )
{
    if( debugPrint_ ) {
        std::cout << "\nframe " 
                  << frameCount_
                  << ": id(loser, winner) = (" 
                  << loser.id 
                  << ", " 
                  << winner.id 
                  << "; (old tracks, new blobs) = (" 
                  << numberOfTracksLastFrame_
                  << ", " 
                  << numberOfNewBlobs_ 
                  << ")\n";
    }
}

void BlobTracker::debugPrintAnotherMatchRequestMade()
{
    if( debugPrint_ ) {
        std::cout << "tracked blob has another match request: frame "
                  << frameCount_ 
                  << "\n";
    }
}

void BlobTracker::debugPrintListOfPotentialMatches()
{
    if( debugPrint_ ) {
        std::cout << "frame " 
                  << frameCount_ 
                  << ": BlobTracker::debugPrintListOfPotentialMatches() called...\n\n";

        int length = trackedBlobHolders_.size();

        for( int i = 0; i < length; ++i ) {
            trackedBlobHolders_[i]->debugPrint();
        }
    }
}

void BlobTracker::debugPrintDeathCount( blobTracker::Blob & trackedBlob )
{
    if( debugPrint_ ) {
        std::cout << "frame "
                  << frameCount_ 
                  << ": Blob id = " << trackedBlob.id 
                  << " and death count = " << trackedBlob.deathCount 
                  << "\n";
    }
}

void BlobTracker::debugPrintTrackedBlobDeletion( blobTracker::Blob & trackedBlob )
{
    if( debugPrint_ ) {
        std::cout << "frame "
                  << frameCount_ 
                  << ": Deleting track id = " 
                  << trackedBlob.id 
                  << "; (old tracks, new blobs) = (" 
                  << numberOfTracksLastFrame_
                  << ", " 
                  << numberOfNewBlobs_ 
                  << ") *****\n";
    }
}

void BlobTracker::debugPrintNewTrackedBlob( blobTracker::Blob & blob )
{
    if( debugPrint_ ) {
        std::cout << "frame " 
                  << frameCount_ 
                  << std::fixed << std::setprecision( 1 )
                  << ": NEW TRACK id = " 
                  << blob.id 
                  << "; (old tracks, new blobs) = (" 
                  << numberOfTracksLastFrame_ 
                  << ", " 
                  << numberOfNewBlobs_ 
                  << ")\n";
    }
}

void BlobTracker::debugPrintNewTrackedBlobDiffCounts( blobTracker::Blob & blob )
{
    if( debugPrint_ ) {
        int frameDiff = frameCount_ - frameCountForLastDelete_;

        if( frameDiff >= 1 && frameDiff <= MAX_DIFF_INDEX ) {
            ++diffCounts_[frameDiff];
            ++diffCounts_[0];
        }
        else {
            ++diffCounts_[MAX_DIFF_INDEX];
            ++diffCounts_[0];
        }
        if( (diffCounts_[0] % 100) == 0 ) {
            double sum = diffCounts_[0];

            std::cout << "frame " 
                      << frameCount_ 
                      << std::fixed << std::setprecision( 1 )
                      << ": NEW TRACK id = " 
                      << blob.id 
                      << "; (old tracks, new blobs) = (" 
                      << numberOfTracksLastFrame_ 
                      << ", " 
                      << numberOfNewBlobs_ 
                      << ")\n"
                      << "Frame diff = " 
                      << frameDiff 
                      << ": total(bin size 1 to n...) = (";

            for( int i = 1; i <= MAX_DIFF_INDEX; ++i ) {
                std::cout << std::fixed << std::setprecision( 2 ) << diffCounts_[i] / sum;
                if( i < MAX_DIFF_INDEX ) { std::cout << ", "; }
            }
            std::cout << ")\nsum = " << sum << "\n";
        }
    }
}
