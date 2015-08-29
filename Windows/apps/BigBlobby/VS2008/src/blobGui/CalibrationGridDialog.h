////////////////////////////////////////////////////////////////////////////////
// CalibrationGridDialog
//
// Purpose: Opens a dialog box for changing the size of the calibration grid and
//          for aligning the grid's bounding box.
//          
// Created: 3/8/2015
////////////////////////////////////////////////////////////////////////////////
/*
BigBlobby (GUI front end for Community Core Vision version 1.3)
Copyright (c) 2015 J.R.Weber <joe.weber77@gmail.com> (BigBlobby classes)

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
#ifndef BLOBGUI_CALIBRATIONGRIDDIALOG_H
#define BLOBGUI_CALIBRATIONGRIDDIALOG_H

#include "ui_CalibrationGridDialog.h"
#include <QDialog>

namespace blobCalibration { class BlobCalibration; }
class CalibrationUtils;

namespace blobGui 
{
    /***********************************************************************//**
    Opens a dialog box for changing the size of the calibration grid and 
    for aligning the grid's bounding box.
    ***************************************************************************/
    class CalibrationGridDialog : public QDialog
    {
        Q_OBJECT

    public:
        enum BoundingBoxState
        {
            MOVE_ENTIRE_BOX,
            MOVE_LEFT_EDGE,
            MOVE_RIGHT_EDGE,
            MOVE_TOP_EDGE,
            MOVE_BOTTOM_EDGE
        };

        CalibrationGridDialog( blobCalibration::BlobCalibration * blobCal,
                               QWidget * parent = 0, 
                               Qt::WindowFlags f = 0 );
        ~CalibrationGridDialog();

        void updateGridSizeSpinBoxes();

    private slots:
        void setNumPointsPerRow( int points );
        void setNumRows( int rows );

        void moveEntireBox( bool b );
        void moveLeftEdge( bool b );
        void moveRightEdge( bool b );
        void moveTopEdge( bool b );
        void moveBottomEdge( bool b );

        void leftArrowPressed();
        void rightArrowPressed();
        void upArrowPressed();
        void downArrowPressed();
        
    private:
        void makeConnections();
        void connectGridSizeSpinBoxes();
        void connectBoundingBoxRadioButtons();
        void connectBoundingBoxArrowButtons();

        Ui::CalibrationGridDialogUi ui_;
        blobCalibration::BlobCalibration * blobCalibration_;
        CalibrationUtils * calibrationUtils_;
        BoundingBoxState boundingBoxState_;
    };
}
#endif
