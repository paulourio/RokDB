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
	while (active) {
		UnicodeString data;

		*socket >> data;
		if (data.isEmpty())
			break;

		char buffer[::MAXRECV];
		memset(buffer, 0, sizeof(buffer));
		data.extract(0, data.length(), buffer, sizeof(buffer));
		std::cout << "Received: " << buffer << std::endl;
		std::cout.flush();

		if (core.get_parser().ProcessCommand(data)) {
			if (!core.rawData) {
				if (core.lastResult)
					*socket << "1\n";
				else
					*socket << "0\n";
			}
			core.rawData = false;
			if (!core.responseBuffer.isEmpty())
				*socket << core.responseBuffer;
			core.responseBuffer = UNICODE_STRING_SIMPLE("0\n");
			if (core.shouldClose) {
				core.shouldClose = false;
				core.FreeLock();
				break;
			}
			core.FreeLock();
		} else {
			*socket << "ERRO\n";
			*socket << "0\n";
		}
	}
	debug(1, "Exiting thread (connection closed)\n");
	pthread_exit(0);
}

void RokConnection::RequestStop() {
	active = false;
	delete socket;
}


