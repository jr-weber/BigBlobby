////////////////////////////////////////////////////////////////////////////////
// BlobParamsXmlReader
//
// Purpose:  Reads blob detector settings from an XML file and creates
//           a BlobParamsValidator object.
//
// Created:  5/23/2011
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
#include "blobIO/BlobParamsXmlReader.h"
#include "blobIO/BlobParamsValidator.h"
#include "blobExceptions/FileNotFoundException.h"
#include "blobExceptions/BlobXmlReaderException.h"
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomDocumentType>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomAttr>
#include <QtXml/QDomText>
#include <QtGui/QMessageBox>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using blobIO::BlobParamsXmlReader;
using blobIO::BlobParamsValidator;
using blobExceptions::FileNotFoundException;
using blobExceptions::BlobXmlReaderException;
using blobExceptions::BlobParamsException;
using blobExceptions::UnknownXmlTagException;

const std::string BlobParamsXmlReader::ASTERISKS_FOR_BANNER( 80, '*' );

BlobParamsXmlReader::BlobParamsXmlReader() :
  unknownXmlTagExceptions_(),
  blobParamsExceptions_(),
  debug_( false )
{
}

BlobParamsXmlReader::~BlobParamsXmlReader()
{
}

/***************************************************************************//**
Reads and parses an XML file to obtain the program settings that are shown in 
the GUI.

The Qt QDomDocument class is used for the parsing.  See the XML file
&quot;data/settings/DefaultBlobParams.xml&quot; for an example of the expected
XML tags.  If a BlobParamsValidator object is returned successfully, it will
hold values that can be used to set the CoreVision object and the GUI controls.
<br />

ERROR HANDLING:<br />
This function can throw a FileNotFoundException, a BlobXmlReaderException, or an 
UnknownXmlTagException if the to level <BlobDetectorParams> tag cannot be found.
*******************************************************************************/
BlobParamsValidator * BlobParamsXmlReader::read( const std::string & xmlFile )
{
    unknownXmlTagExceptions_.clear();
    blobParamsExceptions_.clear();
    debug_.printLn( "\n" + ASTERISKS_FOR_BANNER );
    debug_.printLn( "BlobParamsXmlReader::readXmlFile( " + xmlFile + " ) called..." );
    debug_.printLn( ASTERISKS_FOR_BANNER );

    BlobParamsValidator * validator = NULL;
    QFile file( xmlFile.c_str() );
    QDomDocument doc;
    QString err;
    int line = 0,
        col = 0;

    if( !file.open( QIODevice::ReadOnly ) ) {
        throw FileNotFoundException( xmlFile, "BlobParamsXmlReader::read()" );
    }
    if( !doc.setContent( file.readAll(), &err, &line, &col ) ) {
        throw BlobXmlReaderException( xmlFile,
                                      err.toStdString(), 
                                      line,
                                      col, 
                                      "BlobParamsXmlReader::read()" );
    }
    validator = extractParams( doc, xmlFile );
    debug_.printLn( ASTERISKS_FOR_BANNER + "\n" );
    return validator;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                     Private Helper Functions for read()
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*******************************************************************************
Iterates through all children of the <BlobDetectorParams> tag (case insensitive) 
and plugs each one into a helper function for further processing.    <br />

ERROR HANDLING:    <br />
Throws an UnknownXmlTagException if the <BlobDetectorParams> tag cannot be found.
*******************************************************************************/
BlobParamsValidator * BlobParamsXmlReader::extractParams( QDomDocument & doc,  
                                                          const std::string & xmlFile )
{
    BlobParamsValidator * validator = new BlobParamsValidator( xmlFile );
    QDomElement root = doc.documentElement();
    QDomNode childNode = root.firstChild();
    QString tag = root.tagName().trimmed();
    debug_.printLn( "root = <" + tag.toStdString() + ">" );

    if( tag.compare( "BlobDetectorParams", Qt::CaseInsensitive ) != 0 ) {
        throw UnknownXmlTagException( 
            tag.toStdString(),
            "none (should be root tag, <BlobDetectorParams>)",
            xmlFile,
            "BlobParamsXmlReader::extractParams()" );
    }
    while( !childNode.isNull() ) {
        if( childNode.isElement() ) {
            storeParams( childNode.toElement(), validator );
        }
        childNode = childNode.nextSibling();
    }
    return validator;
}

/*******************************************************************************
Checks for the following XML tags (case insensitive):    <br /><br />

Camera       <br />
Blobs        <br />
Background   <br />
Filters      <br />
Network      <br /><br />

A recognized tag will result in a call to a helper function to process that tag.
<br />

ERROR HANDLING:    <br />
An unrecognized tag will result in an UnknownXmlTagException being added to a
list of UnknownXmlTagExceptions (but is not treated as fatal).
*******************************************************************************/
void BlobParamsXmlReader::storeParams( QDomElement element,
                                       BlobParamsValidator * validator )
{
    QString tag = element.tagName().trimmed();
    QDomNode childNode = element.firstChild();
    debug_.printLn( "   Element = <" + tag.toStdString() + ">" );
    tag = tag.toLower();

    if(      tag == "camera" )       { storeCameraParams( childNode, validator ); }
    else if( tag == "blobs" )        { storeBlobParams( childNode, validator ); } 
    else if( tag == "background" )   { storeBackgroundParams( childNode, validator ); }
    else if( tag == "filters" )      { storeFilterParams( childNode, validator ); }
    else if( tag == "network" )      { storeNetworkParams( childNode, validator ); }
    else if( tag == "experimental" ) { storeExperimentalParams( childNode, validator ); }
    else { 
        if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
        UnknownXmlTagException e( tag.toStdString(),
                                  "BlobDetectorParams",
                                  validator->getBlobParamsXmlFilename(),
                                  "BlobParamsXmlReader::storeParams()" );
        unknownXmlTagExceptions_.push_back( e );
    }
}

/*******************************************************************************
Checks for the following XML tags (case insensitive):    <br /><br />

deviceID                 <br />
videoRate                <br />
videoWidth               <br />
videoHeight              <br />
flipImageVertically      <br />
flipImageHorizontally    <br /><br />

ERROR HANDLING:    <br />
An unrecognized tag will result in an UnknownXmlTagException being added to a
list of UnknownXmlTagExceptions (but is not treated as fatal).  A tag value that
makes no sense to the validator object will result in the validator throwing a 
BlobParamsException, which will be added to a list of BlobParamsExceptions.
*******************************************************************************/
void BlobParamsXmlReader::storeCameraParams( QDomNode & node,
                                             BlobParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debug_.printLn( "        XML tag: " + tag.toStdString() + " = " + text.toStdString() );
            tag = tag.toLower();

            try {
                if( tag == "deviceid" )    { validator->setDeviceID( text ); }
                else if( tag == "videorate" )   { validator->setVideoRate( text ); }
                else if( tag == "videowidth" )  { validator->setVideoWidth( text ); }
                else if( tag == "videoheight" ) { validator->setVideoHeight( text ); }
                else if( tag == "flipimagevertically" )   { validator->flipImageVertically( text ); }
                else if( tag == "flipimagehorizontally" ) { validator->flipImageHorizontally( text ); }
                else { 
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    UnknownXmlTagException e( tag.toStdString(),
                                              "Camera",
                                              validator->getBlobParamsXmlFilename(),
                                              "BlobParamsXmlReader::storeCameraParams()" );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( BlobParamsException e ) {
                blobParamsExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

/*******************************************************************************
Checks for the following XML tags (case insensitive):    <br /><br />

imageThreshold          <br />
minBlobSize             <br />
maxBlobSize             <br />
maxNumberBlobs          <br />
useDarkBlobs            <br />
showPressure            <br />
showOutlines            <br />
showAreas               <br />
showBlobLabels          <br /><br />

ERROR HANDLING:    <br />
An unrecognized tag will result in an UnknownXmlTagException being added to a
list of UnknownXmlTagExceptions (but is not treated as fatal).  A tag value that
makes no sense to the validator object will result in the validator throwing a 
BlobParamsException, which will be added to a list of BlobParamsExceptions.
*******************************************************************************/
void BlobParamsXmlReader::storeBlobParams( QDomNode & node,
                                           BlobParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debug_.printLn( "        XML tag: " + tag.toStdString() + " = " + text.toStdString() );
            tag = tag.toLower();

            try {
                if(      tag == "imagethreshold" ) { validator->setImageThreshold( text ); }
                else if( tag == "minblobsize" )    { validator->setMinBlobSize( text ); }
                else if( tag == "maxblobsize" )    { validator->setMaxBlobSize( text ); }
                else if( tag == "maxnumberblobs" )       { validator->setMaxNumberBlobs( text ); }
                else if( tag == "usedarkblobs" )         { validator->useDarkBlobs( text ); }

                else if( tag == "showcalibrationgrid" )  { validator->showCalibrationGrid( text ); }
                else if( tag == "showareas" )            { validator->showBlobAreas( text ); }
                else if( tag == "showoutlines" )         { validator->showBlobOutlines( text ); }
                else if( tag == "showcrosshairs" )       { validator->showBlobCrosshairs( text ); }
                else if( tag == "showboundingboxes" )    { validator->showBlobBoundingBoxes( text ); }

                else if( tag == "normalizeblobintensities" ) { validator->normalizeBlobIntensities( text ); }
                else if( tag == "showlabels" )               { validator->showBlobLabels( text ); }
                else {  
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    UnknownXmlTagException e( tag.toStdString(),
                                              "Blobs",
                                              validator->getBlobParamsXmlFilename(),
                                              "BlobParamsXmlReader::storeBlobParams()" );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( BlobParamsException e ) {
                blobParamsExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

/*******************************************************************************
Checks for the following XML tags (case insensitive):    <br /><br />

useAutoBackground          <br />
backgroundLearnRate        <br />
usePeriodicBackground      <br />
periodicBackgroundSeconds  <br />
periodicBackgroundPercent  <br /><br />

ERROR HANDLING:    <br />
An unrecognized tag will result in an UnknownXmlTagException being added to a
list of UnknownXmlTagExceptions (but is not treated as fatal).  A tag value that
makes no sense to the validator object will result in the validator throwing a 
BlobParamsException, which will be added to a list of BlobParamsExceptions.
*******************************************************************************/
void BlobParamsXmlReader::storeBackgroundParams( QDomNode & node, 
                                                 blobIO::BlobParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debug_.printLn( "        XML tag: " + tag.toStdString() + " = " + text.toStdString() );
            tag = tag.toLower();

            try {
                if( tag == "useautobackground" ) { validator->useAutoBackground( text ); }
                else if( tag == "backgroundlearnrate" ) { validator->setBackgroundLearnRate( text ); }
                else if( tag == "useperiodicbackground" ) { validator->usePeriodicBackground( text ); }
                else if( tag == "periodicbackgroundseconds" ) { validator->setPeriodicBackgroundSeconds( text ); }
                else if( tag == "periodicbackgroundpercent" ) { validator->setPeriodicBackgroundPercent( text ); }
                else { 
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    UnknownXmlTagException e( tag.toStdString(),
                                              "Background",
                                              validator->getBlobParamsXmlFilename(),
                                              "BlobParamsXmlReader::storeBackgroundParams()" );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( BlobParamsException e ) {
                blobParamsExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

/*******************************************************************************
Checks for the following XML tags (case insensitive):    <br /><br />

useMask              <br />
maskWidthPadding     <br />
maskHeightPadding    <br />
useHighpassAmplify        <br />
preamplify           <br />
useHighpass          <br />
highpassBlur         <br />
useHighpassTwo       <br />
highpassTwoKernel    <br />
useSmoothing         <br />
smoothingBlur        <br />
useSmoothingAmplify  <br />
smoothingAmplify     <br /><br />

ERROR HANDLING:    <br />
An unrecognized tag will result in an UnknownXmlTagException being added to a
list of UnknownXmlTagExceptions (but is not treated as fatal).  A tag value that
makes no sense to the validator object will result in the validator throwing a 
BlobParamsException, which will be added to a list of BlobParamsExceptions.
*******************************************************************************/
void BlobParamsXmlReader::storeFilterParams( QDomNode & node, 
                                             BlobParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debug_.printLn( "        XML tag: " + tag.toStdString() + " = " + text.toStdString() );
            tag = tag.toLower();

            try {
                if(      tag == "hidegaussiancontrols" )              { validator->hideGaussianControls( text ); }
                else if( tag == "hideearlysmoothing" )                { validator->hideEarlySmoothing( text ); }
                else if( tag == "useprebackgroundsmoothing" )         { validator->usePreBkSmoothing( text ); }
                else if( tag == "prebackgroundsmoothingblur" )        { validator->setPreBkSmoothingBlur( text ); }
                else if( tag == "usegaussianprebackgroundsmoothing" ) { validator->useGaussianPreBkSmoothing( text ); }
                else if( tag == "gaussianprebackgroundsigma" )        { validator->setGaussianPreBkSigma( text ); }

                else if( tag == "usemask" )                 { validator->useMask( text ); }
                else if( tag == "maskwidthpadding" )   { validator->setMaskWidthPadding( text ); }
                else if( tag == "maskheightpadding" )  { validator->setMaskHeightPadding( text ); }

                else if( tag == "usehighpass" )        { validator->useHighpass( text ); }
                else if( tag == "highpassblur" )       { validator->setHighpassBlur( text ); }
                else if( tag == "usehighpassamplify" ) { validator->useHighpassAmplify( text ); }
                else if( tag == "highpassamplify" )    { validator->setHighpassAmplifyLevel( text ); }

                else if( tag == "usehighpass2" )       { validator->useHighpassTwo( text ); }
                else if( tag == "highpassblur2" )      { validator->setHighpassTwoKernel( text ); }
                else if( tag == "usehighpasstwoamplify" ) { validator->useHighpassTwoAmplify( text ); }
                else if( tag == "highpasstwoamplify" )    { validator->setHighpassTwoAmplifyLevel( text ); }

                else if( tag == "usesmoothing" )         { validator->useSmoothing( text ); }
                else if( tag == "smoothingblur" )        { validator->setSmoothingBlur( text ); }
                else if( tag == "usegaussiansmoothing" ) { validator->useGaussianSmoothing( text ); }
                else if( tag == "gaussiansigma" )        { validator->setGaussianSigma( text ); }
                else if( tag == "usesmoothingamplify" )  { validator->useSmoothingAmplify( text ); }
                else if( tag == "smoothingamplify" )     { validator->setSmoothingAmplifyLevel( text ); }
                else {
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    UnknownXmlTagException e( tag.toStdString(),   
                                              "Filters",
                                              validator->getBlobParamsXmlFilename(),
                                              "BlobParamsXmlReader::storeFilterParams()" );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( BlobParamsException e ) {
                blobParamsExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

/*******************************************************************************
Checks for the following XML tags (case insensitive):    <br /><br />

showNetworkMenu          <br />
useTuioUdpChannelOne     <br />
useTuioUdpChannelTwo     <br />
showPlaysurfaceCheckBox  <br />
useFlashXmlChannel       <br />
useBinaryTcpChannel      <br />
localHost                <br />
tuioUdpChannelOnePort    <br />
tuioUdpChannelTwoPort    <br />
tuioUdpProtocol          <br />
flashXmlChannelPort      <br />
flashXmlProtocol         <br />
binaryTcpChannelPort     <br /><br />

ERROR HANDLING:    <br />
An unrecognized tag will result in an UnknownXmlTagException being added to a
list of UnknownXmlTagExceptions (but is not treated as fatal).  A tag value that
makes no sense to the validator object will result in the validator throwing a 
BlobParamsException, which will be added to a list of BlobParamsExceptions.
*******************************************************************************/
void BlobParamsXmlReader::storeNetworkParams( QDomNode & node, 
                                              blobIO::BlobParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debug_.printLn( "        XML tag: " + tag.toStdString() + " = " + text.toStdString() );
            tag = tag.toLower();

            try {
                if( tag == "shownetworkmenu" )              { validator->showNetworkMenu( text ); }
                else if( tag == "usenetworksilentmode" )    { validator->useNetworkSilentMode( text ); } 
                else if( tag == "usetuioudpchannelone" )    { validator->useTuioUdpChannelOne( text ); }
                else if( tag == "usetuioudpchanneltwo" )    { validator->useTuioUdpChannelTwo( text ); } 
                else if( tag == "useflashxmlchannel" )      { validator->useFlashXmlChannel( text ); }
                else if( tag == "usebinarytcpchannel" )     { validator->useBinaryTcpChannel( text ); }
                else if( tag == "localhost" )               { validator->setLocalHost( text ); }
                else if( tag == "tuioudpchanneloneport" )   { validator->setTuioUdpChannelOnePort( text ); }
                else if( tag == "tuioudpchanneltwoport" )   { validator->setTuioUdpChannelTwoPort( text ); }
                else if( tag == "tuioudpprotocol" )         { validator->setTuioUdpProfile( text ); }
                else if( tag == "flashxmlchannelport" )     { validator->setFlashXmlChannelPort( text ); }
                else if( tag == "flashxmlprotocol" )        { validator->setFlashXmlProfile( text ); }
                else if( tag == "binarytcpchannelport" )    { validator->setBinaryTcpChannelPort( text ); }
                else if( tag == "simplemessageserverport" ) { validator->setSimpleMessageServerPort( text ); }
                else {
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    UnknownXmlTagException e( tag.toStdString(),
                                              "Network",
                                              validator->getBlobParamsXmlFilename(),
                                              "BlobParamsXmlReader::storeNetworkParams()" );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( BlobParamsException e ) {
                blobParamsExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

/*******************************************************************************
Checks for the following XML tags (case insensitive):    <br /><br />

showExperimentalMenu  <br /><br />

ERROR HANDLING:    <br />
An unrecognized tag will result in an UnknownXmlTagException being added to a
list of UnknownXmlTagExceptions (but is not treated as fatal).  A tag value that
makes no sense to the validator object will result in the validator throwing a 
BlobParamsException, which will be added to a list of BlobParamsExceptions.
*******************************************************************************/
void BlobParamsXmlReader::storeExperimentalParams( QDomNode & node, 
                                                   blobIO::BlobParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debug_.printLn( "        XML tag: " + tag.toStdString() + " = " + text.toStdString() );
            tag = tag.toLower();

            try {
                if( tag == "showexperimentalmenu" ) { validator->showExperimentalMenu( text ); }
                else {
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    UnknownXmlTagException e( tag.toStdString(),
                                              "Experimental",
                                              validator->getBlobParamsXmlFilename(),
                                              "BlobParamsXmlReader::storeExperimentalParams()" );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( BlobParamsException e ) {
                blobParamsExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

bool BlobParamsXmlReader::hasUnknownXmlTagExceptions()
{
    return (unknownXmlTagExceptions_.size() > 0);
}

std::vector<blobExceptions::UnknownXmlTagException> BlobParamsXmlReader::unknownXmlTagExceptions()
{
    return unknownXmlTagExceptions_;
}

bool BlobParamsXmlReader::hasBlobParamsExceptions()
{
    return (blobParamsExceptions_.size() > 0);
}

std::vector<blobExceptions::BlobParamsException> BlobParamsXmlReader::blobParamsExceptions()
{
    return blobParamsExceptions_;
}
