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
#include "blobFilters/ImageFilters.h"
#include "blobBuffers/BufferUtility.h"
#include "blobFilters/BackgroundFilter.h"
#include "blobFilters/SignalNormalizationFilter.h"
#include "blobFilters/MaskFilter.h"
#include "blobFilters/HighpassFilter.h"
#include "blobFilters/SmoothingFilter.h"
#include "blobFilters/ThresholdFilter.h"
#include "blobBuffers/GrayBuffer.h"
#include "blobStats/FilterStats.h"
#include "blobUtils/FunctionTimer.h"
#include <iostream>

using blobFilters::ImageFilters;
using blobEnums::FilterEnum;

ImageFilters::ImageFilters() :
  bufferWidth_( 0 ),
  bufferHeight_( 0 ),
  inputBuffer_( NULL ),
  preBackgroundSmoothingBuffer_( NULL ),
  backgroundBufferCopy_( NULL ),
  diffBuffer_( NULL ),
  normalizeBuffer_( NULL ),
  highpassBuffer_( NULL ),
  highpassTwoBuffer_( NULL ),
  smoothingBuffer_( NULL ),
  grayOutputBuffer_( NULL ),
  bufferUtility_( new blobBuffers::BufferUtility() ),
  preBackgroundSmoothingFilter_( new blobFilters::SmoothingFilter() ),
  maskFilter_( new blobFilters::MaskFilter() ),
  backgroundFilter_( new blobFilters::BackgroundFilter() ),
  signalNormalizationFilter_( new blobFilters::SignalNormalizationFilter() ),
  highpassFilter_( new blobFilters::HighpassFilter() ),
  highpassTwoFilter_( new blobFilters::HighpassFilter() ),
  smoothingFilter_( new blobFilters::SmoothingFilter() ),
  thresholdFilter_( new blobFilters::ThresholdFilter() ),
  processTimer_( new blobUtils::FunctionTimer() ),
  copyCameraPixelsTimer_( new blobUtils::FunctionTimer() ),
  filterStats_( new blobStats::FilterStats() )
{
    thresholdFilter_->setActive( true );
}

ImageFilters::~ImageFilters()
{
    delete filterStats_;
    clearBuffers();
    deleteTimers();
    deleteFilters();
}

void ImageFilters::clearBuffers()
{
    bufferWidth_ = 0;
    bufferHeight_ = 0;
    clearFilterBuffers();
}

void ImageFilters::clearFilterBuffers()
{
    delete inputBuffer_;
    delete preBackgroundSmoothingBuffer_;
    delete backgroundBufferCopy_;
    delete diffBuffer_;
    delete normalizeBuffer_;
    delete highpassBuffer_;
    delete highpassTwoBuffer_;
    delete smoothingBuffer_;
    delete grayOutputBuffer_;
    inputBuffer_ = NULL;
    preBackgroundSmoothingBuffer_ = NULL;
    backgroundBufferCopy_ = NULL;
    diffBuffer_ = NULL;
    normalizeBuffer_ = NULL;
    highpassBuffer_ = NULL;
    highpassTwoBuffer_ = NULL;
    smoothingBuffer_ = NULL;
    grayOutputBuffer_ = NULL;
}

void ImageFilters::deleteTimers()
{
    delete processTimer_;
    delete copyCameraPixelsTimer_;
}

void ImageFilters::deleteFilters()
{
    delete bufferUtility_;
    delete preBackgroundSmoothingFilter_;
    delete maskFilter_;
    delete backgroundFilter_;
    delete signalNormalizationFilter_;
    delete highpassFilter_;
    delete highpassTwoFilter_;
    delete smoothingFilter_;
    delete thresholdFilter_;
}

void ImageFilters::processGrayscaleImage( unsigned char * grayPixels, int width, int height )
{
    processTimer_->startTiming();
    processImage( grayPixels, width, height, blobBuffers::BufferUtility::GRAY_CHANNEL );
    processTimer_->stopTiming();
}

void ImageFilters::processRgbImage( unsigned char * colorPixels, int width, int height )
{
    processTimer_->startTiming();
    processImage( colorPixels, width, height, blobBuffers::BufferUtility::RGB_CHANNELS );
    processTimer_->stopTiming();
}

void ImageFilters::processImage( unsigned char * pixels, 
                                 int width, 
                                 int height, 
                                 int channels )
{
    if( width > 0 && height > 0 ) {
        if( bufferWidth_ != width || bufferHeight_ != height ) {
            initializeBuffers( width, height );
        }
        copyPixelsToInputBuffer( pixels, width, height, channels );
        applyPreBackgroundSmoothingFilter();
        applyMaskFilter();
        applyBackgroundFilter();
        applySignalNormalizationFilter();
        applyHighpassFilter();
        applyHighpassTwoFilter();
        applySmoothingFilter();
        applyThresholdFilter();
    }
    else {
        clearBuffers();
    }
}

void ImageFilters::copyPixelsToInputBuffer( unsigned char * grayPixels, 
                                            int width, 
                                            int height, 
                                            int channels )
{
    copyCameraPixelsTimer_->startTiming();
    bufferUtility_->process( grayPixels, width, height, channels, inputBuffer_ );
    copyCameraPixelsTimer_->stopTiming();
}

void ImageFilters::applyPreBackgroundSmoothingFilter()
{
    preBackgroundSmoothingFilter_->processImage( inputBuffer_, preBackgroundSmoothingBuffer_ );
}

void ImageFilters::applyMaskFilter()
{
    maskFilter_->processImage( preBackgroundSmoothingBuffer_, preBackgroundSmoothingBuffer_ );
}

void ImageFilters::applyBackgroundFilter()
{
    backgroundFilter_->processImage( preBackgroundSmoothingBuffer_, diffBuffer_ );
    backgroundBufferCopy_->copyPixels( backgroundFilter_->backgroundImage() );
}

void ImageFilters::applySignalNormalizationFilter()
{
    signalNormalizationFilter_->processImage( diffBuffer_, normalizeBuffer_ );
}

void ImageFilters::applyHighpassFilter()
{
    highpassFilter_->processImage( normalizeBuffer_, highpassBuffer_ );
}

void ImageFilters::applyHighpassTwoFilter()
{
    highpassTwoFilter_->processImage( highpassBuffer_, highpassTwoBuffer_ );

    // Copy blurred (subtraction) image for display.
    if( highpassFilter_->showSubtractionImage() ) {
        highpassBuffer_->copyPixels( highpassFilter_->blurredImage() );
    }
}

void ImageFilters::applySmoothingFilter()
{
    smoothingFilter_->processImage( highpassTwoBuffer_, smoothingBuffer_ );

    // Copy blurred (subtraction) image for display.
    if( highpassTwoFilter_->showSubtractionImage() ) {
        highpassTwoBuffer_->copyPixels( highpassTwoFilter_->blurredImage() );
    }
}

void ImageFilters::applyThresholdFilter()
{
    thresholdFilter_->processImage( smoothingBuffer_, grayOutputBuffer_ );
}

blobStats::FilterStats * ImageFilters::calculateStats()
{
    filterStats_->setAvgProcessTime( processTimer_->calculateAverageTime() );
    filterStats_->setAvgCopyCameraPixelsTime( copyCameraPixelsTimer_->calculateAverageTime() );
    filterStats_->setAvgPreBkSmoothTime( preBackgroundSmoothingFilter_->calculateAverageTime()
                                         + maskFilter_->calculateAverageTime() );
    filterStats_->setAvgBackgroundTime( backgroundFilter_->calculateAverageTime() );
    filterStats_->setAvgNormalizeTime( signalNormalizationFilter_->calculateAverageTime() );
    filterStats_->setAvgHighpassTime( highpassFilter_->calculateAverageTime() );
    filterStats_->setAvgHighpassTwoTime( highpassTwoFilter_->calculateAverageTime() );
    filterStats_->setAvgSmoothTime( smoothingFilter_->calculateAverageTime() );
    filterStats_->setAvgThresholdTime( thresholdFilter_->calculateAverageTime() );
    return filterStats_;
}

void ImageFilters::initializeBuffers( int width, int height )
{
    clearBuffers();

    if( width > 0 && height > 0 ) {
        bufferWidth_ = width;
        bufferHeight_ = height;
        initializeFilterBuffers();
    }
}

void ImageFilters::initializeFilterBuffers()
{
    inputBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    preBackgroundSmoothingBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    backgroundBufferCopy_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    diffBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    normalizeBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    highpassBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    highpassTwoBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    smoothingBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
    grayOutputBuffer_ = new blobBuffers::GrayBuffer( bufferWidth_, bufferHeight_ );
}

void ImageFilters::saveNextFrameAsBackground()
{
    backgroundFilter_->saveNextFrameAsBackground();
}

void ImageFilters::saveFramesAsBackground( int count )
{
    backgroundFilter_->saveFramesAsBackground( count );
}

void ImageFilters::setAutoBackgroundActive( bool b )
{
    backgroundFilter_->setAutoBackgroundActive( b );
}

bool ImageFilters::isAutoBackgroundActive()
{
    return backgroundFilter_->isAutoBackgroundActive();
}

void ImageFilters::setBackgroundLearnRate( float rate )
{
    backgroundFilter_->setBackgroundLearnRate( rate );
}

void ImageFilters::useDarkBlobs( bool b )
{
    backgroundFilter_->useDarkBlobs( b );
}

bool ImageFilters::useDarkBlobs()
{
    return backgroundFilter_->useDarkBlobs();
}

void ImageFilters::setMaskCalibrationGridPoints( blobNormalize::CalibrationGridPoints * calGridPoints )
{
    maskFilter_->setCalibrationGridPoints( calGridPoints );
}

void ImageFilters::setSignalFishEyeGrid( blobNormalize::FishEyeGrid * fishEyeGrid )
{
    signalNormalizationFilter_->setFishEyeGrid( fishEyeGrid );
}

void ImageFilters::usePeriodicBackground( bool b )
{
    backgroundFilter_->setPeriodicBackgroundActive( b );
}

bool ImageFilters::isPeriodicBackgroundActive()
{
    return backgroundFilter_->isPeriodicBackgroundActive();
}

void ImageFilters::setPeriodicBackgroundSeconds( float t )
{
    backgroundFilter_->setPeriodicBackgroundSeconds( t );
}

float ImageFilters::getPeriodicBackgroundSeconds()
{
    return backgroundFilter_->getPeriodicBackgroundSeconds();
}

void ImageFilters::setPeriodicBackgroundPercent( float percent )
{
    backgroundFilter_->setPeriodicBackgroundPercent( percent );
}

float ImageFilters::getPeriodicBackgroundPercent()
{
    return backgroundFilter_->getPeriodicBackgroundPercent();
}

void ImageFilters::checkPeriodicBackground()
{
    backgroundFilter_->checkPeriodicBackground();
}

void ImageFilters::flipImageHorizontal( bool b )
{
    bufferUtility_->setFlipHorizontal( b );
}

bool ImageFilters::flipImageHorizontal()
{
    return bufferUtility_->flipHorizontal();
}

void ImageFilters::flipImageVertical( bool b )
{
    bufferUtility_->setFlipVertical( b );
}

bool ImageFilters::flipImageVertical()
{
    return bufferUtility_->flipVertical();
}

void ImageFilters::setPreBackgroundSmoothingActive( bool active )
{
    preBackgroundSmoothingFilter_->setActive( active );
    saveNextFrameAsBackground();
}

bool ImageFilters::isPreBackgroundSmoothingActive()
{
    return preBackgroundSmoothingFilter_->isActive();
}

void ImageFilters::setPreBackgroundSmoothingBlurSize( int blurSize )
{
    preBackgroundSmoothingFilter_->setBlurSize( blurSize );
    saveNextFrameAsBackground();
}

int ImageFilters::preBackgroundSmoothingBlurSize()
{
    return preBackgroundSmoothingFilter_->blurSize();
}

void ImageFilters::useGaussianPreBackgroundSmoothing( bool b )
{
    preBackgroundSmoothingFilter_->useGaussianBlur( b );
    saveNextFrameAsBackground();
}

void ImageFilters::setGaussianPreBackgroundSmoothingSigma( double sigma )
{
    preBackgroundSmoothingFilter_->setGaussianBlurSigma( sigma );
    saveNextFrameAsBackground();
}

bool ImageFilters::isGaussianPreBackgroundSmoothingActive()
{
    return preBackgroundSmoothingFilter_->useGaussianBlur();
}

double ImageFilters::gaussianPreBackgroundSmoothingSigma()
{
    return preBackgroundSmoothingFilter_->gaussianBlurSigma();
}

void ImageFilters::setMaskActive( bool active )
{
    maskFilter_->setActive( active );
    saveNextFrameAsBackground();
}

bool ImageFilters::isMaskActive()
{
    return maskFilter_->isActive();
}

void ImageFilters::setMaskWidthPadding( int pixels )
{
    maskFilter_->setWidthPadding( pixels );
    saveNextFrameAsBackground();
}

int ImageFilters::maskWidthPadding()
{
    return maskFilter_->widthPadding();
}

void ImageFilters::setMaskHeightPadding( int pixels )
{
    maskFilter_->setHeightPadding( pixels );
    saveNextFrameAsBackground();
}

int ImageFilters::maskHeightPadding()
{
    return maskFilter_->heightPadding();
}

void ImageFilters::setSignalNormalizationActive( bool b )
{
    signalNormalizationFilter_->setActive( b );
}

bool ImageFilters::isSignalNormalizationActive()
{
    return signalNormalizationFilter_->isActive();
}

void ImageFilters::setHighpassActive( bool active )
{
    highpassFilter_->setActive( active );
}

bool ImageFilters::isHighpassActive()
{
    return highpassFilter_->isActive();
}

void ImageFilters::setHighpassBlurSize( int blurSize )
{
    highpassFilter_->setBlurSize( blurSize );
}

int ImageFilters::highpassBlurSize()
{
    return highpassFilter_->blurSize();
}

void ImageFilters::showHighpassSubtractionImage( bool show )
{
    highpassFilter_->showSubtractionImage( show );
}

void ImageFilters::setHighpassAmplifyActive( bool active )
{
    highpassFilter_->setAmplifyActive( active );
}

bool ImageFilters::isHighpassAmplifyActive()
{
    return highpassFilter_->isAmplifyActive();
}

void ImageFilters::setHighpassAmplifyLevel( int level )
{
    highpassFilter_->setAmplifyLevel( level );
}

int ImageFilters::highpassAmplifyLevel()
{
    return highpassFilter_->amplifyLevel();
}

void ImageFilters::setHighpassTwoActive( bool active )
{
    highpassTwoFilter_->setActive( active );
}

bool ImageFilters::isHighpassTwoActive()
{
    return highpassTwoFilter_->isActive();
}

void ImageFilters::setHighpassTwoKernel( int size )
{
    highpassTwoFilter_->setBlurSize( size );
}

int ImageFilters::highpassTwoKernel()
{
    return highpassTwoFilter_->blurSize();
}

void ImageFilters::showHighpassTwoSubtractionImage( bool show )
{
    highpassTwoFilter_->showSubtractionImage( show );
}

void ImageFilters::setHighpassTwoAmplifyActive( bool active )
{
    highpassTwoFilter_->setAmplifyActive( active );
}

bool ImageFilters::isHighpassTwoAmplifyActive()
{
    return highpassTwoFilter_->isAmplifyActive();
}

void ImageFilters::setHighpassTwoAmplifyLevel( int level )
{
    highpassTwoFilter_->setAmplifyLevel( level );
}

int ImageFilters::highpassTwoAmplifyLevel()
{
    return highpassTwoFilter_->amplifyLevel();
}

void ImageFilters::setSmoothingActive( bool active )
{
    smoothingFilter_->setActive( active );
}

bool ImageFilters::isSmoothingActive()
{
    return smoothingFilter_->isActive();
}

void ImageFilters::setSmoothingBlurSize( int blurSize )
{
    smoothingFilter_->setBlurSize( blurSize );
}

int ImageFilters::smoothingBlurSize()
{
    return smoothingFilter_->blurSize();
}

void ImageFilters::useGaussianSmoothing( bool b )
{
    smoothingFilter_->useGaussianBlur( b );
}

void ImageFilters::setGaussianSmoothingSigma( double sigma )
{
    smoothingFilter_->setGaussianBlurSigma( sigma );
}

bool ImageFilters::isGaussianSmoothingActive()
{
    return smoothingFilter_->useGaussianBlur();
}

double ImageFilters::gaussianSmoothingSigma()
{
    return smoothingFilter_->gaussianBlurSigma();
}

void ImageFilters::setSmoothingAmplifyActive( bool active )
{
    smoothingFilter_->setAmplifyActive( active );
}

bool ImageFilters::isSmoothingAmplifyActive()
{
    return smoothingFilter_->isAmplifyActive();
}

void ImageFilters::setSmoothingAmplifyLevel( int level )
{
    smoothingFilter_->setAmplifyLevel( level );
}

int ImageFilters::smoothingAmplifyLevel()
{
    return smoothingFilter_->amplifyLevel();
}

void ImageFilters::setImageThreshold( int threshold )
{
    thresholdFilter_->setThreshold( threshold );
}

int ImageFilters::imageThreshold()
{
    return thresholdFilter_->threshold();
}

blobBuffers::GrayBuffer * ImageFilters::grayBuffer( blobEnums::FilterEnum::Enum filterType )
{
    switch( filterType ) {
        case FilterEnum::SOURCE:            return inputBuffer_;
        case FilterEnum::PRE_BKGRD_SMOOTH:  return preBackgroundSmoothingBuffer_;
        case FilterEnum::BACKGROUND:        return backgroundBufferCopy_;
        case FilterEnum::DIFF_BUFFER:       return diffBuffer_;
        case FilterEnum::HIGHPASS:          return highpassBuffer_;
        case FilterEnum::HIGHPASS_TWO:      return highpassTwoBuffer_;
        case FilterEnum::SMOOTH:            return smoothingBuffer_;
        case FilterEnum::TRACKED:           return grayOutputBuffer_;
    }
    return NULL;
}

blobBuffers::GrayBuffer * ImageFilters::graySourceBuffer()
{
    return inputBuffer_;
}

blobBuffers::GrayBuffer * ImageFilters::grayOutputBuffer()
{
    return grayOutputBuffer_;
}

blobBuffers::GrayBuffer * ImageFilters::differenceBuffer()
{
    return diffBuffer_;
}

blobBuffers::GrayBuffer * ImageFilters::smoothedSourceBuffer()
{
    return preBackgroundSmoothingBuffer_;
}

blobBuffers::GrayBuffer * ImageFilters::backgroundBuffer()
{
    return backgroundBufferCopy_;
}
