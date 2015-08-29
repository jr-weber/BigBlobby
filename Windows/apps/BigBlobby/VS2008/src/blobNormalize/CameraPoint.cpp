////////////////////////////////////////////////////////////////////////////////
// CameraPoint
//
// PURPOSE: Used by the FastCalibration class to hold the (x, y) position and
//          signal intensity (source minus background) for a camera point.
//          The signal intensity is in 8-bit grayscale (integers 0 to 255).
//
// CREATED: 8/22/2014
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
#include "blobNormalize/CameraPoint.h"

using blobNormalize::CameraPoint;

CameraPoint::CameraPoint() :
  x_( 0.0f ),
  y_( 0.0f ),
  sourceIntensity_( 0.0f ),
  backgroundIntensity_( 0.0f )
{
}

CameraPoint::CameraPoint( float x, 
                          float y, 
                          float sourceIntensity, 
                          float backgroundIntensity ) :
  x_( x ),
  y_( y ),
  sourceIntensity_( sourceIntensity ),
  backgroundIntensity_( backgroundIntensity )
{

}

CameraPoint::CameraPoint( const CameraPoint & other ) :
  x_( other.x_ ),
  y_( other.y_ ),
  sourceIntensity_( other.sourceIntensity_ ),
  backgroundIntensity_( other.backgroundIntensity_ )
{
}

CameraPoint::~CameraPoint()
{
}

float CameraPoint::x() const
{
    return x_;
}

float CameraPoint::y() const
{
    return y_;
}

float CameraPoint::sourceIntensity() const
{
    return sourceIntensity_;
}

float CameraPoint::backgroundIntensity() const
{
    return backgroundIntensity_;
}

void CameraPoint::set( float x, 
                       float y,  
                       float sourceIntensity, 
                       float backgroundIntensity )
{
    x_ = x;
    y_ = y;
    sourceIntensity_ = sourceIntensity;
    backgroundIntensity_ = backgroundIntensity;
}

CameraPoint & CameraPoint::operator=( const CameraPoint & rhs )
{
    x_ = rhs.x_;
    y_ = rhs.y_;
    sourceIntensity_ = rhs.sourceIntensity_;
    backgroundIntensity_ = rhs.backgroundIntensity_;
    return *this;
}