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
#ifndef BLOBIO_BLOBPARAMSVALIDATOR_H
#define BLOBIO_BLOBPARAMSVALIDATOR_H

#include "blobEnums/TuioProfileEnum.h"
#include <QString>
#include <string>

/***************************************************************************//**
Contains classes for reading/writing program settings as XML files.  

The BlobParamsValidator class that is used by the XML reader for data validation
is also used as a means of passing that information back to whatever code calls 
on the read() function of the XML reader class.
<br />

The same validator object is also used as a convenient way to pass program
settings to the XML file writer class.  For reporting errors seen by the reader,
writer, and validator, the classes in the blobExceptions namespace are used.
<br />

The blobIO namespace also contains a minor utility class called DebugPrint,
which is used throughout the program for writing debugging info to standard out.
*******************************************************************************/
namespace blobIO
{
    /***********************************************************************//**
    Performs data validation on program settings read from an XML file.  

    This class has setter and getter functions for each numeric and boolean 
    value that needs to be read from an XML file at program startup along with
    a few string values such as the name of a test video or the IP address
    for localhost.  
    <br />

    The setter functions that will be used by the reader, BlobParamsXmlReader,
    expect a string argument that in most cases will be converted to a number or
    boolean value.  If the conversion is not possible, a BlobParamsException 
    will be thrown.  Some functions will also throw the exception if the 
    conversion results in a negative number or zero where that does not make 
    sense, or if an empty string is given for something like the filename.
    <br />

    If an exception is thrown because the XML file is missing, corrupted, or 
    has an illegal value, the settings hard-coded in the init() function of 
    this class can be used as a &quot;factory default&quot;.
    <br />

    This validator is also used as a way of passing information to the XML
    writer class, BlobParamsXmlWriter.  For that reason, there is a second
    group of setter functions, most of which expect an int or a boolean value.
    Data validation is not needed for this second group of setter functions.
    ***************************************************************************/
    class BlobParamsValidator
    {

    public:
        static const std::string CLASS_NAME;

        BlobParamsValidator();
        BlobParamsValidator( const std::string & xmlFilename );
        ~BlobParamsValidator();
        void init();

        BlobParamsValidator & operator=( const BlobParamsValidator & rhs );

        void setBlobParamsXmlFilename( const std::string & xmlFilename );
        std::string getBlobParamsXmlFilename();

        // Camera Setters
        void setDeviceID( const QString & s );
        void setVideoRate( const QString & s );
        void setVideoWidth( const QString & s );
        void setVideoHeight( const QString & s );
        void flipImageVertically( const QString & s );
        void flipImageHorizontally( const QString & s );

        // Blobs Setters
        void setImageThreshold( const QString & s );
        void setMinBlobSize( const QString & s );
        void setMaxBlobSize( const QString & s );
        void setMaxNumberBlobs( const QString & s );
        void useDarkBlobs( const QString & s );

        void showCalibrationGrid( const QString & s );
        void showBlobAreas( const QString & s );
        void showBlobOutlines( const QString & s );
        void showBlobCrosshairs( const QString & s );
        void showBlobBoundingBoxes( const QString & s );

        void normalizeBlobIntensities( const QString & s );
        void showBlobLabels( const QString & s );

        // Background Setters
        void useAutoBackground( const QString & s );
        void setBackgroundLearnRate( const QString & s );
        void usePeriodicBackground( const QString & s );
        void setPeriodicBackgroundSeconds( const QString & s );
        void setPeriodicBackgroundPercent( const QString & s );

        // Filtering Setters
        void hideGaussianControls( const QString & s );
        void hideEarlySmoothing( const QString & s );

        void usePreBkSmoothing( const QString & s );
        void setPreBkSmoothingBlur( const QString & s );
        void useGaussianPreBkSmoothing( const QString & s );
        void setGaussianPreBkSigma( const QString & s );

        void useMask( const QString & s );
        void setMaskWidthPadding( const QString & s );
        void setMaskHeightPadding( const QString & s );

        void useHighpass( const QString & s );
        void setHighpassBlur( const QString & s );
        void useHighpassAmplify( const QString & s );
        void setHighpassAmplifyLevel( const QString & s );

        void useHighpassTwo( const QString & s );
        void setHighpassTwoKernel( const QString & s );
        void useHighpassTwoAmplify( const QString & s );
        void setHighpassTwoAmplifyLevel( const QString & s );

        void useSmoothing( const QString & s );
        void setSmoothingBlur( const QString & s );
        void useGaussianSmoothing( const QString & s );
        void setGaussianSigma( const QString & s );
        void useSmoothingAmplify( const QString & s );
        void setSmoothingAmplifyLevel( const QString & s );

        // Network Setters
        void showNetworkMenu( const QString & s );
        void useNetworkSilentMode( const QString & s );
        void useTuioUdpChannelOne( const QString & s );
        void useTuioUdpChannelTwo( const QString & s );
        void useFlashXmlChannel( const QString & s );
        void useBinaryTcpChannel( const QString & s );
        void setLocalHost( const QString & s );
        void setTuioUdpChannelOnePort( const QString & s );
        void setTuioUdpChannelTwoPort( const QString & s );
        void setTuioUdpProfile( const QString & s );
        void setFlashXmlChannelPort( const QString & s );
        void setFlashXmlProfile( const QString & s );
        void setBinaryTcpChannelPort( const QString & s );
        void setSimpleMessageServerPort( const QString & s );

        // Experimental
        void showExperimentalMenu( const QString & s );

        // Camera Getters
        bool useCamera();
        int getDeviceID();
        int getVideoRate();
        int getVideoWidth();
        int getVideoHeight();
        bool flipImageVertically();
        bool flipImageHorizontally();

        // Video Player Getters
        std::string getVideoFileName();

        // Blobs Getters
        int getImageThreshold();
        int getMinBlobSize();
        int getMaxBlobSize();
        int getMaxNumberBlobs();
        bool useDarkBlobs();

        bool isShowCalibrationGridOn();
        bool isShowBlobAreasOn();
        bool isShowBlobOutlinesOn();
        bool isShowBlobCrosshairsOn();
        bool isShowBlobBoundingBoxesOn();

        bool isNormalizeBlobIntensitiesOn();
        bool isShowBlobLabelsOn();

        // Background Getters
        bool useAutoBackground();
        float getBackgroundLearnRate();
        bool usePeriodicBackground();
        float getPeriodicBackgroundSeconds();
        float getPeriodicBackgroundPercent();

        // Filtering Getters
        bool hideGaussianControls();
        bool hideEarlySmoothing();

        bool usePreBkSmoothing();
        int getPreBkSmoothingBlur();
        bool useGaussianPreBkSmoothing();
        double getGaussianPreBkSigma();

        bool useMask();
        int getMaskWidthPadding();
        int getMaskHeightPadding();

        bool useHighpass();
        int getHighpassBlur();
        bool useHighpassAmplify();
        int getHighpassAmplifyLevel();

        bool useHighpassTwo();
        int getHighpassTwoKernel();
        bool useHighpassTwoAmplify();
        int getHighpassTwoAmplifyLevel();

        bool useSmoothing();
        int getSmoothingBlur();
        bool useGaussianSmoothing();
        double getGaussianSigma();
        bool useSmoothingAmplify();
        int getSmoothingAmplifyLevel();

        // Network Getters
        bool showNetworkMenu();
        bool useNetworkSilentMode();
        bool useTuioUdpChannelOne();
        bool useTuioUdpChannelTwo();
        bool useFlashXmlChannel();
        bool useBinaryTcpChannel();
        std::string getLocalHost();
        int getTuioUdpChannelOnePort();
        int getTuioUdpChannelTwoPort();
        blobEnums::TuioProfileEnum::Enum getTuioUdpProfile();
        int getFlashXmlChannelPort();
        blobEnums::TuioProfileEnum::Enum getFlashXmlProfile();
        int getBinaryTcpChannelPort();
        int getSimpleMessageServerPort();
        std::string getTuioUdpProfileAsString();
        std::string getFlashXmlProfileAsString();

        // Experimental
        bool showExperimentalMenu();

        // Camera Unchecked Setters
        void useCamera( bool b );
        void setDeviceID( int id );
        void setVideoRate( int rate );
        void setVideoWidth( int width );
        void setVideoHeight( int height );
        void flipImageVertically( bool b );
        void flipImageHorizontally( bool b );

        // Blobs Unchecked Setters
        void setImageThreshold( int threshold );
        void setMinBlobSize( int min );
        void setMaxBlobSize( int max );
        void setMaxNumberBlobs( int max );
        void useDarkBlobs( bool b );

        void showCalibrationGrid( bool b );
        void showBlobAreas( bool b );
        void showBlobOutlines( bool b );
        void showBlobCrosshairs( bool b );
        void showBlobBoundingBoxes( bool b );

        void normalizeBlobIntensities( bool b );
        void showBlobLabels( bool b );

        // Background Unchecked Setters
        void useAutoBackground( bool b );
        void setBackgroundLearnRate( float rate );
        void usePeriodicBackground( bool b );
        void setPeriodicBackgroundSeconds( float t );
        void setPeriodicBackgroundPercent( float t );

        // Filtering Unchecked Setters
        void hideGaussianControls( bool b );
        void hideEarlySmoothing( bool b );

        void usePreBkSmoothing( bool b );
        void setPreBkSmoothingBlur( int blur );
        void useGaussianPreBkSmoothing( bool b );
        void setGaussianPreBkSigma( double sigma );

        void useMask( bool b );
        void setMaskWidthPadding( int pixels );
        void setMaskHeightPadding( int pixels );

        void useHighpass( bool b );
        void setHighpassBlur( int blur );
        void useHighpassAmplify( bool b );
        void setHighpassAmplifyLevel( int amp );

        void useHighpassTwo( bool b );
        void setHighpassTwoKernel( int kernel );
        void useHighpassTwoAmplify( bool b );
        void setHighpassTwoAmplifyLevel( int amp );

        void useSmoothing( bool b );
        void setSmoothingBlur( int blur );
        void useGaussianSmoothing( bool b );
        void setGaussianSigma( double sigma );
        void useSmoothingAmplify( bool b );
        void setSmoothingAmplifyLevel( int amp );

        // Network Unchecked Setters
        void showNetworkMenu( bool b );
        void useNetworkSilentMode( bool b );
        void useTuioUdpChannelOne( bool b );
        void useTuioUdpChannelTwo( bool b );
        void useFlashXmlChannel( bool b );
        void useBinaryTcpChannel( bool b );
        void setLocalHost( const std::string & host );
        void setTuioUdpChannelOnePort( int port );
        void setTuioUdpChannelTwoPort( int port );
        void setTuioUdpProfile( blobEnums::TuioProfileEnum::Enum profile );
        void setFlashXmlChannelPort( int port );
        void setFlashXmlProfile( blobEnums::TuioProfileEnum::Enum profile );
        void setBinaryTcpChannelPort( int port );
        void setSimpleMessageServerPort( int port );

        // Experimental
        void showExperimentalMenu( bool b );

    private:
        std::string blobParamsXmlFilename_;

        // Camera
        int deviceID_,
            videoRate_,
            videoWidth_,
            videoHeight_;
        bool flipImageVertically_,
             flipImageHorizontally_;

        // Blobs
        int imageThreshold_,
            minBlobSize_,
            maxBlobSize_,
            maxNumberBlobs_;
        bool useDarkBlobs_,
             showCalibrationGrid_,
             showBlobAreas_,
             showBlobOutlines_,
             showBlobCrosshairs_,
             showBlobBoundingBoxes_,
             normalizeBlobIntensities_,
             showBlobLabels_;

        // Background
        bool useAutoBackground_;
        float backgroundLearnRate_;
        bool usePeriodicBackground_;
        float periodicBackgroundSeconds_,
              periodicBackgroundPercent_;

        // Filters
        bool hideGaussianControls_,
             hideEarlySmoothing_;
        bool usePreBkSmoothing_;
        int  preBkSmoothingBlur_;
        bool useGaussianPreBkSmoothing_;
        double gaussianPreBkSigma_;
        bool useMask_;
        int maskWidthPadding_,
            maskHeightPadding_;
        bool useHighpass_;
        int  highpassBlur_;
        bool useHighpassAmplify_;
        int  highpassAmplifyLevel_;
        bool useHighpassTwo_;
        int  highpassTwoKernel_;
        bool useHighpassTwoAmplify_;
        int  highpassTwoAmplifyLevel_;
        bool useSmoothing_;
        int  smoothingBlur_;
        bool useGaussianSmoothing_;
        double gaussianSigma_;
        bool useSmoothingAmplify_;
        int  smoothingAmplifyLevel_;

        // Network
        bool showNetworkMenu_,
             useNetworkSilentMode_,
             useTuioUdpChannelOne_,
             useTuioUdpChannelTwo_,
             useFlashXmlChannel_,
             useBinaryTcpChannel_;
        std::string localHost_;
        blobEnums::TuioProfileEnum::Enum tuioUdpProfile_,
                                         flashXmlProfile_;
        int tuioUdpChannelOnePort_,
            tuioUdpChannelTwoPort_,
            flashXmlChannelPort_,
            binaryTcpChannelPort_,
            simpleMessageServerPort_;

        // Experimental
        bool showExperimentalMenu_;
    };
}

#endif
