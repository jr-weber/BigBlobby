/*******************************************************************************
BlobMatch

Purpose: Holds info on a possible match between a tracked blob from the previous
         frame and a new blob from the current frame.

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
#include "blobTracker/BlobMatch.h"

using blobTracker::BlobMatch;

BlobMatch::BlobMatch() :
  newBlobIndex_( -1 ),
  distance_( 0.0 )
{
}

BlobMatch::BlobMatch( int indexOfNewBlob, double distanceToNewBlob ) :
  newBlobIndex_( indexOfNewBlob ),
  distance_( distanceToNewBlob )
{
}

BlobMatch::~BlobMatch()
{
}

void BlobMatch::setIndexAndDistance( int index, double distance )
{
    newBlobIndex_ = index;
    distance_ = distance;
}

int BlobMatch::newBlobIndex() const
{
    return newBlobIndex_;
}

double BlobMatch::distance() const
{
    return distance_;
}
