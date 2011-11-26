/*
 * rokconnection.cpp
 *
 *  Created on: 24/11/2011
 */
#include <iostream>
#include <cstring>
#include <serversocket.h>
#include <rokconnection.h>

using namespace rokdb;

RokConnection::RokConnection(ServerSocket *psocket) {
	debug(1, "Connection opened.");
	socket = psocket;
	active = true;
}

RokConnection::~RokConnection() {
}

void RokConnection::Listen() {
	debug(2, "Starting to listen");
	while (active) {
		UnicodeString data;

		*socket >> data;
		if (data.isEmpty())
			continue;
		UnicodeString toReturn(data);
		*socket << toReturn;
		char buffer[::MAXRECV];
		memset(buffer, 0, ::MAXRECV);
		data.extract(0, data.length() +1 , buffer, ::MAXRECV);
		std::cout << "Received: " << buffer << std::endl;
		std::cout.flush();
	}
	debug(2, "Exiting thread.");
	pthread_exit(0);
}

void RokConnection::RequestStop() {
	active = false;
	delete socket;
}
