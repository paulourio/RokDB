/*
 * socket.cpp
 *
 *  Created on: 23/11/2011
 */
#include <debug.h>
#include <socket.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <fcntl.h>

using namespace rokdb;

Socket::Socket() :
	socket_pid(::SOCKET_INVALID) {
	memset(&socket_addr, 0, sizeof(socket_addr));
}

Socket::~Socket() {
	if (is_valid()) {
		debug(1, "Closing socket.");
		::close(socket_pid);
	}
}

bool Socket::create() {
	socket_pid = socket(AF_INET, SOCK_STREAM, 0);

	if (!is_valid())
		return false;

	int on = 1;
	if (setsockopt(socket_pid, SOL_SOCKET, SO_REUSEADDR, (const char *) &on,
			sizeof(on)) == SOCKET_ERROR)
		return false;

	return true;
}

bool Socket::bind(const int port) {
	if (!is_valid())
		return false;

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_addr.s_addr = INADDR_ANY;
	socket_addr.sin_port = htons(port);
	memset(&socket_addr.sin_zero, 0, 8);

	int bind_return = ::bind(socket_pid, (struct sockaddr *) &socket_addr,
			sizeof(socket_addr));

	return CheckSuccess(bind_return);
}

bool Socket::listen() const {
	if (!is_valid())
		return false;

	int listen_return = ::listen(socket_pid, MAXCONNECTIONS);

	return CheckSuccess(listen_return);
}

bool Socket::accept(Socket& new_socket) const {
	socklen_t addr_length = sizeof(socket_addr);

	new_socket.socket_pid = ::accept(socket_pid, (sockaddr *) &socket_addr,
			&addr_length);
	return CheckSuccess(new_socket.socket_pid);
}

bool Socket::send(const UnicodeString s) const {
	int size = (s.length() + 1) * sizeof(UChar);
	int status;
	char buffer[size];

	s.extract(0, size, buffer, size);
	status = ::send(socket_pid, buffer, size, MSG_NOSIGNAL);
	switch (errno) {
	case EBADF:
		error("Error: send() returned EBADF (Bad file number).");
		break;
	case 0:
		break;
	default:
		std::stringstream message;
		message << "Error: send() returned errno=" << errno;
		error(message.str());
	}
	return CheckSuccess(status);
}

int Socket::recv(UnicodeString& str) const {
	char buffer[MAXRECV + 1];

	memset(buffer, 0, MAXRECV + 1);
	int status = ::recv(socket_pid, buffer, MAXRECV, 0);
	if (!CheckSuccess(status))
		std::cerr << "Socket error on read (errno: " << errno << ")\n";
	else if (status > 0)
		str = buffer;
	return status;
}

bool Socket::connect(const std::string host, const int port) {
	if (!is_valid())
		return false;

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(port);

	int status = inet_pton(AF_INET, host.c_str(), &socket_addr.sin_addr);

	if (errno == EAFNOSUPPORT)
		return false;

	status = ::connect(socket_pid, (sockaddr *) &socket_addr,
			sizeof(socket_addr));
	return status == 0;
}

/**
 * Set wheter the socket should be blocking (wait to return
 * until read the last byte in stream).
 */
void Socket::set_non_blocking(const bool block) {
	int flags;

	flags = fcntl(socket_pid, F_GETFL);
	if (CheckSuccess(flags))
		return;
	if (block)
		flags = (flags | O_NONBLOCK);
	else
		flags = (flags & ~O_NONBLOCK);
	fcntl(socket_pid, F_SETFL, flags);
}
