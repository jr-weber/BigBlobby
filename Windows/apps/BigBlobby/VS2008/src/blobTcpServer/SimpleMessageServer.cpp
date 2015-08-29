////////////////////////////////////////////////////////////////////////////////
// SimpleMessageServer
//
// PURPOSE: TCP server that allows another program, the PlaysurfaceLauncher,
//          to send commands to BigBlobby (such as
//          BigBlobby:SaveSettingsToXmlFiles).
//
// CREATED: 6/30/2015
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
#include "blobTcpServer/SimpleMessageServer.h"
#include "blobGui/BlobMainWindow.h"
#include "blobLogger/FileLogger.h"
#include <QSettings>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QNetworkSession>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <stdlib.h>
#include <iostream>

using blobTcpServer::SimpleMessageServer;

const int SimpleMessageServer::DEFAULT_PORT_NUMBER = 3002;
const QString SimpleMessageServer::SAVE_SETTINGS_MESSAGE = "BigBlobby:SaveSettingsToXmlFiles",
              SimpleMessageServer::SUCCESS_MESSAGE = "BigBlobby:Success",
              SimpleMessageServer::FAILURE_MESSAGE = "BigBlobby:Failure";

SimpleMessageServer::SimpleMessageServer( blobGui::BlobMainWindow * mainWindow ) :
  mainWindow_( mainWindow ),
  networkSession_( NULL ),
  tcpServer_( NULL ), 
  portNumber_( DEFAULT_PORT_NUMBER ),
  log_( blobLogger::FileLogger::instance() )
{
}

SimpleMessageServer::~SimpleMessageServer()
{
    if( tcpServer_ != NULL ) {
        tcpServer_->close();
        delete tcpServer_;
    }
    delete networkSession_;
}

void SimpleMessageServer::setPortNumber( int port )
{
    portNumber_ = port;
}

int SimpleMessageServer::portNumber()
{
    return portNumber_;
}

void SimpleMessageServer::startServer()
{
    QNetworkConfigurationManager manager;

    if( manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired ) {
        //std::cout << "A network session is required.\n";
        // This platform requires a network session.
        networkSession_ = createNetworkSession( manager );
        connect( networkSession_, SIGNAL( opened()), this, SLOT( createTcpServer() ) );
        networkSession_->open();
    }
    else { // A network session is not required.
        //std::cout << "A network session is not required.\n";
        createTcpServer();
    }
}

QNetworkSession * SimpleMessageServer::createNetworkSession( QNetworkConfigurationManager & manager )
{
    // Get saved network configuration.
    QSettings settings( QSettings::UserScope, QLatin1String( "BigBlobby" ) );
    settings.beginGroup( QLatin1String( "QtNetwork" ) );
    const QString id = settings.value( QLatin1String( "DefaultNetworkConfiguration" ) ).toString();
    settings.endGroup();

    // If the saved network configuration is not currently discovered use the system default
    QNetworkConfiguration config = manager.configurationFromIdentifier( id );
    if( (config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered ) {
        config = manager.defaultConfiguration();
    }
    return new QNetworkSession( config, this );
}

void SimpleMessageServer::createTcpServer()
{
    saveNetworkSessionIfItExists();
    tcpServer_ = new QTcpServer( this );
    bool success = tcpServer_->listen( QHostAddress::LocalHost, portNumber_ );

    if( success ) {
        connect( tcpServer_, SIGNAL( newConnection() ), this, SLOT( newConnection() ) );
        logInfo( "Server running on localhost:" + QString::number( tcpServer_->serverPort() ) );
    }
    else {
        logWarning( "Unable to start server: " + tcpServer_->errorString() );
    }
}

void SimpleMessageServer::saveNetworkSessionIfItExists()
{
    if( networkSession_ != NULL ) {
        QNetworkConfiguration config = networkSession_->configuration();
        QString id;

        if(config.type() == QNetworkConfiguration::UserChoice) {
            id = networkSession_->sessionProperty( QLatin1String( "UserChoiceConfiguration" ) ).toString();
        }
        else {
            id = config.identifier();
        }
        QSettings settings( QSettings::UserScope, QLatin1String( "BigBlobby" ) );
        settings.beginGroup( QLatin1String( "QtNetwork" ) );
        settings.setValue( QLatin1String( "DefaultNetworkConfiguration" ), id );
        settings.endGroup();
    }
}

void SimpleMessageServer::newConnection()
{
    if( tcpServer_->hasPendingConnections() ) {
        QTcpSocket * socket = tcpServer_->nextPendingConnection();
        connect( socket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
        connect( socket, SIGNAL( disconnected() ), this, SLOT( disconnected() ) );
    }
}

void SimpleMessageServer::disconnected()
{
    QTcpSocket * socket = static_cast<QTcpSocket *>( sender() );
    socket->deleteLater();
}

void SimpleMessageServer::readyRead()
{
    QTcpSocket * socket = static_cast<QTcpSocket*>( sender() );
    QString clientMessage = readClientMessage( socket );
    QString serverMessage;

    if( clientMessage.compare( SAVE_SETTINGS_MESSAGE, Qt::CaseInsensitive ) == 0 ) {
        mainWindow_->saveGuiAndCameraSettingsToXmlFiles();
        serverMessage = SUCCESS_MESSAGE;
    }
    else {
        serverMessage = FAILURE_MESSAGE;
    }
    //std::cout << "SimpleMessageServer: " << serverMessage.toStdString() << "\n";
    QByteArray block;
    addMessageToBlock( block, serverMessage );
    socket->write( block );
}

QString SimpleMessageServer::readClientMessage( QTcpSocket * clientConnection )
{
    QString clientMessage;
    QDataStream in( clientConnection );
    in.setVersion( QDataStream::Qt_4_0 );
    int bytesAvailable = clientConnection->bytesAvailable();
    quint16 blockSize = 0;

    if( bytesAvailable < (int)sizeof(quint16) ) {
        std::cout << "SimpleMessageServer::readClientMessage(): 0 bytes in message.\n";
        return "";
    }
    in >> blockSize;
    
    if( bytesAvailable < blockSize ) {
        return "";
    }
    in >> clientMessage;
    return clientMessage;
}

void SimpleMessageServer::addMessageToBlock( QByteArray & block, const QString & message )
{
    QDataStream out( &block, QIODevice::ReadWrite );
    out.setVersion( QDataStream::Qt_4_0 );
    out << (quint16)0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof( quint16 ));
}

void SimpleMessageServer::openMessageBox( const QString & message )
{
    QMessageBox messageBox;
    messageBox.setWindowTitle( "Message from PlaysurfaceLauncher" );
    messageBox.setText( message );
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.exec();
}

void SimpleMessageServer::logWarning( const QString & message )
{
    log_.writeWarning( "SimpleMessageServer Warning: " + message.toStdString() );
}

void SimpleMessageServer::logInfo( const QString & message )
{
    log_.writeInfo( "SimpleMessageServer Info: " + message.toStdString() );
}
