#include <iostream>
#include <string.h>

#include "ConnectedSocket.h"
#include "SocketMultiplexer.h"
#include "ServerSocket.h"
#include "FileReader.h"


ConnectedSocket::ConnectedSocket(int csFd, ServerSocket* ss) : SocketReader(csFd), SocketWriter(csFd), serverSocket(ss) {
}


ConnectedSocket::~ConnectedSocket() {
}


InetAddress& ConnectedSocket::getRemoteAddress() {
    return remoteAddress;
}


void ConnectedSocket::setRemoteAddress(const InetAddress& remoteAddress) {
    this->remoteAddress = remoteAddress;
}


void ConnectedSocket::send(const char* puffer, int size) {
    writePuffer.append(puffer, size);
    SocketMultiplexer::instance().getWriteManager().manageSocket(this);
}


void ConnectedSocket::send(const std::string& junk) {
    writePuffer += junk;
    SocketMultiplexer::instance().getWriteManager().manageSocket(this);
}


void ConnectedSocket::sendFile(const std::string& file) {
    FileReader::read(file,
                     [&] (char* data, int length) -> void {
                         if (length > 0) {
                             this->ConnectedSocket::send(data, length);
                         }
                     },
                     [&] (int err) -> void {
                         std::cout << "Error: " << strerror(err) << std::endl;
                         this->end();
                     });
}


void ConnectedSocket::end() {
    SocketMultiplexer::instance().getReadManager().unmanageSocket(this);
}


void ConnectedSocket::clearReadPuffer() {
    readPuffer.clear();
}
