////////////////////////////////////////////////////////////////////////////////
// FunctionTimer
//
// PURPOSE: Operates a StopWatch that is intended to be used for timing how
//          long some functions take.
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
#include "blobUtils/FunctionTimer.h"
#include "blobUtils/StopWatch.h"

using blobUtils::FunctionTimer;

FunctionTimer::FunctionTimer() :
  stopWatch_( new blobUtils::StopWatch() ),
  totalTime_( 0.0 ),
  numFunctionCalls_( 0 )
{
}

FunctionTimer::~FunctionTimer()
{
    delete stopWatch_;
}

void FunctionTimer::startTiming()
{
    stopWatch_->start();
}

void FunctionTimer::stopTiming()
{
    totalTime_ += stopWatch_->elapsed();
    ++numFunctionCalls_;
}

double FunctionTimer::calculateAverageTime()
{
    double averageTime = 0.0;
    
    if( numFunctionCalls_ > 0 ) {
        averageTime = totalTime_ / numFunctionCalls_;
    }
    totalTime_ = 0.0;
    numFunctionCalls_ = 0;
    return averageTime;
}
