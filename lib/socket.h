/*
 * socket.hpp
 *
 *  Created on: 23/11/2011
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <unicode/unistr.h>
#include <arpa/inet.h>
#include "socketexception.h"

namespace rokdb {

const int SOCKET_INVALID = -1;
const int SOCKET_ERROR = -1;
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 10;
const int MAXRECV = 1024;

/**
 * Abstraction class for C sockets.
 */
class Socket {
public:
	Socket();
	virtual ~Socket();

	// Server initialization
	bool create();
	bool bind(const int port);
	bool listen() const;
	bool accept(Socket&) const;

	// Client initialization
	bool connect(const std::string host, const int port);

	// Data Transimission
	bool send(const UnicodeString) const;
	int recv(UnicodeString&) const;

	void set_non_blocking(const bool);

	bool is_valid() const {
		return socket_pid != SOCKET_INVALID;
	}

	bool CheckSuccess(const bool value) const {
		return value != SOCKET_ERROR;
	}
private:
	int socket_pid;
	sockaddr_in socket_addr;
};

}

#endif /* SOCKET_HPP_ */
