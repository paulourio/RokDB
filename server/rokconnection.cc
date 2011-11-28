/*
 * rokconnection.cpp
 *
 *  Created on: 24/11/2011
 */
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <serversocket.h>
#include <rokconnection.h>
#include <rokdb.h>

using namespace rokdb;

extern RokDB core;

RokConnection::RokConnection(ServerSocket *psocket) {
	debug(1, "Thread started (connection opened)");
	socket = psocket;
	active = true;
}

RokConnection::~RokConnection() {
}

void RokConnection::Listen() {
	debug(4, "Starting to listen");
	while (active) {
		UnicodeString data;

		*socket >> data;
		if (data.isEmpty())
			break;

		//UnicodeString toReturn(data);
		//*socket << "HTTP/1.0 200 OK\r\nContent-Type: text/HTML\r\nContent-Length: 77\r\n\r\n<html><body><h1>RokDB Server running on Linux, modafoca</h1></body></html>\r\n";// toReturn;
		char buffer[::MAXRECV];
		memset(buffer, 0, ::MAXRECV);
		data.extract(0, data.length() +1 , buffer, ::MAXRECV);
		std::cout << "Received: " << buffer << std::endl;
		std::cout.flush();

		if (core.get_parser().ProcessCommand(data)) {
			if (core.lastResult)
				*socket << "1\n";
			else
				*socket << "0\n";
			core.FreeLock();
		} else {
			*socket << "ERRO\n";
		}
	}
	debug(1, "Exiting thread (connection closed)\n");
	pthread_exit(0);
}

void RokConnection::RequestStop() {
	active = false;
	delete socket;
}


