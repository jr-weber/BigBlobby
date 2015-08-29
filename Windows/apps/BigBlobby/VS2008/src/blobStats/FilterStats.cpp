////////////////////////////////////////////////////////////////////////////////
// FilterStats
//
// PURPOSE: Keeps track of the average time for filter operations.
//
// CREATED: 5/20/2014
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2014, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#include "blobStats/FilterStats.h"

using blobStats::FilterStats;

FilterStats::FilterStats() :
  avgCopyCameraPixelsTime_( 0.0 ),
  avgProcessTime_( 0.0 ),
  avgPreBkSmoothTime_( 0.0 ),
  avgBackgroundTime_( 0.0 ),
  avgNormalizeTime_( 0.0 ),
  avgHighpassTime_( 0.0 ),
  avgHighpassTwoTime_( 0.0 ),
  avgSmoothTime_( 0.0 ),
  avgThresholdTime_( 0.0 )
{
}

FilterStats::~FilterStats()
{
}

void FilterStats::setAvgCopyCameraPixelsTime( double seconds )
{
    avgCopyCameraPixelsTime_ = seconds;
}

void FilterStats::setAvgProcessTime( double seconds )
{
    avgProcessTime_ = seconds;
}

void FilterStats::setAvgPreBkSmoothTime( double seconds )
{
    avgPreBkSmoothTime_ = seconds;
}

void FilterStats::setAvgBackgroundTime( double seconds )
{
    avgBackgroundTime_ = seconds;
}

void FilterStats::setAvgNormalizeTime( double seconds )
{
    avgNormalizeTime_ = seconds;
}

void FilterStats::setAvgHighpassTime( double seconds )
{
    avgHighpassTime_ = seconds;
}

void FilterStats::setAvgHighpassTwoTime( double seconds )
{
    avgHighpassTwoTime_ = seconds;
}

void FilterStats::setAvgSmoothTime( double seconds )
{
    avgSmoothTime_ = seconds;
}

void FilterStats::setAvgThresholdTime( double seconds )
{
    avgThresholdTime_ = seconds;
}

double FilterStats::avgCopyCameraPixelsTime()
{
    return avgCopyCameraPixelsTime_;
}

double FilterStats::avgProcessTime()
{
    return avgProcessTime_;
}

double FilterStats::avgPreBkSmoothTime()
{
    return avgPreBkSmoothTime_;
}

double FilterStats::avgBackgroundTime()
{
    return avgBackgroundTime_;
}

double FilterStats::avgNormalizeTime()
{
    return avgNormalizeTime_;
}

double FilterStats::avgHighpassTime()
{
    return avgHighpassTime_;
}

double FilterStats::avgHighpassTwoTime()
{
    return avgHighpassTwoTime_;
}

double FilterStats::avgSmoothTime()
{
    return avgSmoothTime_;
}

double FilterStats::avgThresholdTime()
{
    return avgThresholdTime_;
}
