////////////////////////////////////////////////////////////////////////////////
// CustomZoomDialog
//
// Purpose: Provides sliders and spin boxes to allow the user to select an
//          arbitrary zoom size for the main panel images (Source and Tracked)
//          or the filters panel images (Background, Smoothing, Highpass, or
//          Amplify).
//
// Created: 6/24/2013
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
#include "blobGui/CustomZoomDialog.h"

using blobGui::CustomZoomDialog;

/***************************************************************************//**
The constructor sets up and provides access to the Qt Designer GUI defined in
the CustomZoomDialog.ui file.
*******************************************************************************/
CustomZoomDialog::CustomZoomDialog( QWidget * parent /* = 0 */, 
                                    Qt::WindowFlags f /* = 0 */ ) :
  QWidget( parent, f ),
  ui_(),
  debug_( false )
{
    ui_.setupUi( this );
}

CustomZoomDialog::~CustomZoomDialog()
{
}

/***************************************************************************//**
Returns a reference to the Qt Designer GUI object defined in the 
CustomZoomDialog.ui file.  A reference to the CustomZoomDialogUi object will be
needed by the BlobSignalsToSlots object, which is where most GUI connections
are made.
*******************************************************************************/
Ui::CustomZoomDialogUi * CustomZoomDialog::customZoomDialogUi()
{
    return &ui_;
}

/***************************************************************************//**
Sets the zoom dialog box visible and ensures that its window is in front of any
other windows and has focus.  The first time the zoom dialog box is shown, it
will be centered in front of the main window.  If it is moved and then closed,
the new position will be remembered for the next time it is set visible.
*******************************************************************************/
void CustomZoomDialog::show()
{
    QWidget::show();
    activateWindow();
    raise();
}

/***************************************************************************//**
Hides the zoom dialog box.
*******************************************************************************/
void CustomZoomDialog::close()
{
    QWidget::close();
}

/***************************************************************************//**
Sets the zoom value displayed by the spin box and slider for the main panel 
images.  The signals from the QAction objects associated with the spin box and
slider will be blocked so that this function can be used to quietly synchronize
the values with View menu options.
*******************************************************************************/
void CustomZoomDialog::quietlySetZoomControlsForMainPanel( int percent )
{
    ui_.spinBox_zoom_mainPanelImages->blockSignals( true );
    ui_.slider_zoom_mainPanelImages->blockSignals( true );

    ui_.spinBox_zoom_mainPanelImages->setValue( percent );
    ui_.slider_zoom_mainPanelImages->setValue( percent );

    ui_.spinBox_zoom_mainPanelImages->blockSignals( false );
    ui_.slider_zoom_mainPanelImages->blockSignals( false );
}
