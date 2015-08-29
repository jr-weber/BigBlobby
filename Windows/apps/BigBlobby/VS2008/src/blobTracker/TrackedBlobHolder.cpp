/*******************************************************************************
TrackedBlobHolder

Purpose: Holds a tracked blob along with information on possible matches 
         between the tracked blob (from a previous frame) and new blobs
         (from the current frame).

Author: J.R.Weber <joe.weber77@gmail.com> 
*******************************************************************************/
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
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
#include "blobTracker/TrackedBlobHolder.h"
#include <cmath>

using blobTracker::TrackedBlobHolder;

const int TrackedBlobHolder::MAX_NUMBER_OF_MATCHES = 6;

TrackedBlobHolder::TrackedBlobHolder() :
  trackedBlob_(),
  newBlobMatches_()
{
}

TrackedBlobHolder::~TrackedBlobHolder()
{
}

/***************************************************************************//**
Copies info from the blob given as an argument, including the ID number.  If 
this function has not been called yet, the default tracked blob held by this 
object has an ID of -1.
*******************************************************************************/
void TrackedBlobHolder::setTrackedBlob( const blobTracker::Blob & blob )
{
    trackedBlob_ = blob;
}

/***************************************************************************//**
Be careful here.  The blob reference is only good as long as this object exists.
*******************************************************************************/
blobTracker::Blob & TrackedBlobHolder::trackedBlob()
{
    return trackedBlob_;
}

/***************************************************************************//**
This function should be called before trying to use firstChoiceIndex() or 
firstChoiceDistance().
*******************************************************************************/
bool TrackedBlobHolder::hasMatchRequest()
{
    return (newBlobMatches_.size() > 0);
}

int TrackedBlobHolder::trackedBlobId()
{
    return trackedBlob_.id;
}

int TrackedBlobHolder::firstChoiceIndex() const
{
    int index = -1;

    if( newBlobMatches_.size() > 0 ) {
        index = newBlobMatches_.front().newBlobIndex();
    }
    return index;
}

double TrackedBlobHolder::firstChoiceDistance() const
{
    double distance = 1000000000.0;

    if( newBlobMatches_.size() > 0 ) {
        distance = newBlobMatches_.front().distance();
    }
    return distance;
}

void TrackedBlobHolder::deleteCurrentFirstChoice()
{
    if( !newBlobMatches_.empty() ) {
        newBlobMatches_.pop_front();
    }
}

/***************************************************************************//**
The tracked blob must be set before calling this function.  This function will 
build and save a list of the new blobs that are closest to the tracked blob.  
The list of matches will never be greater in length than MAX_NUMBER_OF_MATCHES,
and the distance (squared) will never be greater than the maxBlobDistanceSquared
(square of the distance is used to avoid square root calculations in the
Pythagorean theorem).  The list of matches will be empty if the vector of new 
blobs given as an argument is empty or if the tracked blob is not valid 
(an ID of -1 is non-valid track).
*******************************************************************************/
void TrackedBlobHolder::setBestMatches( std::vector<blobTracker::Blob> & newBlobs,
                                        double maxBlobDistanceSquared )
{
    int numberOfNewBlobs = newBlobs.size();
    newBlobMatches_.clear();

    if( numberOfNewBlobs > 0 && trackedBlob_.id != -1) {
        // Declare the first new blob as the current best match for distance.
        double distance = distanceSquared( newBlobs[0], trackedBlob_ );
        blobTracker::BlobMatch currentMatch( 0, distance );
        newBlobMatches_.push_back( currentMatch );

        // Look for better matches and store up to MAX_NUMBER_OF_MATCHES.
        for( int i = 1; i < numberOfNewBlobs; ++i ) {
            distance = distanceSquared( newBlobs[i], trackedBlob_ );

            if( distance <= maxBlobDistanceSquared ) {
                // Within the allowed distance, so add to sorted linked list.
                std::list<BlobMatch>::iterator iter = newBlobMatches_.begin(),
                                               end = newBlobMatches_.end();
                
                // Search for first existing match where distance is not greater.
                while( iter != end && distance >= iter->distance() ) {
                    ++iter;
                }
                // Iterator is now pointing at either a bigger distance or the end.
                if( iter != end || newBlobMatches_.size() < MAX_NUMBER_OF_MATCHES ) {
                    // Insert new blob in the list of matches.
                    currentMatch.setIndexAndDistance( i, distance );
                    newBlobMatches_.insert( iter, currentMatch );

                    if( newBlobMatches_.size() > MAX_NUMBER_OF_MATCHES ) {
                        // There are too many matches, so trim the list.
				        newBlobMatches_.pop_back();
                    }
                }
            }
        }
    }
}

/***************************************************************************//**
Returns the square of the distance between the two blobs.
*******************************************************************************/
double TrackedBlobHolder::distanceSquared( blobTracker::Blob & blob1, 
                                     blobTracker::Blob & blob2 )
{
    double xDistance = blob1.centroid.x - blob2.centroid.x,
           yDistance = blob1.centroid.y - blob2.centroid.y;
    return (xDistance * xDistance) + (yDistance * yDistance);
}

void TrackedBlobHolder::debugPrint()
{
    int numMatches = newBlobMatches_.size();
    std::cout << "\nTracked blob id = " << trackedBlob_.id 
              << ": matches = " << numMatches << "\n";

    std::list<BlobMatch>::iterator iter = newBlobMatches_.begin(),
                                   end = newBlobMatches_.end();

    while( iter != end ) {
        int index = iter->newBlobIndex();
        double distance = iter->distance();
        std::cout << "    index " << index 
                  << ": distance = " << std::sqrt( distance ) << "\n";
        ++iter;
    }
    std::cout << "\n";
}
