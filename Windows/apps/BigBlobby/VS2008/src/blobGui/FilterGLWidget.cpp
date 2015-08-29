////////////////////////////////////////////////////////////////////////////////
// FilterGLWidget
//
// PURPOSE: Provides an OpenGL rendering surface that will be used for displaying
//          a processed camera image from CCV (Community Core Vision) code.
//
// CREATED: 4/13/2011
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
#include "blobGui/FilterGLWidget.h"
#include "blobFilters/ImageFilters.h"
#include "blobBuffers/GrayBuffer.h"

using blobGui::FilterGLWidget;
using blobEnums::FilterEnum;

const int FilterGLWidget::VIDEO_WIDTH = 320,
          FilterGLWidget::VIDEO_HEIGHT = 240;

/***************************************************************************//**
The filterType argument will determine what image is rendered on the surface of
this widget.

The actual rendering with OpenGL code will be done done by CCV (Community Core 
Vision) objects held by the CoreVision argument.  Handing over a simple image 
buffer and letting this class worry about how to render it would be a better way
to go, but that would likely require an extensive amout of refactoring (or a 
complete rewrite) of a fair amount of the CCV code.
*******************************************************************************/
FilterGLWidget::FilterGLWidget( QWidget * parent,
                                blobGui::SourceGLWidget * sourceGLWidget,
                                blobCore::CoreVision * coreVision,
                                blobEnums::FilterEnum::Enum filterType ) :
  QGLWidget( parent, sourceGLWidget ),
  coreVision_( coreVision ),
  imageFilters_( NULL ),
  filterType_( filterType ),
  backgroundColor_(),
  textureName_( 0 ),
  textureWidth_( 0 ),
  textureHeight_( 0 ),
  viewportWidth_( VIDEO_WIDTH ),
  viewportHeight_( VIDEO_HEIGHT ),
  videoWidth_( VIDEO_WIDTH ),
  videoHeight_( VIDEO_HEIGHT ),
  showHelpImage_( false ),
  helpImage_(),
  helpTextureName_( 0 ),
  debug_( false )
{
    imageFilters_ = coreVision_->imageFilters();
    initBackgroundColor();
}

FilterGLWidget::~FilterGLWidget()
{
}

/***************************************************************************//**
Calls on updateGL(), which will set up the OpenGL context and then call on 
paintGL().  

This function does not block.
*******************************************************************************/
void FilterGLWidget::frame( double deltaFrameTime )
{
    if( isVisible() ) {
        updateGL();
    }
}

void FilterGLWidget::setHelpImage( QImage & helpImage )
{
    helpImage_ = helpImage;

    if( helpImage_.isNull() ) {
        std::cout << "The image is NULL.\n"; 
    }
}

void FilterGLWidget::showHelpImage( bool show )
{
    showHelpImage_ = show;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Protected Functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Sets the background color for the OpenGL drawing surface.
*******************************************************************************/
void FilterGLWidget::initializeGL()
{
    qglClearColor( backgroundColor_ );
}

/***************************************************************************//**
This function will check whether the aspect ratio of the viewport matches the
aspect ratio of the camera Source image, which should be 4:3 for a 320 x 240 
pixel VGA image (or a 640 x 480 pixel higher resolution image).  If the aspect
ratio does not match, the image will not be stretched.  Rather, the largest
correct aspect 4:3 image that can be centered on the widget surface, and the
extra area will be painted as the same background color of the window.  
<br />

WARNING:<br />
Trying an alternate approach such as calling on resize(w,h) inside of resizeGL()
would be a very bad idea, as that would lead to an infinite recursion.
*******************************************************************************/
void FilterGLWidget::resizeGL( int viewportWidth, int viewportHeight )
{
    viewportWidth_ = viewportWidth;
    viewportHeight_ = viewportHeight;
    videoWidth_ = coreVision_->getVideoWidth();
    videoHeight_ = coreVision_->getVideoHeight();
    float videoAspect = (float)videoWidth_ / videoHeight_,
          viewportAspect = (float)viewportWidth_ / viewportHeight_;

    //debugPrintResizeGL( viewportWidth_, viewportHeight_, videoWidth_, videoHeight_ );
    glViewport( 0, 0, viewportWidth_, viewportHeight_ );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    if( viewportAspect > videoAspect ) {
        // Viewport is too wide, so need some left/right padding (on x-axis)
        int biggerWidth = (int)((viewportAspect / videoAspect) * videoWidth_),
            adjustment = (biggerWidth - videoWidth_) / 2;
        glOrtho( -adjustment, videoWidth_ + adjustment, 0, videoHeight_, -1.0, 1.0 );
    }
    else if( videoAspect > viewportAspect ) {
        // Viewport is too tall, so need some top/bottom padding (on y-axis).
        int biggerHeight = (int)((videoAspect / viewportAspect) * videoHeight_),
            adjustment = (biggerHeight - videoHeight_) / 2;
        glOrtho( 0, videoWidth_, -adjustment, videoHeight_ + adjustment, -1.0, 1.0 );
    }
    else { // Aspect ratios are equal.
        glOrtho( 0, videoWidth_, 0, videoHeight_, -1.0, 1.0 );
    }
    glMatrixMode( GL_MODELVIEW );
}

/***************************************************************************//**
Obtains the GrayBuffer corresponding to the filter type of this widget and
renders it using an OpenGL Texture object.  If the filter type is TRACKED,
then the CoreVision object will be asked to render blob ID numbers on top of 
the filter image.  If requested, an image displaying help info can be drawn
instead.
*******************************************************************************/
void FilterGLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    blobBuffers::GrayBuffer * grayBuffer = NULL;
    grayBuffer = imageFilters_->grayBuffer( filterType_ );

    if( grayBuffer != NULL ) {
        if( cameraSizeHasChanged( grayBuffer ) ) {
            resizeGL( viewportWidth_, viewportHeight_ );
        }
        if( showHelpImage_ ) {
            drawHelpImage( grayBuffer->width(), grayBuffer->height() );
        }
        else {
            drawFilterImage( grayBuffer );
        }
    }
}

/*******************************************************************************
If the video player is used and it has a different size than the camera, the
resizeGL() function will need to be called again.
*******************************************************************************/
bool FilterGLWidget::cameraSizeHasChanged( blobBuffers::GrayBuffer * grayBuffer )
{
    return (grayBuffer->width() != videoWidth_) 
        && (grayBuffer->height() != videoHeight_);
}

/*******************************************************************************
Draws an image containing help instructions for this filter.  Texture mapping 
is used only to be consistent with how the filter images are rendered.
*******************************************************************************/
void FilterGLWidget::drawHelpImage( int width, int height )
{
    if( helpTextureName_ == 0 && !helpImage_.isNull() ) {
        helpTextureName_ = bindTexture( helpImage_ );
    }
    if( helpTextureName_ != 0 ) {
        glMatrixMode( GL_MODELVIEW );
        glEnable( GL_TEXTURE_2D );
            glBindTexture( GL_TEXTURE_2D, helpTextureName_ );
            glBegin( GL_QUADS );
                glTexCoord2f( 0, 0 );
                glVertex2f( 0, 0 );
                glTexCoord2f( 1.0f, 0 );
                glVertex2f( width, 0 );
                glTexCoord2f( 1.0f, 1.0f );
                glVertex2f( width, height );
                glTexCoord2f( 0, 1.0f );
                glVertex2f( 0, height );
            glEnd();
        glDisable( GL_TEXTURE_2D );
    }
}

/*******************************************************************************
Rotates and translates the image to be drawn to account for OpenGL using 
positive y as upwards, while Qt widgets use positive y as downwards.
*******************************************************************************/
void FilterGLWidget::drawFilterImage( blobBuffers::GrayBuffer * grayBuffer )
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
        glTranslatef( 0.0f, coreVision_->getVideoHeight(), 0.0f );
        glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );
        drawTexture( grayBuffer );

        if( filterType_ == FilterEnum::TRACKED ) {
            coreVision_->drawBlobIdNumbers();
        }
    glPopMatrix();
}

/*******************************************************************************
Uses an OpenGL Texture object to render the GrayBuffer.
*******************************************************************************/
void FilterGLWidget::drawTexture( blobBuffers::GrayBuffer * grayBuffer )
{
    int bufferWidth = grayBuffer->width(),
        bufferHeight = grayBuffer->height();

    if( bufferWidth != textureWidth_ || bufferHeight != textureHeight_ ) {
        initializeTexture( bufferWidth, bufferHeight );
    }
    glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureName_ );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, bufferWidth, bufferHeight,
                         GL_LUMINANCE, GL_UNSIGNED_BYTE, grayBuffer->pixels() );
        glBegin( GL_QUADS );
            glTexCoord2f( 0, 0 );
            glVertex2f( 0, 0 );
            glTexCoord2f( 1.0f, 0 );
            glVertex2f( bufferWidth, 0 );
            glTexCoord2f( 1.0f, 1.0f );
            glVertex2f( bufferWidth, bufferHeight );
            glTexCoord2f( 0, 1.0f );
            glVertex2f( 0, bufferHeight );
        glEnd();
    glDisable( GL_TEXTURE_2D );
}

/*******************************************************************************
Initializes the texture map that will be used for rendering a GrayBuffer.
*******************************************************************************/
void FilterGLWidget::initializeTexture( int width, int height )
{
    textureWidth_ = width;
    textureHeight_ = height;
    glDeleteTextures( 1, &textureName_ );
    glGenTextures( 1, &textureName_ );

    glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureName_ );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, 
                      textureWidth_, textureHeight_, 0, 
                      GL_LUMINANCE, GL_UNSIGNED_BYTE, 0 );

        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glDisable( GL_TEXTURE_2D );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Private Functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*******************************************************************************
This helper function for the constructor obtains the background color of the
window and stores it for use by the OpenGL rendering code.

If the user resizes this widget to be something other than the 4:3 aspect ratio 
for a VGA image, the image will still be rendered with a 4:3 aspect, and any 
left over area will be filled with the background color.  This approach was
used because Qt does not have any built-in way of restricting the aspect ratio
of a QGLWidget as it is resized.
*******************************************************************************/
void FilterGLWidget::initBackgroundColor()
{
    QPalette pal = palette();
    backgroundColor_ = pal.color( QPalette::Window );
    debugPrintBackgroundColor();
}

/*******************************************************************************
If debugging is turned on for this class, writes the background color to 
standard out.
*******************************************************************************/
void FilterGLWidget::debugPrintBackgroundColor()
{
    debug_.printLn( "\nFilterGLWidget rgb = ("
        + debug_.intToStr( backgroundColor_.red() ) + ", " 
        + debug_.intToStr( backgroundColor_.green() ) + ", " 
        + debug_.intToStr( backgroundColor_.blue() ) + ")\n" );
}

/*******************************************************************************
If debugging is turned on for this class, writes the width and height of the
viewport and the width and height of the video camera.
*******************************************************************************/
void FilterGLWidget::debugPrintResizeGL( int viewportWidth, int viewportHeight, 
                                           int videoWidth, int videoHeight )
{
    debug_.printLn( "\n***** FilterGLWidget::resizeGL(" 
        + debug_.intToStr( viewportWidth ) + ", " 
        + debug_.intToStr( viewportHeight ) + ") *****\n"
        + "videoWidth = " + debug_.intToStr( videoWidth ) + "\n"
        + "videoHeight = " + debug_.intToStr( videoHeight ) + "\n" );
}
