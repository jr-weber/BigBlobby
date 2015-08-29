////////////////////////////////////////////////////////////////////////////////
// BackgroundFilter
//
// PURPOSE: Knows how to subtract a background from an image an image.
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
#ifndef BLOBFILTERS_BACKGROUNDFILTER_H
#define BLOBFILTERS_BACKGROUNDFILTER_H

#include "blobFilters/BlobFilter.h"
#include <opencv2/core/core.hpp>
#include <vector>

namespace blobBuffers { class GrayBuffer; }
namespace blobBuffers { class GrayBufferShort; }
namespace cv { class Mat; }
namespace blobUtils { class BackgroundTimer; }

namespace blobFilters
{
    class BackgroundFilter : public BlobFilter
    {
    public:
        BackgroundFilter();
        virtual ~BackgroundFilter();

        virtual void process( blobBuffers::GrayBuffer * srcImage,
                              blobBuffers::GrayBuffer * dstImage );
        
        blobBuffers::GrayBuffer * backgroundImage();
        void saveNextFrameAsBackground();
        void saveFramesAsBackground( int count );
        void setBackgroundLearnRate( float rate );
        void setAutoBackgroundActive( bool b );
        bool isAutoBackgroundActive();
        void useDarkBlobs( bool b );
        bool useDarkBlobs();

        void setPeriodicBackgroundActive( bool b );
        bool isPeriodicBackgroundActive();
        void setPeriodicBackgroundSeconds( float t );
        float getPeriodicBackgroundSeconds();
        void setPeriodicBackgroundPercent( float percent );
        float getPeriodicBackgroundPercent();
        void checkPeriodicBackground();

    private:
        void clear();
        void clearMatrices();
        void clearBuffers();
        void initialize( int width, int height );
        void initializeBuffers();
        void initializeMatrices();
        void updateWeightedBackground( cv::Mat & srcM, int rows, int cols );
        void updateBackground( cv::Mat & srcM, int rows, int cols );
        bool isWithinBufferSize( const std::vector<cv::Point> & quad );

        int bufferWidth_,
            bufferHeight_;
        blobBuffers::GrayBuffer * srcCopy_,
                                * backgroundImage_;
        blobBuffers::GrayBufferShort * backgroundImageShort_,
                                     * backgroundImageShortTemp_;
        cv::Mat * srcCopyM_,
                * backgroundM_,
                * backgroundShortM_,
                * backgroundShortTempM_;
        float learnRate_;
        int frameCountDown_;
        bool saveNextFrame_,
             isAutoBackgroundActive_,
             useDarkBlobs_,
             useBlur_;
        blobUtils::BackgroundTimer * periodicBackgroundTimer_;
        float periodicBackgroundPercent_,
              periodicBackgroundFraction_;
    };
}

#endif
