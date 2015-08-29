////////////////////////////////////////////////////////////////////////////////
// BlobXmlSettings
//
// PURPOSE: Reads/writes XML files for loading/storing the program settings.
//
// CREATED:  6/26/2013
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
#ifndef BLOBGUI_BLOBXMLSETTINGS_H
#define BLOBGUI_BLOBXMLSETTINGS_H

#include "blobIO/DebugPrint.h"
#include <QString>
#include <QActionGroup>

namespace blobGui { class BlobMainWindow; }
namespace Ui { class BlobMainWindowUi; }
namespace blobGui { class SourceGLWidget; }
namespace blobIO { class BlobParamsXmlReader; }
namespace blobIO { class BlobParamsXmlWriter; }
namespace blobIO { class BlobParamsValidator; }
namespace blobGui { class DialogBoxUtils; }

namespace blobGui 
{
    /***********************************************************************//**
    Reads/writes XML files for loading/storing the program settings.  
    
    In addition to holding a BlobParamsXmlReader and BlobParamsXmlWriter, this 
    class also has a DialogBoxUtils object that it will use to report errors 
    that occur while reading or writing XML.
    ***************************************************************************/
    class BlobXmlSettings : public QObject
    {
        Q_OBJECT

    public:
        static const QString DEFAULT_CONFIG_FILE,
                             FACTORY_CONFIG_FILE;

        BlobXmlSettings( blobGui::BlobMainWindow * blobMainWindow );
        ~BlobXmlSettings();

        void saveFactorySettingsToXmlFile();

    public slots:
        void loadDefaultSettingsAndSaveAsStartupSettings();
        void saveDefaultSettings();
        void restoreStartupSettings();
        void loadFactorySettings();

    private:
        blobIO::BlobParamsValidator * createValidator( const QString & filename );
        void saveCurrentSettingsAsXml( const QString & filename );

        void quietlyUpdateGui( blobIO::BlobParamsValidator * validator );
        void quietlyUpdateMenuBar( blobIO::BlobParamsValidator * validator );
        void quietlyUpdateCameraMenu( blobIO::BlobParamsValidator * validator );
        void quietlyUpdateBlobsMenu( blobIO::BlobParamsValidator * validator );
        void quietlyUpdateNetworkMenu( blobIO::BlobParamsValidator * validator );
        void quietlyUpdateExperimentalMenu( blobIO::BlobParamsValidator * validator );
        void setNetworkMenuTuioUdpProfile( blobIO::BlobParamsValidator * validator );
        void setNetworkMenuFlashXmlProfile( blobIO::BlobParamsValidator * validator );
        void quietlyUpdateMainPanel( blobIO::BlobParamsValidator * validator );
        void updateFiltersPanel( blobIO::BlobParamsValidator * validator );
        void updateBackgroundControls( blobIO::BlobParamsValidator * validator );

        void debugPrint( blobIO::BlobParamsValidator * validator );

        blobGui::BlobMainWindow * blobMainWindow_;
        Ui::BlobMainWindowUi * mainWindowUi_;
        blobGui::SourceGLWidget * sourceGLWidget_;
        blobIO::BlobParamsXmlReader * blobParamsXmlReader_;
        blobIO::BlobParamsXmlWriter * blobParamsXmlWriter_;
        blobGui::DialogBoxUtils * dialogBoxUtils_;
        blobIO::BlobParamsValidator * startupParamsValidator_;

        // Debugging Utility that Writes to Standard Out
        blobIO::DebugPrint debug_;
    };
}

#endif
