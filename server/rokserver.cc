/*
 * rokserver.cpp
 *
 *  Created on: 24/11/2011
 */
#include <cstdlib>
#include <csignal>
#include <algorithm>
#include <pthread.h>

#include <config.h>
#include <rokserver.h>
#include <rokdb.h>
#include <serversocket.h>
#include <rokconnection.h>

using namespace rokdb;

extern RokDB core;

/* Schedule thread execution */
void RokServer::Start() {
	void *ret_join;
	int ret;

	ret = pthread_create(&server_thread, NULL,
			(void* (*)(void*)) &RokServer::RunServer, NULL);
	if (ret != 0) {
		error("Error: Failed to create the server thread.");
		exit(EXIT_FAILURE);
	}
	ret = pthread_join(server_thread, &ret_join);
	if (ret != 0) {
		error("Error: Failed to join server thread.");
		exit(EXIT_FAILURE);
	}
}

/* Called when a signal is received. */
void RokServer::Stop() {
	active = false;
	CloseConnections();
}

void RokServer::Execute() {
	debug(0, "Running....\n");
	try {
		ServerSocket server(core.get_config().get_port());

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		while (active) {
			socket = new ServerSocket;
			struct ConnectionInfo *info;
			int ret;

			server.Accept(socket);
			info = new struct ConnectionInfo;
			info->connection = new RokConnection(socket);
			info->thread = new pthread_t;
			ret = pthread_create(info->thread, &attr,
					(void* (*)(void*)) &RokServer::NewConnection,
					(void *) info);
			if (ret != 0) {
				error("Error: Failed to create the connection thread.");
				exit(EXIT_FAILURE);
			}
			connections.push_back(info);
		}
	} catch (SocketException& e) {
		std::stringstream sstm;

		sstm << "Exception: " << e.description() << "\nExiting.\n";
		error(sstm.str());
		exit(EXIT_FAILURE);
	}
}

void RokServer::CloseConnections() {
	while (!connections.empty()) {
		struct ConnectionInfo *info = connections.front();
		connections.pop_front();
		info->connection->RequestStop();
		std::stringstream sstm;

		sstm << "Stopping thread " << info->thread;
		debug(2, sstm.str());
		pthread_cancel(*info->thread);
		FreeInfo(info);
	}
	if (!active) // TODO : Verify if this is really true
		pthread_cancel(server_thread);
}

void RokServer::FreeInfo(struct ConnectionInfo *info) {
	delete info->connection;
	delete info->thread;
	delete info;
}

/**
 * Method called inside a thread.
 */
void *RokServer::RunServer(void *arg) {
	core.get_server().Execute();
	return NULL;
}

void *RokServer::NewConnection(void *arg) {
	struct ConnectionInfo *info = (struct ConnectionInfo *) arg;

	info->connection->Listen();
	return NULL;
}

RokServer::RokServer() :
	active(true),
	server_thread(-1) {
}

RokServer::~RokServer() {
	CloseConnections();
	pthread_attr_destroy(&attr);
	delete socket;
}

void RokServer::set_active(bool value) {
	active = value;
}

std::list<struct ConnectionInfo *> RokServer::get_connections() {
	return connections;
}
