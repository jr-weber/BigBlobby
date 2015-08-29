////////////////////////////////////////////////////////////////////////////////
// GridPoint
//
// PURPOSE: Holds the xy-coordinates of a calibration grid point along with the
//          source intensity and background intensity at that grid point (the
//          centroid of a blob).  The signal (source minus background) tends
//          to be weaker at edges and corners of the calibration grid, due at
//          least in part to the "fish eye" effect of the camera.  The relative
//          signal intensity for calibration points (edges compared to center)
//          will be used in the SignalNormalizationFilter to make corrections.
//
// CREATED: 6/13/2014
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
#include "blobNormalize/GridPoint.h"

using blobNormalize::GridPoint;

GridPoint::GridPoint() :
  x_( 0.0f ),
  y_( 0.0f ),
  srcMinusBkIntensity_( 0.0f ),
  sourceIntensity_( 0.0f ),
  backgroundIntensity_( 0.0f ),
  intensityNormalizationFactor_( 1.0f )
{
}

GridPoint::GridPoint( const GridPoint & other ) :
  x_( other.x_ ),
  y_( other.y_ ),
  srcMinusBkIntensity_( other.srcMinusBkIntensity_ ),
  sourceIntensity_( other.sourceIntensity_ ),
  backgroundIntensity_( other.backgroundIntensity_ ),
  intensityNormalizationFactor_( other.intensityNormalizationFactor_ )
{
}

GridPoint::~GridPoint()
{
}

float GridPoint::x()
{
    return x_;
}

float GridPoint::y()
{
    return y_;
}

float GridPoint::srcMinusBkIntensity()
{
    return srcMinusBkIntensity_;
}

float GridPoint::sourceIntensity()
{
    return sourceIntensity_;
}

float GridPoint::backgroundIntensity()
{
    return backgroundIntensity_;
}

float GridPoint::intensityNormalization()
{
    return intensityNormalizationFactor_;
}

void GridPoint::setXY( float x, float y )
{
    x_ = x;
    y_ = y;
}

void GridPoint::setSrcMinusBkIntensity( float intensity )
{
    srcMinusBkIntensity_ = intensity;
}

void GridPoint::setSourceIntensity( float intensity )
{
    sourceIntensity_ = intensity;
}

void GridPoint::setBackgroundIntensity( float intensity )
{
    backgroundIntensity_ = intensity;
}

void GridPoint::calculateSrcMinusBackground()
{
    float diff = sourceIntensity_ - backgroundIntensity_;
    srcMinusBkIntensity_ = (diff > 0.0f) ? diff : 0.0f;
}

void GridPoint::setIntensityNormalization( float factor )
{
    intensityNormalizationFactor_ = factor;
}

GridPoint & GridPoint::operator=( const GridPoint & rhs )
{
    x_ = rhs.x_;
    y_ = rhs.y_;
    srcMinusBkIntensity_ = rhs.srcMinusBkIntensity_;
    sourceIntensity_ = rhs.sourceIntensity_;
    backgroundIntensity_ = rhs.backgroundIntensity_;
    intensityNormalizationFactor_ = rhs.intensityNormalizationFactor_;
    return *this;
}
