/*
 * server.cpp
 *
 *  Created on: 23/11/2011
 */
#include <lib/socketexception.h>
#include <serversocket.h>

namespace rokdb {

ServerSocket::ServerSocket(const int port) {
	if (!Socket::create())
		throw SocketException("Could not create server socket.");

	if (!Socket::bind(port))
		throw SocketException("Could not bind to port.");

	if (!Socket::listen())
		throw SocketException("Could not listen to socket.");
}

ServerSocket::ServerSocket() {
}

const ServerSocket& ServerSocket::operator <<(const UnicodeString& str) const {
	if (!CheckSuccess(send(str)))
		throw SocketException("Could not write to socket.");
	return *this;
}

const ServerSocket& ServerSocket::operator >>(UnicodeString& str) const {
	if (!CheckSuccess(recv(str)))
		throw SocketException("Could not read from socket.");
	return *this;
}

void ServerSocket::Accept(ServerSocket *sock) {
	if (!Socket::accept(*sock))
		throw SocketException("Could not accept socket.");
}

ServerSocket::~ServerSocket() {

}

}
