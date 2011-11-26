/*
 * server.hpp
 *
 *  Created on: 23/11/2011
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <sstream>
#include <debug.h>
#include <lib/socket.h>

namespace rokdb {

/**
 * Implementation of Socket class, with some facilities,
 * for a server behaviour.
 */
class ServerSocket: private Socket {
public:
	ServerSocket(int port);
	ServerSocket();

	virtual ~ServerSocket();

	const ServerSocket& operator <<(const UnicodeString&) const;
	const ServerSocket& operator >>(UnicodeString&) const;

	void Accept(ServerSocket *);
};

}

#endif /* SERVER_HPP_ */
