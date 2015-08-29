////////////////////////////////////////////////////////////////////////////////
// KeyEnum.cpp
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
#include "blobEnums/KeyEnum.h"

using blobEnums::KeyEnum;

/***************************************************************************//**
Converts a KeyEnum to a string that could be printed for debugging purposes.
*******************************************************************************/
std::string KeyEnum::convertToString( KeyEnum::Enum key )
{
    switch( key ) {
        case KeyEnum::KEY_a:            return "KEY_a";
        case KeyEnum::KEY_b:            return "KEY_b";
        case KeyEnum::KEY_c:            return "KEY_c";
        case KeyEnum::KEY_d:            return "KEY_d";
        case KeyEnum::KEY_e:            return "KEY_e";
        case KeyEnum::KEY_f:            return "KEY_f";
        case KeyEnum::KEY_g:            return "KEY_g";
        case KeyEnum::KEY_h:            return "KEY_h";
        case KeyEnum::KEY_i:            return "KEY_i";
        case KeyEnum::KEY_j:            return "KEY_j";
        case KeyEnum::KEY_k:            return "KEY_k";
        case KeyEnum::KEY_l:            return "KEY_l";
        case KeyEnum::KEY_m:            return "KEY_m";
        case KeyEnum::KEY_n:            return "KEY_n";
        case KeyEnum::KEY_o:            return "KEY_o";
        case KeyEnum::KEY_p:            return "KEY_p";
        case KeyEnum::KEY_q:            return "KEY_q";
        case KeyEnum::KEY_r:            return "KEY_r";
        case KeyEnum::KEY_s:            return "KEY_s";
        case KeyEnum::KEY_t:            return "KEY_t";
        case KeyEnum::KEY_u:            return "KEY_u";
        case KeyEnum::KEY_v:            return "KEY_v";
        case KeyEnum::KEY_w:            return "KEY_w";
        case KeyEnum::KEY_x:            return "KEY_x";
        case KeyEnum::KEY_y:            return "KEY_y";
        case KeyEnum::KEY_z:            return "KEY_z";
        case KeyEnum::KEY_ARROW_LEFT:   return "KEY_ARROW_LEFT";
        case KeyEnum::KEY_ARROW_RIGHT:  return "KEY_ARROW_RIGHT";
        case KeyEnum::KEY_ARROW_UP:     return "KEY_ARROW_UP";
        case KeyEnum::KEY_ARROW_DOWN:   return "KEY_ARROW_DOWN";
        case KeyEnum::KEY_PLUS:         return "KEY_PLUS";
        case KeyEnum::KEY_MINUS:        return "KEY_MINUS";
        case KeyEnum::KEY_EQUAL_SIGN:   return "KEY_EQUAL_SIGN";
        case KeyEnum::KEY_UNDERSCORE:   return "KEY_UNDERSCORE";
    }
    return "KEY_unknown";
}
