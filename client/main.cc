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
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "clientsocket.h"

using namespace rokdb;

ClientSocket client_socket("localhost", 3666);

void ProcessCommand() {
	char buffer[1024];

	try {
		UnicodeString reply_code, rows;
		UnicodeString message;
		char input[1024];
		try {
			std::cout << ">>> ";
			std::cin.getline(input, 1024);
			message = input;
			if (message.isEmpty()) {
				if (std::cin.eof()) {
					exit(-1);
				} else {
					return;
				}
			}
			client_socket << message;

			client_socket >> reply_code;
			reply_code.extract(0, 1024, buffer, 1024);
			std::cout << buffer;

			client_socket >> rows;
			rows.extract(0, 1024, buffer, 1024);
			std::cout << buffer;

			char row[20];
			int row_count;

			memset(row, 0, sizeof(row));
			rows.extract(0, rows.length(), row, 20);
			sscanf(row, "%d", &row_count);
			while (row_count-- > 0) {
				client_socket >> rows;
				rows.extract(0, 1024, buffer, 1024);
				std::cout << buffer<< "\n";
			}
		} catch (SocketException&) {
		}
		std::cout.flush();
	} catch (SocketException& e) {
		std::cerr << "Exception was caught: " << e.description() << "\n";
	}
}

int main(int argc, char *argv[]) {
	while (true)
		ProcessCommand();
	return EXIT_SUCCESS;
}
