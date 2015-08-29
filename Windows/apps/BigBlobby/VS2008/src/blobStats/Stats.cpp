////////////////////////////////////////////////////////////////////////////////
// Stats.cpp
//
// PURPOSE: Used to produce formatted strings for reporting statistics such as
//          processing time for various operations.
//
// CREATED: 7/27/2012
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2012, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#include "blobStats/Stats.h"
#include "blobStats/FilterStats.h"
#include <iomanip>

using blobStats::Stats;

Stats::Stats() :
  showProcessingTime_( false )
{
}

Stats::~Stats()
{
}

void Stats::showProcessingTime( bool b )
{
    showProcessingTime_ = b;
}

std::string Stats::fpsStats( double fps, int videoWidth, int videoHeight )
{
    double avgFrameTime = (fps > 0) ? (1.0 / fps) : 0.0;
    std::stringstream strStream;
    strStream << std::fixed << std::setprecision( 0 )
              << "Frame Rate:  " << fps << " fps\n"
              << "Resolution:    " << videoWidth << " x " << videoHeight << "\n"
              << std::setprecision( 1 )
              << "Frame Time:  " << (avgFrameTime * 1000) << + " millisec";
    
    return strStream.str();
}

std::string Stats::networkAndFilterStats( const std::string & tuioOutputSummary,
                                          blobStats::FilterStats * filterStats,
                                          double contourProcessingTime,
                                          double blobProcessingTime,
                                          double tuioChannelsTime,
                                          double processingTime,
                                          double videoWriterTime,
                                          bool isRecording )
{
    std::stringstream strStream;
    strStream << tuioOutputSummary;
    
    if( showProcessingTime_ ) {
        addProcessingTime( strStream, 
                           filterStats, 
                           contourProcessingTime,
                           blobProcessingTime, 
                           tuioChannelsTime,
                           processingTime );
    }
    if( isRecording ) {
        strStream << "Video Writer:\n"
                  << (videoWriterTime * 1000) 
                  << " millisec\n";
    }
    return strStream.str();
}

void Stats::addProcessingTime( std::stringstream & strStream,
                               blobStats::FilterStats * filterStats,
                               double contourProcessingTime,
                               double blobProcessingTime,
                               double tuioChannelsTime,
                               double processingTime )
{
    if( filterStats != NULL ) {
        double filterTime = filterStats->avgProcessTime() * 1000,
               copyCameraTime = filterStats->avgCopyCameraPixelsTime() * 1000,
               //earlySmoothing = filterStats->avgPreBkSmoothTime() * 1000,
               backgroundTime = filterStats->avgBackgroundTime() * 1000,
               normalizeTime = filterStats->avgNormalizeTime() * 1000,
               highpassTime = filterStats->avgHighpassTime() * 1000,
               highpass2Time = filterStats->avgHighpassTwoTime() * 1000,
               smoothing2 = filterStats->avgSmoothTime() * 1000,
               thresholdTime = filterStats->avgThresholdTime() * 1000;

        strStream << std::fixed 
                  << std::setprecision( 3 ) 
                  << "\nProcessing Time (milliseconds)"
                  << "\nCopy Image:   " << copyCameraTime
                  //<< "\nEarly Smoothing:   " << earlySmoothing
                  << "\nBackground:    " << backgroundTime
                  << "\nNormalize:       " << normalizeTime
                  << "\nHighpass 1:     " << highpassTime
                  << "\nHighpass 2:     " << highpass2Time
                  << "\nSmoothing:      " << smoothing2
                  << "\nThreshold:       " << thresholdTime
                  
                  << "\n\nTotal Filters:      " << filterTime
                  << "\nFind Contours:   " << contourProcessingTime * 1000
                  << "\nTrack Blobs:       " << blobProcessingTime * 1000
                  << "\nTUIO Servers:   "   << tuioChannelsTime * 1000
                  << "\nTotal Time:        " << processingTime * 1000;
    }
}
