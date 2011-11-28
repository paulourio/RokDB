/*
 * main.c
 *
 *  Created on: 25/11/2011
 *
 *  This is an application test for ClientSocket.  In order
 *  to test, you have to get the server running.
 *  In a terminal, run:
 *  	bin $ ./RokDB
 *  The RokDB server will be running, then you can start the
 *  client application, in an other terminal:
 *  	bin $ ./rokdbclient
 */
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "clientsocket.h"

using namespace rokdb;

ClientSocket client_socket("localhost", 3666);

void ProcessCommand() {
	char buffer[1024];

	try {
		UnicodeString reply;
		UnicodeString message;
		char input[1024];
		try {
			std::cout << ">>> ";
			std::cin.getline(input, 1024);
			message = input;
			if (message.isEmpty())
				if (std::cin.eof())
					exit(-1);
				else
					return;
			client_socket << message;
			client_socket >> reply;
		} catch (SocketException&) {
		}

		reply.extract(0, 1024, buffer, 1024);
		std::cout << buffer<< "\n";
	} catch (SocketException& e) {
		std::cerr << "Exception was caught: " << e.description() << "\n";
	}
}

int main(int argc, char *argv[]) {
	while (true)
		ProcessCommand();
	return EXIT_SUCCESS;
}
