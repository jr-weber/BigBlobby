////////////////////////////////////////////////////////////////////////////////
// BlobParamsValidator
//
// Purpose: Performs data validation on program settings read from an XML file.
//
// Created: 5/18/2011
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2011, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#include "blobIO/BlobParamsValidator.h"
#include "blobExceptions/BlobParamsException.h"
#include <QString>

using blobIO::BlobParamsValidator;
using blobExceptions::BlobParamsException;

const std::string BlobParamsValidator::CLASS_NAME = "blobGui::BlobParamsValidator::";

/***************************************************************************//**
This constructor calls on the init() function to set default values for all
data members.
*******************************************************************************/
BlobParamsValidator::BlobParamsValidator()
{
    init();
}

/***************************************************************************//**
This constructor calls on the init() function to set default values for all
data members, and then stores the XML filename given as an argument.
*******************************************************************************/
BlobParamsValidator::BlobParamsValidator( const std::string & filename )
{
    init();

    // XML filename needs to be set AFTER init() is called.
    setBlobParamsXmlFilename( filename );
}

BlobParamsValidator::~BlobParamsValidator()
{
}

/***************************************************************************//**
Sets all data members to hard-coded default values.

When BigBlobby starts up, the values that will actually be used to set up the 
GUI display and the underlying CoreVision object are expected to be read from a
default XML file, &quot;data/settings/DefaultBlobParams.xml&quot;.  However,
if the file is missing, corrupted, or has been hand-edited and contains an 
error, the &quot;factory default&quot; values defined in init() should be 
used instead (after warning the user of the error).
<br />

The values hard-coded in this function were chosen because they work well with
the PS3Eye camera used with the Playsurface (http://playsurface.org) that
BigBlobby was written for.  Depending on lighting conditions and variables 
such as fingertip size, some of these settings will likely need to be tweaked 
for optimal results.
*******************************************************************************/
void BlobParamsValidator::init()
{
    blobParamsXmlFilename_ = "data/settings/DefaultBlobParams.xml";

    // Camera
    deviceID_ = 0;
    videoRate_ = 60;
    videoWidth_ = 320;
    videoHeight_ = 240;
    flipImageVertically_ = false;
    flipImageHorizontally_ = true;

    // Blobs
    imageThreshold_ = 19;
    minBlobSize_ = 40;
    maxBlobSize_ = 200;
    maxNumberBlobs_ = 20;
    useDarkBlobs_ = false;

    showCalibrationGrid_ = true;
    showBlobAreas_ = true;
    showBlobOutlines_ = true;
    showBlobCrosshairs_ = true;
    showBlobBoundingBoxes_ = false;

    normalizeBlobIntensities_ = true;
    showBlobLabels_ = true;

    // Background
    useAutoBackground_ = false;
    backgroundLearnRate_ = 200;
    usePeriodicBackground_ = true;
    periodicBackgroundSeconds_ = 4;
    periodicBackgroundPercent_ = 10;

    // Filters
    hideGaussianControls_ = true;
    hideEarlySmoothing_ = true;
    usePreBkSmoothing_ = true;
    preBkSmoothingBlur_ = 1;
    useGaussianPreBkSmoothing_ = true;
    gaussianPreBkSigma_ = 2.0;

    useMask_ = false;
    maskWidthPadding_ = 5;
    maskHeightPadding_ = 5;
    useHighpass_ = true;
    highpassBlur_ = 7;
    useHighpassAmplify_ = true;
    highpassAmplifyLevel_ = 60;
    useHighpassTwo_ = true;
    highpassTwoKernel_ = 7;
    useHighpassTwoAmplify_ = true;
    highpassTwoAmplifyLevel_ = 10;
    useSmoothing_ = true;
    smoothingBlur_ = 4;
    useGaussianSmoothing_ = true;
    gaussianSigma_ = 2.0;
    useSmoothingAmplify_ = true;
    smoothingAmplifyLevel_ = 10;

    // Network
    showNetworkMenu_ = true;
    useNetworkSilentMode_ = false;
    useTuioUdpChannelOne_ = true;
    useTuioUdpChannelTwo_ = true;
    useFlashXmlChannel_ = true;
    useBinaryTcpChannel_ = false;
    localHost_ = "127.0.0.1";
    tuioUdpChannelOnePort_ = 3333;
    tuioUdpChannelTwoPort_ = 3334;
    tuioUdpProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_CURSOR;
    flashXmlChannelPort_ = 3000;
    flashXmlProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_CURSOR;
    binaryTcpChannelPort_ = 3001;
    simpleMessageServerPort_ = 3002;

    // Experimental
    showExperimentalMenu_ = false;
}

/***************************************************************************//**
The assignment operator copies over all parameters values from the validator
object on the right-hand side of the equal operator.
*******************************************************************************/
BlobParamsValidator & BlobParamsValidator::operator=( const BlobParamsValidator & rhs )
{
    if( this != &rhs ) {
        blobParamsXmlFilename_ = rhs.blobParamsXmlFilename_;

        // Camera
        deviceID_ = rhs.deviceID_;
        videoRate_ = rhs.videoRate_;
        videoWidth_ = rhs.videoWidth_;
        videoHeight_ = rhs.videoHeight_;
        flipImageVertically_ = rhs.flipImageVertically_;
        flipImageHorizontally_ = rhs.flipImageHorizontally_;

        // Blobs
        imageThreshold_ = rhs.imageThreshold_;
        minBlobSize_ = rhs.minBlobSize_;
        maxBlobSize_ = rhs.maxBlobSize_;
        maxNumberBlobs_ = rhs.maxNumberBlobs_;
        useDarkBlobs_ = rhs.useDarkBlobs_;

        showCalibrationGrid_ = rhs.showCalibrationGrid_;
        showBlobAreas_ = rhs.showBlobAreas_;
        showBlobOutlines_ = rhs.showBlobOutlines_;
        showBlobCrosshairs_ = rhs.showBlobCrosshairs_;
        showBlobBoundingBoxes_ = rhs.showBlobBoundingBoxes_;

        normalizeBlobIntensities_ = rhs.normalizeBlobIntensities_;
        showBlobLabels_ = rhs.showBlobLabels_;

        // Background
        useAutoBackground_ = rhs.useAutoBackground_;
        backgroundLearnRate_ = rhs.backgroundLearnRate_;
        usePeriodicBackground_ = rhs.usePeriodicBackground_;
        periodicBackgroundSeconds_ = rhs.periodicBackgroundSeconds_;
        periodicBackgroundPercent_ = rhs.periodicBackgroundPercent_;

        // Filters
        hideGaussianControls_ = rhs.hideGaussianControls_;
        hideEarlySmoothing_ = rhs.hideEarlySmoothing_;
        usePreBkSmoothing_ = rhs.usePreBkSmoothing_;
        preBkSmoothingBlur_ = rhs.preBkSmoothingBlur_;
        useGaussianPreBkSmoothing_ = rhs.useGaussianPreBkSmoothing_;
        gaussianPreBkSigma_ = rhs.gaussianPreBkSigma_;

        useMask_ = rhs.useMask_;
        maskWidthPadding_ = rhs.maskWidthPadding_;
        maskHeightPadding_ = rhs.maskHeightPadding_;
        useHighpass_ = rhs.useHighpass_;
        highpassBlur_ = rhs.highpassBlur_;
        useHighpassAmplify_ = rhs.useHighpassAmplify_;
        highpassAmplifyLevel_ = rhs.highpassAmplifyLevel_;
        useHighpassTwo_ = rhs.useHighpassTwo_;
        highpassTwoKernel_ = rhs.highpassTwoKernel_;
        useHighpassTwoAmplify_ = rhs.useHighpassTwoAmplify_;
        highpassTwoAmplifyLevel_ = rhs.highpassTwoAmplifyLevel_;
        useSmoothing_ = rhs.useSmoothing_;
        smoothingBlur_ = rhs.smoothingBlur_;
        useGaussianSmoothing_ = rhs.useGaussianSmoothing_;
        gaussianSigma_ = rhs.gaussianSigma_;
        useSmoothingAmplify_ = rhs.useSmoothingAmplify_;
        smoothingAmplifyLevel_ = rhs.smoothingAmplifyLevel_;

        // Network
        showNetworkMenu_ = rhs.showNetworkMenu_;
        useNetworkSilentMode_ = rhs.useNetworkSilentMode_;
        useTuioUdpChannelOne_ = rhs.useTuioUdpChannelOne_;
        useTuioUdpChannelTwo_ = rhs.useTuioUdpChannelTwo_;
        useFlashXmlChannel_ = rhs.useFlashXmlChannel_;
        useBinaryTcpChannel_ = rhs.useBinaryTcpChannel_;
        localHost_ = rhs.localHost_;
        tuioUdpChannelOnePort_ = rhs.tuioUdpChannelOnePort_;
        tuioUdpChannelTwoPort_ = rhs.tuioUdpChannelTwoPort_;
        tuioUdpProfile_ = rhs.tuioUdpProfile_;
        flashXmlChannelPort_ = rhs.flashXmlChannelPort_;
        flashXmlProfile_ = rhs.flashXmlProfile_;
        binaryTcpChannelPort_ = rhs.binaryTcpChannelPort_;
        simpleMessageServerPort_ = rhs.simpleMessageServerPort_;

        // Experimental
        showExperimentalMenu_ = rhs.showExperimentalMenu_;
    }
    return *this;
}

/***************************************************************************//**
The XML filename should be a relative path starting with &quot;data&quot; and
ending with an extension of &quot;.xml&quot;.
*******************************************************************************/
void BlobParamsValidator::setBlobParamsXmlFilename( const std::string & xmlFilename )
{
    blobParamsXmlFilename_ = xmlFilename;
}

/***************************************************************************//**
The XML filename should be a relative path starting with &quot;data&quot; and
ending with an extension of &quot;.xml&quot;.
*******************************************************************************/
std::string BlobParamsValidator::getBlobParamsXmlFilename()
{
    return blobParamsXmlFilename_;
}

/***************************************************************************//**
The device (camera) ID is expected to be an integer of value zero or greater.
*******************************************************************************/
void BlobParamsValidator::setDeviceID( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException( "deviceID",
                                   s.toStdString(),
                                   "zero or a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setDeviceID()" );
    }
    deviceID_ = n;
}

/***************************************************************************//**
The requested video rate must be an integer greater than zero, and is typically
30 or 60 frames per second.
*******************************************************************************/
void BlobParamsValidator::setVideoRate( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "videoRate",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setVideoRate()" );
    }
    videoRate_ = n;
}

/***************************************************************************//**
The video width must be a positive integer, and usually will be either 320 or
640 pixels.
*******************************************************************************/
void BlobParamsValidator::setVideoWidth( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "videoWidth",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setVideoWidth()" );
    }
    videoWidth_ = n;
}

/***************************************************************************//**
The video height must be a positive integer, and usually will be either 240 or
480 pixels.
*******************************************************************************/
void BlobParamsValidator::setVideoHeight( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException(  "videoHeight",
                                    s.toStdString(),
                                    "a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setVideoHeight()" );
    }
    videoHeight_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::flipImageVertically( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        flipImageVertically_ = true;
    }
    else if( b == "false" ) {
        flipImageVertically_ = false;
    }
    else {
        throw BlobParamsException( "flipImageVertically",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "flipImageVertically()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::flipImageHorizontally( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        flipImageHorizontally_ = true;
    }
    else if( b == "false" ) {
        flipImageHorizontally_ = false;
    }
    else {
        throw BlobParamsException( "flipImageHorizontally",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "flipImageHorizontally()" );
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Blobs Setters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setImageThreshold( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException(  "imageThreshold",
                                    s.toStdString(),
                                    "a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setImageThreshold()" );
    }
    imageThreshold_ = n;
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setMinBlobSize( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "minBlobSize",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setMinBlobSize()" );
    }
    minBlobSize_ = n;
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setMaxBlobSize( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException( "maxBlobSize",
                                   s.toStdString(),
                                   "zero or a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setMaxBlobSize()" );
    }
    maxBlobSize_ = n;
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater (20 is a 
reasonable number, but 50 or 60 should still work).
*******************************************************************************/
void BlobParamsValidator::setMaxNumberBlobs( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "maxNumberBlobs",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setMaxNumberBlobs()" );
    }
    maxNumberBlobs_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useDarkBlobs( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useDarkBlobs_ = true;
    }
    else if( b == "false" ) {
        useDarkBlobs_ = false;
    }
    else {
        throw BlobParamsException( "useDarkBlobs",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useDarkBlobs()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showCalibrationGrid( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showCalibrationGrid_ = true;
    }
    else if( b == "false" ) {
        showCalibrationGrid_ = false;
    }
    else {
        throw BlobParamsException( "showCalibrationGrid",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showCalibrationGrid()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showBlobAreas( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showBlobAreas_ = true;
    }
    else if( b == "false" ) {
        showBlobAreas_ = false;
    }
    else {
        throw BlobParamsException( "showBlobAreas",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showBlobAreas()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showBlobOutlines( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showBlobOutlines_ = true;
    }
    else if( b == "false" ) {
        showBlobOutlines_ = false;
    }
    else {
        throw BlobParamsException( "showBlobOutlines",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showBlobOutlines()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showBlobCrosshairs( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showBlobCrosshairs_ = true;
    }
    else if( b == "false" ) {
        showBlobCrosshairs_ = false;
    }
    else {
        throw BlobParamsException( "showBlobCrosshairs",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showBlobCrosshairs()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showBlobBoundingBoxes( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showBlobBoundingBoxes_ = true;
    }
    else if( b == "false" ) {
        showBlobBoundingBoxes_ = false;
    }
    else {
        throw BlobParamsException( "showBlobBoundingBoxes",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showBlobBoundingBoxes()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showBlobLabels( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showBlobLabels_ = true;
    }
    else if( b == "false" ) {
        showBlobLabels_ = false;
    }
    else {
        throw BlobParamsException( "showBlobLabels",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showBlobLabels()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::normalizeBlobIntensities( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        normalizeBlobIntensities_ = true;
    }
    else if( b == "false" ) {
        normalizeBlobIntensities_ = false;
    }
    else {
        throw BlobParamsException( "normalizeBlobIntensities",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "normalizeBlobIntensities()" );
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                        Dynamic Background Setters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useAutoBackground( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useAutoBackground_ = true;
    }
    else if( b == "false" ) {
        useAutoBackground_ = false;
    }
    else {
        throw BlobParamsException( "useAutoBackground",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useAutoBackground()" );
    }
}

/***************************************************************************//**
The arg must convert to an float with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setBackgroundLearnRate( const QString & s )
{
    bool ok = false;
    float n = s.toFloat( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "backgroundLearnRate",
                                   s.toStdString(),
                                   "zero or a positive number",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setBackgroundLearnRate()" );
    }
    backgroundLearnRate_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::usePeriodicBackground( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        usePeriodicBackground_ = true;
    }
    else if( b == "false" ) {
        usePeriodicBackground_ = false;
    }
    else {
        throw BlobParamsException( "usePeriodicBackground",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "usePeriodicBackground()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setPeriodicBackgroundSeconds( const QString & s )
{
    bool ok = false;
    float n = s.toFloat( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException( "periodicBackgroundSeconds",
                                   s.toStdString(),
                                   "zero or a positive number",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setPeriodicBackgroundSeconds()" );
    }
    periodicBackgroundSeconds_ = n;
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setPeriodicBackgroundPercent( const QString & s )
{
    bool ok = false;
    float n = s.toFloat( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException( "periodicBackgroundPercent",
                                   s.toStdString(),
                                   "zero or a positive number",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setPeriodicBackgroundPercent()" );
    }
    periodicBackgroundPercent_ = n;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Filters Setters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::hideGaussianControls( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        hideGaussianControls_ = true;
    }
    else if( b == "false" ) {
        hideGaussianControls_ = false;
    }
    else {
        throw BlobParamsException( "hideGaussianControls",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "hideGaussianControls()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::hideEarlySmoothing( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        hideEarlySmoothing_ = true;
    }
    else if( b == "false" ) {
        hideEarlySmoothing_ = false;
    }
    else {
        throw BlobParamsException( "hideEarlySmoothing",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "hideEarlySmoothing()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::usePreBkSmoothing( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        usePreBkSmoothing_ = true;
    }
    else if( b == "false" ) {
        usePreBkSmoothing_ = false;
    }
    else {
        throw BlobParamsException( "usePreBackgroundSmoothing",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "usePreBkSmoothing()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setPreBkSmoothingBlur( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "preBackgroundSmoothingBlur",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setPreBkSmoothingBlur()" );
    }
    preBkSmoothingBlur_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useGaussianPreBkSmoothing( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useGaussianPreBkSmoothing_ = true;
    }
    else if( b == "false" ) {
        useGaussianPreBkSmoothing_ = false;
    }
    else {
        throw BlobParamsException( "useGaussianPreBackgroundSmoothing",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useGaussianPreBkSmoothing()" );
    }
}

/***************************************************************************//**
The arg must convert to a double with a value greater than 0.
*******************************************************************************/
void BlobParamsValidator::setGaussianPreBkSigma( const QString & s )
{
    bool ok = false;
    float n = s.toDouble( &ok );

    if( !ok || n <= 0 ) {
        throw BlobParamsException( "gaussianPreBackgroundSigma",
                                   s.toStdString(),
                                   "a positive number",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setGaussianPreBkSigma()" );
    }
    gaussianPreBkSigma_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useMask( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useMask_ = true;
    }
    else if( b == "false" ) {
        useMask_ = false;
    }
    else {
        throw BlobParamsException( "useMask",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useMask()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setMaskWidthPadding( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "maskWidthPadding",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setMaskWidthPadding()" );
    }
    maskWidthPadding_ = n;
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setMaskHeightPadding( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "maskHeightPadding",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setMaskHeightPadding()" );
    }
    maskHeightPadding_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useHighpass( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useHighpass_ = true;
    }
    else if( b == "false" ) {
        useHighpass_ = false;
    }
    else {
        throw BlobParamsException( "useHighpass",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useHighpass()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setHighpassBlur( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException( "highpassBlur",
                                   s.toStdString(),
                                   "zero or a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setHighpassBlur()" );
    }
    highpassBlur_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useHighpassAmplify( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useHighpassAmplify_ = true;
    }
    else if( b == "false" ) {
        useHighpassAmplify_ = false;
    }
    else {
        throw BlobParamsException( "useHighpassAmplify",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useHighpassAmplify()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setHighpassAmplifyLevel( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "highpassAmplify",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setHighpassAmplifyLevel()" );
    }
    highpassAmplifyLevel_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useHighpassTwo( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useHighpassTwo_ = true;
    }
    else if( b == "false" ) {
        useHighpassTwo_ = false;
    }
    else {
        throw BlobParamsException( "useHighpassTwo",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useHighpassTwo()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setHighpassTwoKernel( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "highpassTwoKernel",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setHighpassTwoKernel()" );
    }
    highpassTwoKernel_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useHighpassTwoAmplify( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useHighpassTwoAmplify_ = true;
    }
    else if( b == "false" ) {
        useHighpassTwoAmplify_ = false;
    }
    else {
        throw BlobParamsException( "useHighpassTwoAmplify",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useHighpassTwoAmplify()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setHighpassTwoAmplifyLevel( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "highpassTwoAmplify",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setHighpassTwoAmplifyLevel()" );
    }
    highpassTwoAmplifyLevel_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useSmoothing( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useSmoothing_ = true;
    }
    else if( b == "false" ) {
        useSmoothing_ = false;
    }
    else {
        throw BlobParamsException( "useSmoothing",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useSmoothing()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setSmoothingBlur( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "smoothingBlur",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setSmoothingBlur()" );
    }
    smoothingBlur_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useGaussianSmoothing( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useGaussianSmoothing_ = true;
    }
    else if( b == "false" ) {
        useGaussianSmoothing_ = false;
    }
    else {
        throw BlobParamsException( "useGaussianSmoothing",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useGaussianSmoothing()" );
    }
}

/***************************************************************************//**
The arg must convert to a double with a value greater than 0.
*******************************************************************************/
void BlobParamsValidator::setGaussianSigma( const QString & s )
{
    bool ok = false;
    float n = s.toDouble( &ok );

    if( !ok || n <= 0 ) {
        throw BlobParamsException( "gaussianSigma",
                                   s.toStdString(),
                                   "a positive number",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setGaussianSigma()" );
    }
    gaussianSigma_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useSmoothingAmplify( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useSmoothingAmplify_ = true;
    }
    else if( b == "false" ) {
        useSmoothingAmplify_ = false;
    }
    else {
        throw BlobParamsException( "useSmoothingAmplify",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useSmoothingAmplify()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setSmoothingAmplifyLevel( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 0 ) {
        throw BlobParamsException(  "smoothingAmplify",
                                    s.toStdString(),
                                    "zero or a positive integer",
                                    getBlobParamsXmlFilename(),
                                    CLASS_NAME + "setSmoothingAmplifyLevel()" );
    }
    smoothingAmplifyLevel_ = n;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Network Setters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showNetworkMenu( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showNetworkMenu_ = true;
    }
    else if( b == "false" ) {
        showNetworkMenu_ = false;
    }
    else {
        throw BlobParamsException( "showNetworkMenu",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showNetworkMenu()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useNetworkSilentMode( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useNetworkSilentMode_ = true;
    }
    else if( b == "false" ) {
        useNetworkSilentMode_ = false;
    }
    else {
        throw BlobParamsException( "useNetworkSilentMode",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useNetworkSilentMode()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useTuioUdpChannelOne( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useTuioUdpChannelOne_ = true;
    }
    else if( b == "false" ) {
        useTuioUdpChannelOne_ = false;
    }
    else {
        throw BlobParamsException( "useTuioUdpChannelOne",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useTuioUdpChannelOne()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useTuioUdpChannelTwo( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useTuioUdpChannelTwo_ = true;
    }
    else if( b == "false" ) {
        useTuioUdpChannelTwo_ = false;
    }
    else {
        throw BlobParamsException( "useTuioUdpChannelTwo",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useTuioUdpChannelTwo()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useFlashXmlChannel( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useFlashXmlChannel_ = true;
    }
    else if( b == "false" ) {
        useFlashXmlChannel_ = false;
    }
    else {
        throw BlobParamsException( "useFlashXmlChannel",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useFlashXmlChannel()" );
    }
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::useBinaryTcpChannel( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useBinaryTcpChannel_ = true;
    }
    else if( b == "false" ) {
        useBinaryTcpChannel_ = false;
    }
    else {
        throw BlobParamsException( "useBinaryTcpChannel",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "useBinaryTcpChannel()" );
    }
}

/***************************************************************************//**
This string will usually just be &quot;localhost&quot;.
*******************************************************************************/
void BlobParamsValidator::setLocalHost( const QString & s )
{
    QString hostname( s.trimmed() );

    if( hostname.size() < 1 ) {
        throw BlobParamsException( "localHost",
                                   s.toStdString(),
                                   "the local host name cannot be empty",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setLocalHost()" );
    }
    localHost_ = hostname.toStdString();
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater (3333 is
usually used).
*******************************************************************************/
void BlobParamsValidator::setTuioUdpChannelOnePort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "tuioUdpChannelOnePort",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setTuioUdpChannelOnePort()" );
    }
    tuioUdpChannelOnePort_ = n;
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater (3334 is
usually used).
*******************************************************************************/
void BlobParamsValidator::setTuioUdpChannelTwoPort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "tuioUdpChannelTwoPort",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setTuioUdpChannelTwoPort()" );
    }
    tuioUdpChannelTwoPort_ = n;
}

/***************************************************************************//**
This string should be '/tuio/2Dcur', '/tuio/2Dblb' or '/tuio/2Dcur:/tuio/2Dblb'.
*******************************************************************************/
void BlobParamsValidator::setTuioUdpProfile( const QString & s )
{
    QString profile( s.trimmed().toLower() );

    if( profile == "/tuio/2dcur" ) {
        tuioUdpProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_CURSOR;
    }
    else if( profile == "/tuio/2dblb" ) {
        tuioUdpProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_BLOB;
    }
    else if( profile == "/tuio/2dcur:/tuio/2dblb" ) {
        tuioUdpProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB;
    }
    else {
        throw BlobParamsException( "tuioUdpProfile",
                                   s.toStdString(),
                                   "/tuio/2Dblb, /tuio/2Dcur, or /tuio/2Dcur:/tuio/2Dblb",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setTuioUdpProfile()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater (3000 is
usually used).
*******************************************************************************/
void BlobParamsValidator::setFlashXmlChannelPort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "flashXmlChannelPort",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setFlashXmlChannelPort()" );
    }
    flashXmlChannelPort_ = n;
}

/***************************************************************************//**
This string should be '/tuio/2Dcur', '/tuio/2Dblb' or '/tuio/2Dcur:/tuio/2Dblb'.
*******************************************************************************/
void BlobParamsValidator::setFlashXmlProfile( const QString & s )
{
    QString profile( s.trimmed().toLower() );

    if( profile == "/tuio/2dcur" ) {
        flashXmlProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_CURSOR;
    }
    else if( profile == "/tuio/2dblb" ) {
        flashXmlProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_BLOB;
    }
    else if( profile == "/tuio/2dcur:/tuio/2dblb" ) {
        flashXmlProfile_ = blobEnums::TuioProfileEnum::TUIO_2D_CURSOR_AND_BLOB;
    }
    else {
        throw BlobParamsException( "flashXmlProfile",
                                   s.toStdString(),
                                   "/tuio/2Dblb, /tuio/2Dcur, or /tuio/2Dcur:/tuio/2Dblb",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setFlashXmlProfile()" );
    }
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setBinaryTcpChannelPort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "binaryTcpChannelPort",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setBinaryTcpChannelPort()" );
    }
    binaryTcpChannelPort_ = n;
}

/***************************************************************************//**
The arg must convert to an integer with a value of zero or greater.
*******************************************************************************/
void BlobParamsValidator::setSimpleMessageServerPort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw BlobParamsException( "simpleMessageServerPort",
                                   s.toStdString(),
                                   "a positive integer",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "setSimpleMessageServerPort()" );
    }
    simpleMessageServerPort_ = n;
}

/***************************************************************************//**
Expects &quot;true&quot; or &quot;false&quot; as an argument (case insensitive).
*******************************************************************************/
void BlobParamsValidator::showExperimentalMenu( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        showExperimentalMenu_ = true;
    }
    else if( b == "false" ) {
        showExperimentalMenu_ = false;
    }
    else {
        throw BlobParamsException( "showExperimentalMenu",
                                   s.toStdString(),
                                   "true or false",
                                   getBlobParamsXmlFilename(),
                                   CLASS_NAME + "showExperimentalMenu()" );
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                               Camera Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int BlobParamsValidator::getDeviceID() { return deviceID_; }
int BlobParamsValidator::getVideoRate() { return videoRate_; }
int BlobParamsValidator::getVideoWidth() { return videoWidth_; }
int BlobParamsValidator::getVideoHeight() { return videoHeight_; }
bool BlobParamsValidator::flipImageVertically() { return flipImageVertically_; }
bool BlobParamsValidator::flipImageHorizontally() { return flipImageHorizontally_; }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                Blobs Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int BlobParamsValidator::getImageThreshold() { return imageThreshold_; }
int BlobParamsValidator::getMinBlobSize() { return minBlobSize_; }
int BlobParamsValidator::getMaxBlobSize() { return maxBlobSize_; }
int BlobParamsValidator::getMaxNumberBlobs() { return maxNumberBlobs_; }
bool BlobParamsValidator::useDarkBlobs() { return useDarkBlobs_; }

bool BlobParamsValidator::isShowCalibrationGridOn() { return showCalibrationGrid_; }
bool BlobParamsValidator::isShowBlobAreasOn() { return showBlobAreas_; }
bool BlobParamsValidator::isShowBlobOutlinesOn() { return showBlobOutlines_; }
bool BlobParamsValidator::isShowBlobCrosshairsOn() { return showBlobCrosshairs_; }
bool BlobParamsValidator::isShowBlobBoundingBoxesOn() { return showBlobBoundingBoxes_; }

bool BlobParamsValidator::isNormalizeBlobIntensitiesOn() { return normalizeBlobIntensities_; }
bool BlobParamsValidator::isShowBlobLabelsOn() { return showBlobLabels_; }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Background Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool BlobParamsValidator::useAutoBackground() { return useAutoBackground_; }
float BlobParamsValidator::getBackgroundLearnRate() { return backgroundLearnRate_; }
bool BlobParamsValidator::usePeriodicBackground() { return usePeriodicBackground_; }
float BlobParamsValidator::getPeriodicBackgroundSeconds() { return periodicBackgroundSeconds_; }
float BlobParamsValidator::getPeriodicBackgroundPercent() { return periodicBackgroundPercent_; }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Filters Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool BlobParamsValidator::hideGaussianControls() { return hideGaussianControls_; }
bool BlobParamsValidator::hideEarlySmoothing() { return hideEarlySmoothing_; }

bool BlobParamsValidator::usePreBkSmoothing() { return usePreBkSmoothing_; }
int BlobParamsValidator::getPreBkSmoothingBlur() { return preBkSmoothingBlur_; }
bool BlobParamsValidator::useGaussianPreBkSmoothing() { return useGaussianPreBkSmoothing_; }
double BlobParamsValidator::getGaussianPreBkSigma() { return gaussianPreBkSigma_; }

bool BlobParamsValidator::useMask() { return useMask_; }
int BlobParamsValidator::getMaskWidthPadding() { return maskWidthPadding_; }
int BlobParamsValidator::getMaskHeightPadding() { return maskHeightPadding_; }

bool BlobParamsValidator::useHighpass() { return useHighpass_; }
int BlobParamsValidator::getHighpassBlur() { return highpassBlur_; }
bool BlobParamsValidator::useHighpassAmplify() { return useHighpassAmplify_; }
int BlobParamsValidator::getHighpassAmplifyLevel() { return highpassAmplifyLevel_; }

bool BlobParamsValidator::useHighpassTwo() { return useHighpassTwo_; }
int BlobParamsValidator::getHighpassTwoKernel() { return highpassTwoKernel_; }
bool BlobParamsValidator::useHighpassTwoAmplify() { return useHighpassTwoAmplify_; }
int BlobParamsValidator::getHighpassTwoAmplifyLevel() { return highpassTwoAmplifyLevel_; }

bool BlobParamsValidator::useSmoothing() { return useSmoothing_; }
int BlobParamsValidator::getSmoothingBlur() { return smoothingBlur_; }
bool BlobParamsValidator::useGaussianSmoothing() { return useGaussianSmoothing_; }
double BlobParamsValidator::getGaussianSigma() { return gaussianSigma_; }
bool BlobParamsValidator::useSmoothingAmplify() { return useSmoothingAmplify_; }
int BlobParamsValidator::getSmoothingAmplifyLevel() { return smoothingAmplifyLevel_; }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Network Getters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool BlobParamsValidator::showNetworkMenu() { return showNetworkMenu_; }
bool BlobParamsValidator::useNetworkSilentMode() { return useNetworkSilentMode_; }
bool BlobParamsValidator::useTuioUdpChannelOne() { return useTuioUdpChannelOne_; }
bool BlobParamsValidator::useTuioUdpChannelTwo() { return useTuioUdpChannelTwo_; }
bool BlobParamsValidator::useFlashXmlChannel() { return useFlashXmlChannel_; }
bool BlobParamsValidator::useBinaryTcpChannel() { return useBinaryTcpChannel_; }
std::string BlobParamsValidator::getLocalHost() { return localHost_; }

int BlobParamsValidator::getTuioUdpChannelOnePort() { return tuioUdpChannelOnePort_; }
int BlobParamsValidator::getTuioUdpChannelTwoPort() { return tuioUdpChannelTwoPort_; }
blobEnums::TuioProfileEnum::Enum BlobParamsValidator::getTuioUdpProfile() { return tuioUdpProfile_; }
int BlobParamsValidator::getFlashXmlChannelPort() { return flashXmlChannelPort_; }
blobEnums::TuioProfileEnum::Enum BlobParamsValidator::getFlashXmlProfile() { return flashXmlProfile_; }
int BlobParamsValidator::getBinaryTcpChannelPort() { return binaryTcpChannelPort_; }
int BlobParamsValidator::getSimpleMessageServerPort() { return simpleMessageServerPort_; }

std::string BlobParamsValidator::getTuioUdpProfileAsString()
{
    return blobEnums::TuioProfileEnum::convertToString( tuioUdpProfile_ );
}

std::string BlobParamsValidator::getFlashXmlProfileAsString()
{
    return blobEnums::TuioProfileEnum::convertToString( flashXmlProfile_ );
}

bool BlobParamsValidator::showExperimentalMenu()
{
    return showExperimentalMenu_;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                            Camera Unchecked Setters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BlobParamsValidator::setDeviceID( int id ) { deviceID_ = id; }
void BlobParamsValidator::setVideoRate( int rate ) { videoRate_ = rate; }
void BlobParamsValidator::setVideoWidth( int width ) { videoWidth_ = width; }
void BlobParamsValidator::setVideoHeight( int height ) { videoHeight_ = height; }
void BlobParamsValidator::flipImageVertically( bool b ) { flipImageVertically_ = b; }
void BlobParamsValidator::flipImageHorizontally( bool b ) { flipImageHorizontally_ = b; }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                           Blobs Unchecked Setters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BlobParamsValidator::setImageThreshold( int threshold ) { imageThreshold_ = threshold; }
void BlobParamsValidator::setMinBlobSize( int min ) { minBlobSize_ = min; }
void BlobParamsValidator::setMaxBlobSize( int max ) { maxBlobSize_ = max; }
void BlobParamsValidator::setMaxNumberBlobs( int max ) { maxNumberBlobs_ = max; }
void BlobParamsValidator::useDarkBlobs( bool b ) { useDarkBlobs_ = b; }

void BlobParamsValidator::showCalibrationGrid( bool b ) { showCalibrationGrid_ = b; }
void BlobParamsValidator::showBlobAreas( bool b ) { showBlobAreas_ = b; }
void BlobParamsValidator::showBlobOutlines( bool b ) { showBlobOutlines_ = b; }
void BlobParamsValidator::showBlobCrosshairs( bool b ) { showBlobCrosshairs_ = b; }
void BlobParamsValidator::showBlobBoundingBoxes( bool b ) { showBlobBoundingBoxes_ = b; }

void BlobParamsValidator::normalizeBlobIntensities( bool b ) { normalizeBlobIntensities_ = b; }
void BlobParamsValidator::showBlobLabels( bool b ) { showBlobLabels_ = b; }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                           Background Unchecked Setters
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BlobParamsValidator::useAutoBackground( bool b ) { useAutoBackground_ = b; }
void BlobParamsValidator::setBackgroundLearnRate( float rate ) { backgroundLearnRate_ = rate; }
void BlobParamsValidator::usePeriodicBackground( bool b ) { usePeriodicBackground_ = b; }
void BlobParamsValidator::setPeriodicBackgroundSeconds( float t ) { periodicBackgroundSeconds_ = t; }
void BlobParamsValidator::setPeriodicBackgroundPercent( float t ) { periodicBackgroundPercent_ = t; }

// Filtering unchecked setters
void BlobParamsValidator::hideGaussianControls( bool b ) { hideGaussianControls_ = b; }
void BlobParamsValidator::hideEarlySmoothing( bool b ) { hideEarlySmoothing_ = b; }

void BlobParamsValidator::usePreBkSmoothing( bool b ) { usePreBkSmoothing_ = b; }
void BlobParamsValidator::setPreBkSmoothingBlur( int blur ) { preBkSmoothingBlur_ = blur; }
void BlobParamsValidator::useGaussianPreBkSmoothing( bool b ) { useGaussianPreBkSmoothing_ = b; }
void BlobParamsValidator::setGaussianPreBkSigma( double sigma ) { gaussianPreBkSigma_ = sigma; }

void BlobParamsValidator::useMask( bool b ) { useMask_ = b; }
void BlobParamsValidator::setMaskWidthPadding( int pixels ) { maskWidthPadding_ = pixels; }
void BlobParamsValidator::setMaskHeightPadding( int pixels ) { maskHeightPadding_ = pixels; }

void BlobParamsValidator::useHighpass( bool b ) { useHighpass_ = b; }
void BlobParamsValidator::setHighpassBlur( int blur ) { highpassBlur_ = blur; }
void BlobParamsValidator::useHighpassAmplify( bool b ) { useHighpassAmplify_ = b; }
void BlobParamsValidator::setHighpassAmplifyLevel( int amp ) { highpassAmplifyLevel_ = amp; }

void BlobParamsValidator::useHighpassTwo( bool b ) { useHighpassTwo_ = b; }
void BlobParamsValidator::setHighpassTwoKernel( int kernel ) { highpassTwoKernel_ = kernel; }
void BlobParamsValidator::useHighpassTwoAmplify( bool b ) { useHighpassTwoAmplify_ = b; }
void BlobParamsValidator::setHighpassTwoAmplifyLevel( int amp ) { highpassTwoAmplifyLevel_ = amp; }

void BlobParamsValidator::useSmoothing( bool b ) { useSmoothing_ = b; }
void BlobParamsValidator::setSmoothingBlur( int blur ) { smoothingBlur_ = blur; }
void BlobParamsValidator::useGaussianSmoothing( bool b ) { useGaussianSmoothing_ = b; }
void BlobParamsValidator::setGaussianSigma( double sigma ) { gaussianSigma_ = sigma; }
void BlobParamsValidator::useSmoothingAmplify( bool b ) { useSmoothingAmplify_ = b; }
void BlobParamsValidator::setSmoothingAmplifyLevel( int amp ) { smoothingAmplifyLevel_ = amp; }

// Network unchecked setters
void BlobParamsValidator::showNetworkMenu( bool b ) { showNetworkMenu_ = b; }
void BlobParamsValidator::useNetworkSilentMode( bool b ) { useNetworkSilentMode_ = b; }
void BlobParamsValidator::useTuioUdpChannelOne( bool b ) { useTuioUdpChannelOne_ = b; }
void BlobParamsValidator::useTuioUdpChannelTwo( bool b ) { useTuioUdpChannelTwo_ = b; }
void BlobParamsValidator::useFlashXmlChannel( bool b ) { useFlashXmlChannel_ = b; }
void BlobParamsValidator::useBinaryTcpChannel( bool b ) { useBinaryTcpChannel_ = b; }
void BlobParamsValidator::setLocalHost( const std::string & host ) { localHost_ = host; }
void BlobParamsValidator::setTuioUdpChannelOnePort( int port ) { tuioUdpChannelOnePort_ = port; }
void BlobParamsValidator::setTuioUdpChannelTwoPort( int port ) { tuioUdpChannelTwoPort_ = port; }
void BlobParamsValidator::setTuioUdpProfile( blobEnums::TuioProfileEnum::Enum profile ) { tuioUdpProfile_ = profile; }
void BlobParamsValidator::setFlashXmlChannelPort( int port )  { flashXmlChannelPort_ = port; }
void BlobParamsValidator::setFlashXmlProfile( blobEnums::TuioProfileEnum::Enum profile ) { flashXmlProfile_ = profile; }
void BlobParamsValidator::setBinaryTcpChannelPort( int port )  { binaryTcpChannelPort_ = port; }
void BlobParamsValidator::setSimpleMessageServerPort( int port )  { simpleMessageServerPort_ = port; }

// Experimental
void BlobParamsValidator::showExperimentalMenu( bool b ) { showExperimentalMenu_ = b; }
