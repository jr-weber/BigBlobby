/*  TouchEvents
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
#include "blobTouchEvents/TouchEvents.h"
#include "blobTouchEvents/TouchListener.h"
#include <Poco/Delegate.h>

using blobTouchEvents::TouchEvents;
using blobTouchEvents::TouchListener;

TouchEvents & TouchEvents::instance()
{
    static TouchEvents touchEventsSingleton;
    return touchEventsSingleton;
}

void TouchEvents::addRAWListener( TouchListener * listener )
{
	addRAWTouchDownListener(listener);
	addRAWTouchUpListener(listener);
	addRAWTouchMovedListener(listener);
	addRAWTouchHeldListener(listener);
}

void TouchEvents::addRAWTouchDownListener( TouchListener * listener )
{
	RAWTouchDown += Poco::Delegate<TouchListener, blobTracker::Blob>( listener, &TouchListener::RAWTouchDown );
}

void TouchEvents::addRAWTouchUpListener( TouchListener * listener )
{
	RAWTouchUp += Poco::Delegate<TouchListener, blobTracker::Blob>( listener, &TouchListener::RAWTouchUp );
}

void TouchEvents::addRAWTouchMovedListener( TouchListener * listener )
{
	RAWTouchMoved += Poco::Delegate<TouchListener, blobTracker::Blob>( listener, &TouchListener::RAWTouchMoved );
}

void TouchEvents::addRAWTouchHeldListener( TouchListener * listener )
{
	RAWTouchHeld += Poco::Delegate<TouchListener, blobTracker::Blob>( listener, &TouchListener::RAWTouchHeld );
}

void TouchEvents::notifyRAWTouchDown( void * sender )
{
	RAWTouchDown.notify( sender, RAWmessengerBlob );
}

void TouchEvents::notifyRAWTouchUp( void * sender )
{
	RAWTouchUp.notify( sender, RAWmessengerBlob );
}

void TouchEvents::notifyRAWTouchMoved( void * sender )
{
	RAWTouchMoved.notify( sender, RAWmessengerBlob );
}

void TouchEvents::notifyRAWTouchHeld( void * sender )
{
	RAWTouchHeld.notify( sender, RAWmessengerBlob );
}
