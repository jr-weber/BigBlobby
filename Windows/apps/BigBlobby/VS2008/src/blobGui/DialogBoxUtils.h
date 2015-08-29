////////////////////////////////////////////////////////////////////////////////
// DialogBoxUtils
//
// Purpose: This utility class knows how to extract info from any subclass 
//          of BlobException and format the info for presentation in a modal 
//          warning-style QMessageBox.
//
// Created: 6/6/2011
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
#ifndef BLOBGUI_DIALOGBOXUTILS
#define BLOBGUI_DIALOGBOXUTILS

#include "blobExceptions/BlobException.h"
#include "blobExceptions/BlobParamsException.h"
#include "blobExceptions/BlobIOException.h"
#include "blobExceptions/FileNotFoundException.h"
#include "blobExceptions/BlobXmlReaderException.h"
#include "blobExceptions/BlobXmlWriterException.h"
#include "blobExceptions/UnknownXmlTagException.h"
#include <QString>
#include <vector>

namespace blobGui 
{
    /***********************************************************************//**
    This utility class knows how to extract info from any subclass 
    of BlobException and format the info for presentation in a modal 
    warning-style QMessageBox.

    The message box will display a warning icon next to its message and will 
    have an OK button.  The term &quot;modal&quot; as used here means that all 
    user input to the GUI is blocked until the user clicks on the OK button to
    close the dialog box.
    ***************************************************************************/
    class DialogBoxUtils
    {

    public:
        DialogBoxUtils();
        virtual ~DialogBoxUtils();

        void warnUser( blobExceptions::BlobException e );
        void warnUser( blobExceptions::UnknownXmlTagException e );
        void warnUser( blobExceptions::BlobParamsException e );
        void warnUser( blobExceptions::BlobIOException e );
        void warnUser( blobExceptions::FileNotFoundException e );
        void warnUser( blobExceptions::BlobXmlReaderException e );
        void warnUser( blobExceptions::BlobXmlWriterException e );
        void warnUser( std::vector<blobExceptions::UnknownXmlTagException> & errors );
        void warnUser( std::vector<blobExceptions::BlobParamsException> & errors );
        void warnUser( const QString & message );

    private:
        void openWarningMessageBox( QString title, QString message );
    };
}

#endif
