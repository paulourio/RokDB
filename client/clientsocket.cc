/*
 * client.cc
 *
 *  Created on: 25/11/2011
 */
#include <socket.h>
#include "clientsocket.h"

using namespace rokdb;

ClientSocket::ClientSocket(std::string host, int port) {
	if (!Socket::create())
		throw SocketException("Could not create client socket.");
	if (!Socket::connect(host, port))
		throw SocketException("Could not bind to port.");
}

const ClientSocket& ClientSocket::operator <<(const UnicodeString& s) const {
	if (!Socket::send(s))
		throw SocketException("Could not write to socket.");
	return *this;

}

const ClientSocket& ClientSocket::operator >>(UnicodeString& s) const {
	if (!Socket::recv(s))
		throw SocketException("Could not read from socket.");
	return *this;
}
