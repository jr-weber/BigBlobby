/*******************************************************************************
TrackedBlobHolder

Purpose: This class holds a tracked Blob along with information on what new 
         Blobs the tracked Blob wants to be matched with.

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
#ifndef BLOBTRACKER_BLOBMATCHES_H
#define BLOBTRACKER_BLOBMATCHES_H

#include "blobTracker/Blob.h"
#include "blobTracker/BlobMatch.h"
#include <vector>
#include <list>

namespace blobTracker
{
    class TrackedBlobHolder
    {
    public:
        static const int MAX_NUMBER_OF_MATCHES;

        TrackedBlobHolder();
        ~TrackedBlobHolder();

        void setTrackedBlob( const blobTracker::Blob & blob );
        blobTracker::Blob & trackedBlob();
        int trackedBlobId();

        int firstChoiceIndex() const;
        double firstChoiceDistance() const;
        void deleteCurrentFirstChoice();

        void setBestMatches( std::vector<blobTracker::Blob> & newBlobs,
                             double maxBlobDistanceSquared );
        bool hasMatchRequest();
        void debugPrint();

    private:
        double distanceSquared( blobTracker::Blob & blob1, blobTracker::Blob & blob2 );

        blobTracker::Blob trackedBlob_;
        std::list<blobTracker::BlobMatch> newBlobMatches_;
    };
}

#endif
