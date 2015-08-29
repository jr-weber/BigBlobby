////////////////////////////////////////////////////////////////////////////////
// BlobParamsXmlWriter
//
// Purpose: Reads program settings from a BlobParamsValidator object and writes
//          them to an XML file.
//
// Created:  6/6/2011
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
#include "blobIO/BlobParamsXmlWriter.h"
#include "blobIO/BlobParamsValidator.h"
#include "blobEnums/TuioProfileEnum.h"
#include "blobExceptions/BlobXmlWriterException.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

using blobIO::BlobParamsXmlWriter;
using blobExceptions::BlobXmlWriterException;

BlobParamsXmlWriter::BlobParamsXmlWriter() :
  debug_( false )
{
    
}

BlobParamsXmlWriter::~BlobParamsXmlWriter()
{
}

/***************************************************************************//**
Extracts all values from the validator object and writes them out in XML form
to the XML file specified in the validator object.

ERROR HANDLING:  <br />
Throws a BlobXmlWriterException if something goes wrong while trying to write 
the file.
*******************************************************************************/
void BlobParamsXmlWriter::write( blobIO::BlobParamsValidator * validator )
{
    std::string xmlFile = validator->getBlobParamsXmlFilename();
    QFile file( xmlFile.c_str() );

    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) ) {
        QTextStream stream( &file );
        stream << getSettingsAsXml( validator );
        file.close();
        debug_.printLn( xmlFile + " was written out." );
    }
    else {
        throw BlobXmlWriterException( xmlFile, "BlobParamsXmlWriter::write()" );
    }
}

/***************************************************************************//**
Generates exactly the same XML as the write() function, but prints the info to 
standard out.
*******************************************************************************/
void BlobParamsXmlWriter::printXmlToStandardOut( blobIO::BlobParamsValidator * validator )
{
    std::cout << getSettingsAsXml( validator ).toStdString();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                 Private Helper Functions for write()
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*******************************************************************************
Writes the BlobDetectorParams tag and then calls on helper function for each 
subtag.
*******************************************************************************/
QString BlobParamsXmlWriter::getSettingsAsXml( blobIO::BlobParamsValidator * validator )
{
    QString xml( "<BlobDetectorParams>\n\n" );
    xml.append( getVideoCameraXml( validator ) );
    xml.append( getBlobsXml( validator ) );
    xml.append( getBackgroundXml( validator ) );
    xml.append( getFiltersXml( validator ) );
    xml.append( getNetworkXml( validator ) );
    xml.append( getExperimentalXml( validator ) );
    xml.append( "</BlobDetectorParams>\n" );
    return xml;
}

/*******************************************************************************
Writes all subtags of <Camera>.
*******************************************************************************/
QString BlobParamsXmlWriter::getVideoCameraXml( blobIO::BlobParamsValidator * validator )
{
    QString xml( "    <Camera>\n" );
    //xml.append( createXmlFromBool( "useCamera", validator->useCamera() ) );
    xml.append( createXmlFromInt( "deviceID", validator->getDeviceID() ) );
    xml.append( createXmlFromInt( "videoRate", validator->getVideoRate() ) );
    xml.append( createXmlFromInt( "videoWidth", validator->getVideoWidth() ) );
    xml.append( createXmlFromInt( "videoHeight", validator->getVideoHeight() ) );
    xml.append( createXmlFromBool( "flipImageVertically", validator->flipImageVertically() ) );
    xml.append( createXmlFromBool( "flipImageHorizontally", validator->flipImageHorizontally() ) );
    xml.append( "    </Camera>\n\n" );
    return xml;
}

/*******************************************************************************
Writes all subtags of <Blobs>.
*******************************************************************************/
QString BlobParamsXmlWriter::getBlobsXml( blobIO::BlobParamsValidator * validator )
{
    QString xml( "    <Blobs>\n" );
    xml.append( createXmlFromInt( "imageThreshold", validator->getImageThreshold() ) );
    xml.append( createXmlFromInt( "minBlobSize", validator->getMinBlobSize() ) );
    xml.append( createXmlFromInt( "maxBlobSize", validator->getMaxBlobSize() ) );
    xml.append( createXmlFromInt( "maxNumberBlobs", validator->getMaxNumberBlobs() ) );
    xml.append( createXmlFromBool( "useDarkBlobs", validator->useDarkBlobs() ) );
    xml.append( createXmlFromBool( "showCalibrationGrid", validator->isShowCalibrationGridOn() ) );
    xml.append( createXmlFromBool( "showAreas", validator->isShowBlobAreasOn() ) );
    xml.append( createXmlFromBool( "showOutlines", validator->isShowBlobOutlinesOn() ) );
    xml.append( createXmlFromBool( "showCrosshairs", validator->isShowBlobCrosshairsOn() ) );
    xml.append( createXmlFromBool( "showBoundingBoxes", validator->isShowBlobBoundingBoxesOn() ) );
    xml.append( createXmlFromBool( "normalizeBlobIntensities", validator->isNormalizeBlobIntensitiesOn() ) );
    xml.append( createXmlFromBool( "showLabels", validator->isShowBlobLabelsOn() ) );
    xml.append( "    </Blobs>\n\n" );
    return xml;
}

/*******************************************************************************
Writes all subtags of <Background>.
*******************************************************************************/
QString BlobParamsXmlWriter::getBackgroundXml( blobIO::BlobParamsValidator * validator )
{
    QString xml( "    <Background>\n" );
    xml.append( createXmlFromBool( "useAutoBackground", validator->useAutoBackground() ) );
    xml.append( createXmlFromInt( "backgroundLearnRate", validator->getBackgroundLearnRate() ) );
    xml.append( createXmlFromBool( "usePeriodicBackground", validator->usePeriodicBackground() ) );
    xml.append( createXmlFromInt( "periodicBackgroundSeconds", validator->getPeriodicBackgroundSeconds() ) );
    xml.append( createXmlFromInt( "periodicBackgroundPercent", validator->getPeriodicBackgroundPercent() ) );
    xml.append( "    </Background>\n\n" );
    return xml;
}

/*******************************************************************************
Writes all subtags of <Filters>.
*******************************************************************************/
QString BlobParamsXmlWriter::getFiltersXml( blobIO::BlobParamsValidator * validator )
{
    QString xml( "    <Filters>\n" );
    
    xml.append( createXmlFromBool( "hideGaussianControls", validator->hideGaussianControls() ) );
    xml.append( createXmlFromBool( "hideEarlySmoothing", validator->hideEarlySmoothing() ) );

    xml.append( createXmlFromBool( "usePreBackgroundSmoothing", validator->usePreBkSmoothing() ) );
    xml.append( createXmlFromInt( "preBackgroundSmoothingBlur", validator->getPreBkSmoothingBlur() ) );
    xml.append( createXmlFromBool( "useGaussianPreBackgroundSmoothing", validator->useGaussianPreBkSmoothing() ) );
    xml.append( createXmlFromDouble( "gaussianPreBackgroundSigma", validator->getGaussianPreBkSigma() ) );

    xml.append( createXmlFromBool( "useMask", validator->useMask() ) );
    xml.append( createXmlFromInt( "maskWidthPadding", validator->getMaskWidthPadding() ) );
    xml.append( createXmlFromInt( "maskHeightPadding", validator->getMaskHeightPadding() ) );

    xml.append( createXmlFromBool( "useHighpass", validator->useHighpass() ) );
    xml.append( createXmlFromInt( "highpassBlur", validator->getHighpassBlur() ) );
    xml.append( createXmlFromBool( "useHighpassAmplify", validator->useHighpassAmplify() ) );
    xml.append( createXmlFromInt( "highpassAmplify", validator->getHighpassAmplifyLevel() ) );

    xml.append( createXmlFromBool( "useHighpass2", validator->useHighpassTwo() ) );
    xml.append( createXmlFromInt( "highpassBlur2", validator->getHighpassTwoKernel() ) );
    xml.append( createXmlFromBool( "useHighpassTwoAmplify", validator->useHighpassTwoAmplify() ) );
    xml.append( createXmlFromInt( "highpassTwoAmplify", validator->getHighpassTwoAmplifyLevel() ) );

    xml.append( createXmlFromBool( "useSmoothing", validator->useSmoothing() ) );
    xml.append( createXmlFromInt( "smoothingBlur", validator->getSmoothingBlur() ) );
    xml.append( createXmlFromBool( "useGaussianSmoothing", validator->useGaussianSmoothing() ) );
    xml.append( createXmlFromDouble( "gaussianSigma", validator->getGaussianSigma() ) );
    xml.append( createXmlFromBool( "useSmoothingAmplify", validator->useSmoothingAmplify() ) );
    xml.append( createXmlFromInt( "smoothingAmplify", validator->getSmoothingAmplifyLevel() ) );
    xml.append( "    </Filters>\n\n" );
    return xml;
}

/*******************************************************************************
Writes all subtags of <Network>.
*******************************************************************************/
QString BlobParamsXmlWriter::getNetworkXml( blobIO::BlobParamsValidator * validator )
{
    blobEnums::TuioProfileEnum::Enum profile = validator->getTuioUdpProfile();
    
    QString xml( "    <Network>\n" );
    xml.append( createXmlFromBool( "showNetworkMenu", validator->showNetworkMenu() ) );
    xml.append( createXmlFromBool( "useNetworkSilentMode", validator->useNetworkSilentMode() ) );
    xml.append( createXmlFromBool( "useTuioUdpChannelOne", validator->useTuioUdpChannelOne() ) );
    xml.append( createXmlFromBool( "useTuioUdpChannelTwo", validator->useTuioUdpChannelTwo() ) );
    xml.append( createXmlFromBool( "useFlashXmlChannel", validator->useFlashXmlChannel() ) );
    xml.append( createXmlFromBool( "useBinaryTcpChannel", validator->useBinaryTcpChannel() ) );
    xml.append( createXmlFromString( "localHost", validator->getLocalHost().c_str() ) );
    xml.append( createXmlFromInt( "tuioUdpChannelOnePort", validator->getTuioUdpChannelOnePort() ) );
    xml.append( createXmlFromInt( "tuioUdpChannelTwoPort", validator->getTuioUdpChannelTwoPort() ) );
    xml.append( createXmlFromString( "tuioUdpProtocol", validator->getTuioUdpProfileAsString().c_str() ) );
    xml.append( createXmlFromInt( "flashXmlChannelPort", validator->getFlashXmlChannelPort() ) );
    xml.append( createXmlFromString( "flashXmlProtocol", validator->getFlashXmlProfileAsString().c_str() ) );
    xml.append( createXmlFromInt( "binaryTcpChannelPort", validator->getBinaryTcpChannelPort() ) );
    xml.append( createXmlFromInt( "simpleMessageServerPort", validator->getSimpleMessageServerPort() ) );
    xml.append( "    </Network>\n\n" );
    return xml;
}

/*******************************************************************************
Writes all subtags of <Experimental>.
*******************************************************************************/
QString BlobParamsXmlWriter::getExperimentalXml( blobIO::BlobParamsValidator * validator )
{
    QString xml( "    <Experimental>\n" );
    xml.append( createXmlFromBool( "showExperimentalMenu", validator->showExperimentalMenu() ) );
    xml.append( "    </Experimental>\n\n" );
    return xml;
}

/*******************************************************************************
Returns the tag and its associated value (true or false) as an XML format string.
*******************************************************************************/
QString BlobParamsXmlWriter::createXmlFromBool( QString tag, bool b )
{
    return createXmlFromString( tag, (b ? "true" : "false") );
}

/*******************************************************************************
Returns the tag and its associated integer value as an XML formatted string.
*******************************************************************************/
QString BlobParamsXmlWriter::createXmlFromInt( QString tag, int n )
{
    return createXmlFromString( tag, QString::number( n ) );
}

/*******************************************************************************
Returns the tag and its associated integer value as an XML formatted string.
*******************************************************************************/
QString BlobParamsXmlWriter::createXmlFromDouble( QString tag, double n )
{
    return createXmlFromString( tag, QString::number( n, 'g', 3 ) );
}

/*******************************************************************************
Returns the tag and its associated value as an XML formatted string.
*******************************************************************************/
QString BlobParamsXmlWriter::createXmlFromString( QString tag, QString theValue )
{
    return QString( "        <" + tag + "> " + theValue + " </" + tag + ">\n" );
}
