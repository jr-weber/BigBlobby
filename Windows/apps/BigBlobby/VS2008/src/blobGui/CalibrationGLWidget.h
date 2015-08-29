////////////////////////////////////////////////////////////////////////////////
// CalibrationGLWidget
//
// PURPOSE: Full screen QGLWidget for drawing the CCV calibration screen.
//
// CREATED: 5/25/2011
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
#ifndef BLOBGUI_CALIBRATIONGLWIDGET_H
#define BLOBGUI_CALIBRATIONGLWIDGET_H

#include "blobIO/DebugPrint.h"
#include <QGLWidget>

namespace blobGui { class SourceGLWidget; }
namespace blobGui { class CalibrationGridDialog; }
namespace blobCore { class CoreVision; }

namespace blobGui
{
    /***********************************************************************//**
    Full screen QGLWidget for drawing the CCV calibration screen.

    The actual OpenGL drawing code is in the CCV (Community Core Vision) code.
    This class only needs to provide the full screen rendering surface and to
    pass keyboard signals on to the CoreVision object.
    ***************************************************************************/
    class CalibrationGLWidget : public QGLWidget
    {
        Q_OBJECT

    public:
        static const QColor BACKGROUND_COLOR;

        CalibrationGLWidget( QWidget * parent,
                             blobGui::SourceGLWidget * sourceGLWidget,
                             blobCore::CoreVision * coreVision );
        ~CalibrationGLWidget();
        void frame( double deltaFrameTime );

    public slots:

    signals:
        void closeCalibrationScreen();

    protected:
        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();
        void keyPressEvent( QKeyEvent * event );
        void keyReleaseEvent( QKeyEvent * event );

    private:
        void openCalibrationGridDialog();
        void processKeyEqual( QKeyEvent * keyEvent );
        void processKeyMinus( QKeyEvent * keyEvent );
        void processKeyPlus( QKeyEvent * keyEvent );
        void debugPrintResizeGL( int width, int height );

        blobCore::CoreVision * coreVision_;
        blobGui::CalibrationGridDialog * calibrationGridDialog_;
                
        // Debugging Utility that Writes to Standard Out
        blobIO::DebugPrint debug_;
    };
}

#endif
