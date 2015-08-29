////////////////////////////////////////////////////////////////////////////////
// CalibrationGLWidget
//
// PURPOSE: Full screen QGLWidget for drawing the CCV calibration screen.
//
// CREATED: 5/25/2011
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
#include "blobGui/SourceGLWidget.h"
#include "blobGui/CalibrationGLWidget.h"
#include "blobGui/CalibrationGridDialog.h"
#include "blobCore/CoreVision.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

using blobGui::CalibrationGLWidget;
using blobGui::SourceGLWidget;
using blobCore::CoreVision;
using blobEnums::KeyEnum;

const QColor CalibrationGLWidget::BACKGROUND_COLOR = QColor( 0, 255, 0 );

/***************************************************************************//**
The constructor holds on to a reference to the CoreVision object (the other
args are passed to the superclass, QGLWidget).
*******************************************************************************/
CalibrationGLWidget::CalibrationGLWidget( QWidget * parent,
                                          SourceGLWidget * sourceGLWidget,
                                          CoreVision * coreVision ) :
  QGLWidget( parent, sourceGLWidget ),
  coreVision_( coreVision ),
  calibrationGridDialog_( NULL ),
  debug_( false )
{
    blobCalibration::BlobCalibration * blobCal = coreVision_->blobCalibrationObj();
    calibrationGridDialog_ = new blobGui::CalibrationGridDialog( blobCal, this ); 
}

CalibrationGLWidget::~CalibrationGLWidget()
{
    delete calibrationGridDialog_;
}

/***************************************************************************//**
Calls on updateGL(), which will set up the OpenGL context and then call on 
paintGL().  

This function does not block.
*******************************************************************************/
void CalibrationGLWidget::frame( double deltaFrameTime )
{
    //std::cout << "deltaFrameTime = " << deltaFrameTime << " seconds\n";
    updateGL();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                              Protected Functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/***************************************************************************//**
Sets the background color for the OpenGL drawing surface.
*******************************************************************************/
void CalibrationGLWidget::initializeGL()
{
    qglClearColor( BACKGROUND_COLOR );

    // SourceGLWidget does XML file reading and setup of the
    // BlobCalibration object, so do not call on setup here.
    //coreVision_->setUpBlobCalibration();
}

/***************************************************************************//**
Sets the OpenGL viewport.
*******************************************************************************/
void CalibrationGLWidget::resizeGL( int width, int height )
{
    //debugPrintResizeGL( width, height );
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, (GLint)width, 0, (GLint)height, -1.0, 1.0 );
    glMatrixMode( GL_MODELVIEW );
}

/***************************************************************************//**
Calls on the CoreVision object to do the rendering and calibration.
*******************************************************************************/
void CalibrationGLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
    coreVision_->drawCalibration( QGLWidget::width(), QGLWidget::height() );
}

/***************************************************************************//**
Passes key press events on to the CoreVision object while converting each Qt
key to a blobsEnums::KeyEnum.  The conversion is needed because the CoreVision 
object does not know about Qt classes or enums.
*******************************************************************************/
void CalibrationGLWidget::keyPressEvent( QKeyEvent * keyEvent )
{
    debug_.printLn( "CalibrationGLWidget::keyPressEvent() called..." );

    switch( keyEvent->key() ) {
        case Qt::Key_G:  openCalibrationGridDialog();                break;
        case Qt::Key_B:  coreVision_->learnBackground();             break;
        case Qt::Key_C:  coreVision_->keyPressed( KeyEnum::KEY_c );  break;
        case Qt::Key_F:  coreVision_->keyPressed( KeyEnum::KEY_f );  break;
        case Qt::Key_R:  coreVision_->keyPressed( KeyEnum::KEY_r );  break;
        case Qt::Key_T:  coreVision_->keyPressed( KeyEnum::KEY_t );  break;

        case Qt::Key_A:  coreVision_->keyPressed( KeyEnum::KEY_a );  break;
        case Qt::Key_D:  coreVision_->keyPressed( KeyEnum::KEY_d );  break;
        case Qt::Key_W:  coreVision_->keyPressed( KeyEnum::KEY_w );  break;
        case Qt::Key_S:  coreVision_->keyPressed( KeyEnum::KEY_s );  break;

        case Qt::Key_Right:  coreVision_->keyPressed( KeyEnum::KEY_ARROW_RIGHT );
                             if( keyEvent->isAutoRepeat() ) { QApplication::processEvents(); }
                             break;
        case Qt::Key_Left:   coreVision_->keyPressed( KeyEnum::KEY_ARROW_LEFT );
                             if( keyEvent->isAutoRepeat() ) { QApplication::processEvents(); }
                             break;
        case Qt::Key_Down:   coreVision_->keyPressed( KeyEnum::KEY_ARROW_DOWN );
                             if( keyEvent->isAutoRepeat() ) { QApplication::processEvents(); }
                              break;
        case Qt::Key_Up:     coreVision_->keyPressed( KeyEnum::KEY_ARROW_UP );
                             if( keyEvent->isAutoRepeat() ) { QApplication::processEvents(); }
                             break;
        case Qt::Key_Equal:  processKeyEqual( keyEvent );  break;
        case Qt::Key_Minus:  processKeyMinus( keyEvent );  break;
        case Qt::Key_Plus:   processKeyPlus( keyEvent );   break;
        default:             QGLWidget::keyPressEvent( keyEvent );
    }
}

void CalibrationGLWidget::processKeyEqual( QKeyEvent * keyEvent )
{
    if( keyEvent->modifiers() & Qt::ShiftModifier ) {
        //std::cout << "CalibrationGLWidget(Qt::Key_Equal + Qt::ShiftModifier) maps to KeyEnum::KEY_PLUS\n";
        coreVision_->keyPressed( KeyEnum::KEY_PLUS );
    }
    else {
        //std::cout << "CalibrationGLWidget(Key_Equal) maps to KeyEnum::KEY_EQUAL_SIGN\n";
        coreVision_->keyPressed( KeyEnum::KEY_EQUAL_SIGN );
    }
}

void CalibrationGLWidget::processKeyMinus( QKeyEvent * keyEvent )
{
    if( keyEvent->modifiers() & Qt::ShiftModifier ) {
        //std::cout << "CalibrationGLWidget(Qt::Key_Minus + Qt::ShiftModifier) maps to KeyEnum::KEY_UNDERSCORE\n";
        coreVision_->keyPressed( KeyEnum::KEY_UNDERSCORE );
    }
    else {
        //std::cout << "CalibrationGLWidget(Qt::Key_Minus) maps to KeyEnum::KEY_MINUS\n";
        coreVision_->keyPressed( KeyEnum::KEY_MINUS );
    }
}

void CalibrationGLWidget::processKeyPlus( QKeyEvent * keyEvent )
{
    if( keyEvent->modifiers() & Qt::ShiftModifier ) {
        //std::cout << "CalibrationGLWidget(Qt::Key_Plus + Qt::ShiftModifier) maps to KeyEnum::KEY_PLUS\n";
        coreVision_->keyPressed( KeyEnum::KEY_PLUS );
    }
    else {
        //std::cout << "CalibrationGLWidget(Qt::Key_Plus) maps to KeyEnum::KEY_EQUAL_SIGN\n";
        coreVision_->keyPressed( KeyEnum::KEY_EQUAL_SIGN );
    }
}

/***************************************************************************//**
Passes key release events on to the CoreVision object while converting each Qt
key to a blobsEnums::KeyEnum.  The conversion is needed because the CoreVision 
object does not know about Qt classes or enums.
*******************************************************************************/
void CalibrationGLWidget::keyReleaseEvent( QKeyEvent * event )
{
    debug_.printLn( "CalibrationGLWidget::keyReleaseEvent() called..." );

    switch( event->key() ) {
        case Qt::Key_W: coreVision_->keyReleased( KeyEnum::KEY_w );  break;
        case Qt::Key_S: coreVision_->keyReleased( KeyEnum::KEY_s );  break;
        case Qt::Key_A: coreVision_->keyReleased( KeyEnum::KEY_a );  break;
        case Qt::Key_D: coreVision_->keyReleased( KeyEnum::KEY_d );  break;
        case Qt::Key_X: coreVision_->keyReleased( KeyEnum::KEY_x );  
                        emit closeCalibrationScreen();
                        break;

        case Qt::Key_Left:   coreVision_->keyReleased( KeyEnum::KEY_ARROW_LEFT );   break;
        case Qt::Key_Right:  coreVision_->keyReleased( KeyEnum::KEY_ARROW_RIGHT );  break;
        case Qt::Key_Up:     coreVision_->keyReleased( KeyEnum::KEY_ARROW_UP );     break;
        case Qt::Key_Down:   coreVision_->keyReleased( KeyEnum::KEY_ARROW_DOWN );   break;

        default:             QGLWidget::keyPressEvent( event );
    } 
}

void CalibrationGLWidget::openCalibrationGridDialog()
{
    calibrationGridDialog_->updateGridSizeSpinBoxes();
    int status = calibrationGridDialog_->exec();
    //std::cout << "() status = " << status << "\n";
}

/*******************************************************************************
If debugging is turned on for this class, writes the width and height to 
standard out.
*******************************************************************************/
void CalibrationGLWidget::debugPrintResizeGL( int width, int height )
{
    debug_.printLn( "***** resizeGL(" 
                  + debug_.intToStr( width )
                  + ", "
                  + debug_.intToStr( height )
                  + ") *****\n" );
}
