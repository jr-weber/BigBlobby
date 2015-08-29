/*******************************************************************************
FileLogger

PURPOSE: This concrete implementation of the abstract Log class uses the 
         Log library of Boost to write log messages to a text file.

CREATED: 4/19/2015
*******************************************************************************/
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
#include "blobLogger/FileLogger.h"

// Disable deprecation warning for _vsnwprintf in boost/log/snprintf.hpp.
#pragma warning(push)
#pragma warning(disable: 4996)
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#pragma warning(pop)

using blobLogger::FileLogger;

const std::string FileLogger::LOG_FILE_NAME( "data/log/BigBlobby-Log_%N.log" ),
                  FileLogger::LOG_DIRECTORY( "data/log" ),
                  FileLogger::MESSAGE_FORMAT( "[%TimeStamp%] %Message%" );

const int FileLogger::MAX_FILE_SIZE = 100 * 1024,
          FileLogger::MAX_DIRECTORY_SIZE = 2 * 1024 * 1024,
          FileLogger::DEFAULT_SEVERITY_LEVEL = boost::log::trivial::warning;

/***************************************************************************//**
The constructor initializes the Boost Log library.
*******************************************************************************/
FileLogger::FileLogger()
{
    initialize();
}

FileLogger::~FileLogger()
{
}

/***************************************************************************//**
The Singleton Design Pattern is used here so that the Boost Log library code 
is only initialized once.
*******************************************************************************/
blobLogger::Log & FileLogger::instance()
{
    static FileLogger fileLoggerSingleton;
    return fileLoggerSingleton;
}

void FileLogger::initialize()
{
    setFileInfo();
    setFilter();
    addCommonAttributes();
}

void FileLogger::setFileInfo()
{
    boost::log::add_file_log
    (
        boost::log::keywords::file_name = LOG_FILE_NAME,
        boost::log::keywords::target = LOG_DIRECTORY,
        boost::log::keywords::rotation_size = MAX_FILE_SIZE,
        boost::log::keywords::max_size = MAX_DIRECTORY_SIZE,
        boost::log::keywords::format = MESSAGE_FORMAT
    );
}

void FileLogger::setFilter()
{
    boost::log::core::get()->set_filter
    ( 
        boost::log::trivial::severity >= DEFAULT_SEVERITY_LEVEL 
    );
}

void FileLogger::addCommonAttributes()
{
    boost::log::add_common_attributes();
}

void FileLogger::writeTrace( const std::string & message )
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > severityLogger;
    BOOST_LOG_SEV( severityLogger, boost::log::trivial::trace ) << message;
}

void FileLogger::writeDebug( const std::string & message )
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > severityLogger;
    BOOST_LOG_SEV( severityLogger, boost::log::trivial::debug ) << message;
}

void FileLogger::writeInfo( const std::string & message )
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > severityLogger;
    BOOST_LOG_SEV( severityLogger, boost::log::trivial::info ) << message;
}

void FileLogger::writeWarning( const std::string & message )
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > severityLogger;
    BOOST_LOG_SEV( severityLogger, boost::log::trivial::warning ) << message;
}

void FileLogger::writeError( const std::string & message )
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > severityLogger;
    BOOST_LOG_SEV( severityLogger, boost::log::trivial::error ) << message;
}

void FileLogger::writeFatal( const std::string & message )
{
    boost::log::sources::severity_logger< boost::log::trivial::severity_level > severityLogger;
    BOOST_LOG_SEV( severityLogger, boost::log::trivial::fatal ) << message;
}
