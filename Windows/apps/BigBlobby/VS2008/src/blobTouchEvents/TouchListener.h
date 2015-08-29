/* TouchListener
 *  
 *  Created on 2/2/09.
 *  Copyright 2009 NUI Group. All rights reserved.
 *
 *  This class was refactored out of CCV 1.3 code.
 *      -J.R.Weber <joe.weber77@gmail.com >
 */
/*
Copyright (c) 2009 NUI Group/Inc. All rights reserved.
Web: http://nuigroup.com
  
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are 
met:
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
#ifndef BLOBTOUCHEVENTS_TOUCHLISTENER_H
#define BLOBTOUCHEVENTS_TOUCHLISTENER_H

#include "blobTracker/Blob.h"

namespace blobTouchEvents
{
    class TouchListener 
    {
    public:
	    void RAWTouchDown( const void * sender, blobTracker::Blob & blob );
	    void RAWTouchUp( const void * sender, blobTracker::Blob & blob );
	    void RAWTouchMoved( const void * sender, blobTracker::Blob & blob );
	    void RAWTouchHeld( const void * sender, blobTracker::Blob & blob );

    protected:
	    virtual void RAWTouchDown( blobTracker::Blob blob ) = 0;
	    virtual void RAWTouchUp( blobTracker::Blob blob ) = 0;
	    virtual void RAWTouchMoved( blobTracker::Blob blob ) = 0;
	    virtual void RAWTouchHeld( blobTracker::Blob blob ) = 0;
    };
}

#endif
