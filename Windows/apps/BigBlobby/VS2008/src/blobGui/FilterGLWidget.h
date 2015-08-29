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
#ifndef BLOBGUI_FILTERGLWIDGET_H
#define BLOBGUI_FILTERGLWIDGET_H

#include "blobGui/SourceGLWidget.h"
#include "blobEnums/FilterEnum.h"
#include "blobIO/DebugPrint.h"
#include <QGLWidget>
#include <QColor>
#include <QRect>
#include <QImage>

namespace blobFilters { class ImageFilters; }
namespace blobBuffers { class GrayBuffer; }

namespace blobGui
{
    /***********************************************************************//**
    Provides an OpenGL rendering surface that will be used for displaying a
    processed camera image from CCV (Community Core Vision) code.

    The Background, Smoothing, Highpass, Amplify, and Tracked images of the
    BigBlobby GUI are each rendered using an instance of FilterGLWidget,
    while only the Source image is rendered with a SourceGLWidget.
    <br />

    DESIGN NOTE:<br />
    While this class is a great deal simpler than the SourceGLWidget class, these
    two classes have several private or protected functions that are the same
    or very similar.  At some future time, it would probably be a good idea to 
    create a common superclass with the shared code.
    ***************************************************************************/
    class FilterGLWidget : public QGLWidget
    {
        Q_OBJECT
    public:
        static const int VIDEO_WIDTH,
                         VIDEO_HEIGHT;

        FilterGLWidget( QWidget * parent,
                          blobGui::SourceGLWidget * sourceGLWidget,
                          blobCore::CoreVision * coreVision,
                          blobEnums::FilterEnum::Enum filterType );
        ~FilterGLWidget();
        void frame( double deltaFrameTime );
        void setHelpImage( QImage & helpImage );
        void showHelpImage( bool show );

    protected:
        void initializeGL();
        void resizeGL( int viewportWidth, int viewportHeight );
        void paintGL();

    private:
        void initBackgroundColor();
        void drawHelpImage( int width, int height );
        void drawFilterImage( blobBuffers::GrayBuffer * grayBuffer );
        void drawTexture( blobBuffers::GrayBuffer * grayBuffer );
        void initializeTexture( int width, int height );
        bool cameraSizeHasChanged( blobBuffers::GrayBuffer * grayBuffer );
        void debugPrintBackgroundColor();
        void debugPrintResizeGL( int viewportWidth, 
                                 int viewportHeight, 
                                 int videoWidth, 
                                 int videoHeight );
            
        blobCore::CoreVision * coreVision_;
        blobFilters::ImageFilters * imageFilters_;
        blobEnums::FilterEnum::Enum filterType_;
        QColor backgroundColor_;
        GLuint textureName_;
        int textureWidth_,
            textureHeight_,
            viewportWidth_,
            viewportHeight_,
            videoWidth_,
            videoHeight_;
        bool showHelpImage_;
        QImage helpImage_;
        GLuint helpTextureName_;

        // Debugging Utility that Writes to Standard Out
        blobIO::DebugPrint debug_;
    };
}

#endif
