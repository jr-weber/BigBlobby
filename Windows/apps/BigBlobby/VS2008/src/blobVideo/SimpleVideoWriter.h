////////////////////////////////////////////////////////////////////////////////
// SimpleVideoWriter
//
// PURPOSE: Writes grayscale images to a video file that can be used for 
//          testing BigBlobby.
//
// CREATED: 7/2/2014
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
#ifndef BLOBVIDEO_VIDEOWRITER_H
#define BLOBVIDEO_VIDEOWRITER_H

#include <string>

namespace blobBuffers { class GrayBuffer; }
namespace blobUtils { class FunctionTimer; }
namespace cv { class VideoWriter; }
namespace cv { class Mat; }

namespace blobVideo
{
    class SimpleVideoWriter
    {
    public:
        static const int RGB_BYTES;

        SimpleVideoWriter();
        ~SimpleVideoWriter();

        bool initGrayscale( const std::string & filename,
                            int videoWidth,
                            int videoHeight,
                            int videoRate = 20 );
        bool initColor( const std::string & filename,
                        int videoWidth,
                        int videoHeight,
                        int videoRate = 20 );
        void flipImageVertical( bool b );
        void flipImageHorizontal( bool b );
        bool processGrayscaleImage( blobBuffers::GrayBuffer * grayBuffer );
        bool processRgbImage( unsigned char * bytes, int width, int height, int bytesPerRow );
        void clear();
        double calculateAverageTime();

    private:
        void initBuffer( int bufferWidth, int bufferHeight );
        void flipIfRequested( cv::Mat & m );

        blobUtils::FunctionTimer * processTimer_;
        cv::VideoWriter * cvVideoWriter_;
        unsigned char * rgbPixels_;
        int bufferWidth_,
            bufferHeight_;
        bool flipVertical_,
             flipHorizontal_;
    };
}

#endif
