////////////////////////////////////////////////////////////////////////////////
// ImageFilters
//
// PURPOSE: Holds the filters that are applied to the camera image before the
//          image is given to the contour finder.
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
#ifndef BLOBFILTERS_IMAGEFILTERS_H
#define BLOBFILTERS_IMAGEFILTERS_H

#include "blobEnums/FilterEnum.h"

namespace blobBuffers { class GrayBuffer; }
namespace blobBuffers { class BufferUtility; }
namespace blobFilters { class BackgroundFilter; }
namespace blobFilters { class SignalNormalizationFilter; }
namespace blobFilters { class MaskFilter; }
namespace blobFilters { class HighpassFilter; }
namespace blobFilters { class SmoothingFilter; }
namespace blobFilters { class ThresholdFilter; }
namespace blobStats { class FilterStats; }
namespace blobUtils { class FunctionTimer; }
namespace blobNormalize { class CalibrationGridPoints; }
namespace blobNormalize { class FishEyeGrid; }

namespace blobFilters
{
    class ImageFilters
    {
    public:
        ImageFilters();
        virtual ~ImageFilters();

        void processGrayscaleImage( unsigned char * grayPixels, int width, int height );
        void processRgbImage( unsigned char * colorPixels, int width, int height );

        blobStats::FilterStats * calculateStats();
        void saveNextFrameAsBackground();
        void saveFramesAsBackground( int count );
        void setAutoBackgroundActive( bool b );
        bool isAutoBackgroundActive();
        void setBackgroundLearnRate( float rate );
        void useDarkBlobs( bool b );
        bool useDarkBlobs();
        void setMaskCalibrationGridPoints( blobNormalize::CalibrationGridPoints * calGridPoints );
        void setSignalFishEyeGrid( blobNormalize::FishEyeGrid * fishEyeGrid );

        void usePeriodicBackground( bool b );
        bool isPeriodicBackgroundActive();
        void setPeriodicBackgroundSeconds( float t );
        float getPeriodicBackgroundSeconds();
        void setPeriodicBackgroundPercent( float percent );
        float getPeriodicBackgroundPercent();
        void checkPeriodicBackground();
        
        void flipImageHorizontal( bool b );
        bool flipImageHorizontal();
        void flipImageVertical( bool b );
        bool flipImageVertical();

        void setPreBackgroundSmoothingActive( bool active );
        bool isPreBackgroundSmoothingActive();
        void setPreBackgroundSmoothingBlurSize( int size );
        int preBackgroundSmoothingBlurSize();
        void useGaussianPreBackgroundSmoothing( bool b );
        void setGaussianPreBackgroundSmoothingSigma( double sigma );
        bool isGaussianPreBackgroundSmoothingActive();
        double gaussianPreBackgroundSmoothingSigma();

        void setMaskActive( bool active );
        bool isMaskActive();
        void setMaskWidthPadding( int pixels );
        int maskWidthPadding();
        void setMaskHeightPadding( int pixels );
        int maskHeightPadding();

        void setSignalNormalizationActive( bool b );
        bool isSignalNormalizationActive();

        void setHighpassActive( bool active );
        bool isHighpassActive();
        void setHighpassBlurSize( int size );
        int highpassBlurSize();
        void showHighpassSubtractionImage( bool show );
        void setHighpassAmplifyActive( bool active );
        bool isHighpassAmplifyActive();
        void setHighpassAmplifyLevel( int level );
        int highpassAmplifyLevel();

        void setHighpassTwoActive( bool active );
        bool isHighpassTwoActive();
        void setHighpassTwoKernel( int size );
        int highpassTwoKernel();
        void showHighpassTwoSubtractionImage( bool show );
        void setHighpassTwoAmplifyActive( bool active );
        bool isHighpassTwoAmplifyActive();
        void setHighpassTwoAmplifyLevel( int level );
        int highpassTwoAmplifyLevel();

        void setSmoothingActive( bool active );
        bool isSmoothingActive();
        void setSmoothingBlurSize( int size );
        int smoothingBlurSize();
        void useGaussianSmoothing( bool b );
        void setGaussianSmoothingSigma( double sigma );
        bool isGaussianSmoothingActive();
        double gaussianSmoothingSigma();
        void setSmoothingAmplifyActive( bool active );
        bool isSmoothingAmplifyActive();
        void setSmoothingAmplifyLevel( int level );
        int smoothingAmplifyLevel();

        void setImageThreshold( int threshold );
        int imageThreshold();

        blobBuffers::GrayBuffer * grayBuffer( blobEnums::FilterEnum::Enum filterType );
        blobBuffers::GrayBuffer * graySourceBuffer();
        blobBuffers::GrayBuffer * grayOutputBuffer();
        blobBuffers::GrayBuffer * differenceBuffer();
        blobBuffers::GrayBuffer * smoothedSourceBuffer();
        blobBuffers::GrayBuffer * backgroundBuffer();

    private:
        void clearBuffers();
        void clearFilterBuffers();
        void deleteTimers();
        void deleteFilters();
        void initializeBuffers( int width, int height );
        void initializeFilterBuffers();

        void processImage( unsigned char * pixels, int width, int height, int channels );
        void copyPixelsToInputBuffer( unsigned char * pixels, int width, int height, int channels );
        void applyPreBackgroundSmoothingFilter();
        void applyMaskFilter();
        void applyBackgroundFilter();
        void applySignalNormalizationFilter();
        void applyHighpassFilter();
        void applyHighpassTwoFilter();
        void applySmoothingFilter();
        void applyThresholdFilter();

        int bufferWidth_,
            bufferHeight_;

        blobBuffers::GrayBuffer * inputBuffer_,
                                * preBackgroundSmoothingBuffer_,
                                * backgroundBufferCopy_,
                                * diffBuffer_,
                                * normalizeBuffer_,
                                * highpassBuffer_,
                                * highpassTwoBuffer_,
                                * smoothingBuffer_,
                                * grayOutputBuffer_;

        blobBuffers::BufferUtility * bufferUtility_;
        blobFilters::SmoothingFilter * preBackgroundSmoothingFilter_;
        blobFilters::MaskFilter * maskFilter_;
        blobFilters::BackgroundFilter * backgroundFilter_;
        blobFilters::SignalNormalizationFilter * signalNormalizationFilter_;
        blobFilters::HighpassFilter * highpassFilter_;
        blobFilters::HighpassFilter * highpassTwoFilter_;
        blobFilters::SmoothingFilter * smoothingFilter_;
        blobFilters::ThresholdFilter * thresholdFilter_;

        blobUtils::FunctionTimer * processTimer_,
                                 * copyCameraPixelsTimer_;
        blobStats::FilterStats * filterStats_;
    };
}

#endif
