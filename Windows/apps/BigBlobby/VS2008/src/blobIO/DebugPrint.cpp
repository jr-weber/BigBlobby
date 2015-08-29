////////////////////////////////////////////////////////////////////////////////
// DebugPrint
//
// PURPOSE: Utility class for printing debugging messages to standard out.
//
// CREATED: 5/27/2011
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
#include "blobIO/DebugPrint.h"
#include <sstream>
#include <iostream>

using blobIO::DebugPrint;

/***************************************************************************//**
Writing to standard out is turned off by default.
*******************************************************************************/
DebugPrint::DebugPrint() :
  debug_( false )
{
}

/***************************************************************************//**
A constructor argument of true will result in print functions writing to
standard out.

The intent here is that a class that needs to print debugging to standard out 
should have its constructor create an instance of DebugPrint and just hard code
true or false temporarily.
*******************************************************************************/
DebugPrint::DebugPrint( bool b ) :
  debug_( b )
{
}

DebugPrint::~DebugPrint()
{
}

/***************************************************************************//**
At the time this comment was written, DebugPrint is only used by hard-coding
a true or false argument given to its constructor, but at a later time, this 
function could be useful for hooking up a keyboard shortcut or menu choice to
allow some debugging to be turned on/off while the program is up and running.
*******************************************************************************/
void DebugPrint::setDebuggingOn( bool b )
{
    debug_ = b;
}

/***************************************************************************//**
The print functions only write to standard out if debugging has been turned on
by giving either the constructor or setDebuggingOn() an argument of true.
*******************************************************************************/
bool DebugPrint::isDebuggingOn()
{
    return debug_;
}



/***************************************************************************//**
If the constructor or setDebuggingOn() has been given an argument of true, this
function writes the message to standard out and adds a newline to the end.
*******************************************************************************/
void DebugPrint::printLn( const std::string & message )
{
    if( debug_ ) {
        std::cout << message << "\n";
    }
}

/***************************************************************************//**
If the constructor or setDebuggingOn() has been given an argument of true, this
function writes the message to standard out (without adding a newline to the 
end).
*******************************************************************************/
void DebugPrint::print( const std::string & message )
{
    if( debug_ ) {
        std::cout << message;
    }
}

/***************************************************************************//**
Converts an int to a standard string.
*******************************************************************************/
std::string DebugPrint::intToStr( int n )
{
    std::stringstream out;
    out << n;
    return out.str();
}

/***************************************************************************//**
Converts a float to a standard string using stringstream.
*******************************************************************************/
std::string DebugPrint::floatToStr( float n )
{
    std::stringstream out;
    out << n;
    return out.str();
}

/***************************************************************************//**
Converts a double to a standard string using stringstream.
*******************************************************************************/
std::string DebugPrint::dblToStr( double n )
{
    std::stringstream out;
    out << n;
    return out.str();
}

/***************************************************************************//**
Converts a bool to a standard string (lowercase &quot;true&quot; or 
&quot;false&quot;).
*******************************************************************************/
std::string DebugPrint::boolToStr( bool b )
{
    return b ? "true" : "false";
}
