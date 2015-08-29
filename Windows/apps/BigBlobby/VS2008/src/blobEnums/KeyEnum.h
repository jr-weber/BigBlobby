////////////////////////////////////////////////////////////////////////////////
// KeyEnum.h
//
// PURPOSE: Contains an enum for most of the keyboard keys (alphabetic keys and
//          a few special character keys).
//
// CREATED: 5/26/2011
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2011, 2015 J.R.Weber <joe.weber77@gmail.com>

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
#ifndef BLOBENUMS_KEYENUM_H
#define BLOBENUMS_KEYENUM_H

#include <string>

namespace blobEnums 
{
    /***********************************************************************//**
    Contains an enum for most of the keyboard keys (alphabetic keys and a few 
    special character keys).

    This enum was created for passing keyboard event information to the 
    BlobCalibration class in a way that would not require that class
    to know about the Qt GUI.  When key press and release events are detected
    by the blobGUI::CalibrationGLWidget class, the Qt key enum will be 
    converted to this library-independent enum and then passed in to the 
    BlobCalibration class, which requires the use of the arrow keys
    along with several keyboard shortcuts during the touch surface calibration 
    procedure done by CCV 1.3 code.
    ***************************************************************************/
    class KeyEnum
    {
    public:
        enum Enum {
            KEY_a,
            KEY_b,
            KEY_c,
            KEY_d,
            KEY_e,
            KEY_f,
            KEY_g,
            KEY_h,
            KEY_i,
            KEY_j,
            KEY_k,
            KEY_l,
            KEY_m,
            KEY_n,
            KEY_o,
            KEY_p,
            KEY_q,
            KEY_r,
            KEY_s,
            KEY_t,
            KEY_u,
            KEY_v,
            KEY_w,
            KEY_x,
            KEY_y,
            KEY_z,
            KEY_ARROW_LEFT,
            KEY_ARROW_RIGHT,
            KEY_ARROW_UP,
            KEY_ARROW_DOWN,
            KEY_PLUS,
            KEY_MINUS,
            KEY_EQUAL_SIGN,
            KEY_UNDERSCORE
        };
        
        static std::string convertToString( blobEnums::KeyEnum::Enum key );
    };
}

#endif
