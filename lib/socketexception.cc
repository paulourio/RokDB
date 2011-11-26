/*
 * socketexception.cc
 *
 *  Created on: 26/11/2011
 */
#include "socketexception.h"
#include <cerrno>

extern int errno;

using namespace rokdb;

SocketException::SocketException(std::string description) :
	message_str(description), error(errno) {
	/* VOID */
};

std::string SocketException::description() const {
	return message_str;
}
