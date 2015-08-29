////////////////////////////////////////////////////////////////////////////////
// BlobMainWindowView
//
// PURPOSE: This helper class for BlobMainWindow has functions for controlling
//          the size of the Source, Tracked, and filter images.
//
// CREATED:  6/20/2013
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2013, 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#ifndef BLOBGUI_BLOBMAINWINDOWVIEW_H
#define BLOBGUI_BLOBMAINWINDOWVIEW_H

#include "ui_BlobMainWindow.h"
#include "blobIO/DebugPrint.h"

namespace blobGui { class BlobMainWindow; }
class QRect;

namespace blobGui 
{
    /***********************************************************************//**
    This helper class for BlobMainWindow has functions for controlling the size
    of windows, widgets, and images.  The public functions of BlobMainWindowView
    are called on primarily by the View menu at the top of the main window.
    ***************************************************************************/
    class BlobMainWindowView : public QObject
    {
        Q_OBJECT

    public:
        static const double MIN_ASPECT_RATIO;
        static const int TOP_MARGIN,
                         SIDE_MARGIN,
                         MIN_FRAME_GEO_WIDTH,
                         MIN_FRAME_GEO_HEIGHT,
                         MAX_WIDGET_DIMENSION;

        BlobMainWindowView( BlobMainWindow * blobMainWindow,
                            Ui::BlobMainWindowUi * ui );
        virtual ~BlobMainWindowView();

    public slots:
        void setViewToMinimalWindow();
        void setViewToDefault();
        void zoomMainPanelImages100();
        void zoomMainPanelImages200();
        void zoomMainPanelImages400();
        void zoomMainPanelImagesPercent( int percent );

    signals:
        void zoomMainPanelImagesCalled( int percent );

    private:
        void calculateDefaultSizeAndCentering();
        void resizeAndCenterToDefault();
        void memorizeMainWindowDimensions();
        void scaleImages( double scaleFactor );

        void debugPrintGeo( const std::string & name, const QRect & rect );
        void debugPrintInitialDimensions();

        BlobMainWindow * blobMainWindow_;
        Ui::BlobMainWindowUi * mainUi_;
        double initialAspectRatio_;
        int initialFrameX_,
            initialFrameY_,
            initialMainWindowWidth_,
            initialMainWindowHeight_,
            initialMainPanelWidth_,
            initialMainPanelHeight_,
            initialMainPanelMinWidth_,
            initialMainPanelMinHeight_,
            initialImageWidth_,
            initialImageHeight_;

        blobIO::DebugPrint debug_; // Debug utility writes to standard out.
    };
}

#endif
