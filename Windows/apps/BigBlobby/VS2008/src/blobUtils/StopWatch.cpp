////////////////////////////////////////////////////////////////////////////////
// StopWatch
//
// PURPOSE: Provides a time keeper for benchmarking functions.  The chrono 
//          library of Boost is used to get the system time.  The time is
//          reported in seconds, but as a floating decimal place number that
//          should have microsecond accuracy (on most machines).
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
#ifdef _WIN32
/******************************************************************************/
/************************* Windows Version with Boost *************************/
/******************************************************************************/
#include "blobUtils/StopWatch.h"
#include <iostream>

using blobUtils::StopWatch;
using boost::chrono::system_clock;

StopWatch::StopWatch() :
  startTime_( system_clock::now() )
{
}

StopWatch::~StopWatch()
{
}

void StopWatch::start()
{
    startTime_ = system_clock::now();
}

double StopWatch::restart()
{
    boost::chrono::duration<double> sec = system_clock::now() - startTime_;
    startTime_ = system_clock::now();
    return sec.count();
}

double StopWatch::elapsed()
{
    boost::chrono::duration<double> sec = system_clock::now() - startTime_;
    return sec.count();
}

void StopWatch::print( double timeInSeconds )
{
    std::cout << "t = " << timeInSeconds << " seconds\n" << std::flush;
}
#else
/******************************************************************************/
/*********************** Linux Version with sys/timeb.h ***********************/
/******************************************************************************/
#include "ghostUtils/StopWatch.h"
#include <iostream>
#include <time.h>
#include <sys/timeb.h>

using blobUtils::StopWatch;

StopWatch::StopWatch() :
  startTime_( seconds() )
{
}

StopWatch::~StopWatch()
{
}

void StopWatch::start()
{
    startTime_ = seconds();
}

double StopWatch::restart()
{
    double diff = seconds() - startTime_;
    startTime_ = diff;
    return diff;
}

double StopWatch::elapsed()
{
    return seconds() - startTime_;
}

void StopWatch::print( double timeInSeconds )
{
    std::cout << "t = " << timeInSeconds << " seconds\n" << std::flush;
}

double StopWatch::seconds()
{
    timespec ts;
    clock_gettime( CLOCK_REALTIME, &ts );
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}
#endif