// Multicam version of PS3Eye fix
#include "ofxPS3.h"
#include <stdio.h>

unsigned int ofxPS3::camNum=0;

ofxPS3::ofxPS3() 
{
}

ofxPS3::~ofxPS3()
{
	// Stop capturing
	PS3EyeMulticamStop();
	Sleep(50);
    saveCameraSettingsToXmlFile();
	//PS3EyeMulticamClose(); // Process will not exit if this is called!	
	delete [] pBuffer;

	// this delete the temp settings.xml which is saved to data/settings/cameras/cameras.xml
	remove("settings.xml");
}

void ofxPS3::saveCameraSettingsToXmlFile()
{
    printf( "About to save data/settings/cameras/cameras.xml file...\n" );
    PS3EyeMulticamSaveSettings( "data/settings/cameras/cameras.xml" );
}


void ofxPS3::listDevices()
{
	// Enumerate the cameras on the bus.
	camNum = PS3EyeMulticamGetCameraCount();
 	printf("\nFound %d PS3Eye camera(s)...\n", camNum);
}

PBYTE ofxPS3::getPixels()
{
	return pBuffer;
}

bool ofxPS3::isFrameNew()
{
	return PS3EyeMulticamGetFrame(pBuffer);
}

void ofxPS3::initPS3(int width, int height, int framerate)
{
	printf("selecting format...\n");
	PS3EyeMulticamOpen(camNum, (height==480) ? VGA:QVGA, framerate);
	PS3EyeMulticamLoadSettings("data/settings/cameras/cameras.xml");
	// get stitched image width
	PS3EyeMulticamGetFrameDimensions(camWidth, camHeight);
	// Allocate image buffer for grayscale image
	pBuffer = new BYTE[camWidth*camHeight];
	// Start capturing
	PS3EyeMulticamStart();
}

int ofxPS3::getDeviceCount()
{
	return PS3EyeMulticamGetCameraCount();
}

int ofxPS3::getCamWidth()
{
	return camWidth;
}

int ofxPS3::getCamHeight()
{
	return camHeight;
}

void ofxPS3::showSettings()
{
	PS3EyeMulticamShowSettings();
}
