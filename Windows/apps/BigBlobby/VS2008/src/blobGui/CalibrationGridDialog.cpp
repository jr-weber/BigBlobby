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
#include "blobGui/CalibrationGridDialog.h"
#include "blobCalibration/BlobCalibration.h"
#include "blobCalibration/CalibrationUtils.h"

using blobGui::CalibrationGridDialog;

CalibrationGridDialog::CalibrationGridDialog(  blobCalibration::BlobCalibration * blobCal,
                                               QWidget * parent, 
                                               Qt::WindowFlags f ) :
  QDialog( parent, f ),
  ui_(),
  blobCalibration_( blobCal ),
  calibrationUtils_( blobCal->calibrationUtils() ),
  boundingBoxState_( MOVE_ENTIRE_BOX )
{
    ui_.setupUi( this );
    makeConnections();

    ui_.cancelButton->hide();
}

CalibrationGridDialog::~CalibrationGridDialog()
{
}

void CalibrationGridDialog::makeConnections()
{
    connectGridSizeSpinBoxes();
    connectBoundingBoxRadioButtons();
    connectBoundingBoxArrowButtons();
}

void CalibrationGridDialog::connectGridSizeSpinBoxes()
{
    connect( ui_.spinBox_pointsPerRow, SIGNAL( valueChanged( int ) ),
             this, SLOT( setNumPointsPerRow( int ) ) );

    connect( ui_.spinBox_numberOfRows, SIGNAL( valueChanged( int ) ),
             this, SLOT( setNumRows( int ) ) );
}

void CalibrationGridDialog::connectBoundingBoxRadioButtons()
{
    connect( ui_.radioButton_moveEntireBox, SIGNAL( toggled( bool ) ),
             this, SLOT( moveEntireBox( bool ) ) );

    connect( ui_.radioButton_moveLeftEdge, SIGNAL( toggled( bool ) ),
             this, SLOT( moveLeftEdge( bool ) ) );

    connect( ui_.radioButton_moveRightEdge, SIGNAL( toggled( bool ) ),
             this, SLOT( moveRightEdge( bool ) ) );

    connect( ui_.radioButton_moveTopEdge, SIGNAL( toggled( bool ) ),
             this, SLOT( moveTopEdge( bool ) ) );

    connect( ui_.radioButton_moveBottomEdge, SIGNAL( toggled( bool ) ),
             this, SLOT( moveBottomEdge( bool ) ) );
}

void CalibrationGridDialog::connectBoundingBoxArrowButtons()
{
    connect( ui_.pushButton_moveLeft, SIGNAL( pressed() ),
             this, SLOT( leftArrowPressed() ) );

    connect( ui_.pushButton_moveUp, SIGNAL( pressed() ),
             this, SLOT( upArrowPressed() ) );

    connect( ui_.pushButton_moveRight, SIGNAL( pressed() ),
             this, SLOT( rightArrowPressed() ) );

    connect( ui_.pushButton_moveDown, SIGNAL( pressed() ),
             this, SLOT( downArrowPressed() ) );
}

void CalibrationGridDialog::updateGridSizeSpinBoxes()
{
    ui_.spinBox_pointsPerRow->setValue( blobCalibration_->numPointsPerRow() );
    ui_.spinBox_numberOfRows->setValue( blobCalibration_->numRows() );
}

void CalibrationGridDialog::setNumPointsPerRow( int points )
{
    blobCalibration_->setNumPointsPerRow( points );
}

void CalibrationGridDialog::setNumRows( int rows )
{
    blobCalibration_->setNumRows( rows );
}

void CalibrationGridDialog::moveEntireBox( bool b )
{
    if( b ) {
        boundingBoxState_ = MOVE_ENTIRE_BOX;
        ui_.pushButton_moveLeft->setEnabled( true );
        ui_.pushButton_moveRight->setEnabled( true );
        ui_.pushButton_moveUp->setEnabled( true );
        ui_.pushButton_moveDown->setEnabled( true );
    }
}

void CalibrationGridDialog::moveLeftEdge( bool b )
{
    if( b ) {
        boundingBoxState_ = MOVE_LEFT_EDGE;
        ui_.pushButton_moveLeft->setEnabled( true );
        ui_.pushButton_moveRight->setEnabled( true );
        ui_.pushButton_moveUp->setEnabled( false );
        ui_.pushButton_moveDown->setEnabled( false );
    }
}

void CalibrationGridDialog::moveRightEdge( bool b )
{
    if( b ) {
        boundingBoxState_ = MOVE_RIGHT_EDGE;
        ui_.pushButton_moveLeft->setEnabled( true );
        ui_.pushButton_moveRight->setEnabled( true );
        ui_.pushButton_moveUp->setEnabled( false );
        ui_.pushButton_moveDown->setEnabled( false );
    }
}

void CalibrationGridDialog:: moveTopEdge( bool b )
{
    if( b ) {
        boundingBoxState_ = MOVE_TOP_EDGE;
        ui_.pushButton_moveLeft->setEnabled( false );
        ui_.pushButton_moveRight->setEnabled( false );
        ui_.pushButton_moveUp->setEnabled( true );
        ui_.pushButton_moveDown->setEnabled( true );
    }
}

void CalibrationGridDialog::moveBottomEdge( bool b )
{
    if( b ) {
        boundingBoxState_ = MOVE_BOTTOM_EDGE;
        ui_.pushButton_moveLeft->setEnabled( false );
        ui_.pushButton_moveRight->setEnabled( false );
        ui_.pushButton_moveUp->setEnabled( true );
        ui_.pushButton_moveDown->setEnabled( true );
    }
}

void CalibrationGridDialog::leftArrowPressed()
{
    switch( boundingBoxState_ ) {
        case MOVE_ENTIRE_BOX:  calibrationUtils_->moveBoundingBoxToTheLeft(); break;
        case MOVE_LEFT_EDGE:   calibrationUtils_->moveBoundingBoxLeftEdgeToTheLeft(); break;
        case MOVE_RIGHT_EDGE:  calibrationUtils_->moveBoundingBoxRightEdgeToTheLeft(); break;
    }
}

void CalibrationGridDialog::rightArrowPressed()
{
    switch( boundingBoxState_ ) {
        case MOVE_ENTIRE_BOX:  calibrationUtils_->moveBoundingBoxToTheRight(); break;
        case MOVE_LEFT_EDGE:   calibrationUtils_->moveBoundingBoxLeftEdgeToTheRight(); break;
        case MOVE_RIGHT_EDGE:  calibrationUtils_->moveBoundingBoxRightEdgeToTheRight(); break;
    }
}

void CalibrationGridDialog::upArrowPressed()
{
    switch( boundingBoxState_ ) {
        case MOVE_ENTIRE_BOX:  calibrationUtils_->moveBoundingBoxUpward(); break;
        case MOVE_TOP_EDGE:    calibrationUtils_->moveBoundingBoxUpperEdgeUpward(); break;
        case MOVE_BOTTOM_EDGE: calibrationUtils_->moveBoundingBoxLowerEdgeUpward(); break;
    }
}
void CalibrationGridDialog::downArrowPressed()
{
    switch( boundingBoxState_ ) {
        case MOVE_ENTIRE_BOX:  calibrationUtils_->moveBoundingBoxDownward(); break;
        case MOVE_TOP_EDGE:    calibrationUtils_->moveBoundingBoxUpperEdgeDownward(); break;
        case MOVE_BOTTOM_EDGE: calibrationUtils_->moveBoundingBoxLowerEdgeDownward(); break;
    }
}
