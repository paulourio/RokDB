#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <iostream>

#include <access/rokaccess.h>
#include "rokdb.h"

using namespace rokdb;

extern RokDB core;

RokDB::RokDB() : config_file("rokdb.conf") {
	signal(SIGTERM, (sighandler_t) &RokDB::SignalHandler);
	signal(SIGINT, (sighandler_t) &RokDB::SignalHandler);

	get_config().ReadFromFile(config_file);
	pthread_mutex_init(&lock, NULL);
	Lock();

	parser.OnInsert((ProtocolEventInsert) &RokAccess::HandleInsert);
	parser.OnNewDatabase((ProtocolEventDatabase) &RokAccess::HandleNewDatabase);
	parser.OnDestroyDatabase((ProtocolEventDatabase) &RokAccess::HandleDestroyDatabase);
}

RokDB::~RokDB() {
	pthread_mutex_destroy(&lock);

}

void RokDB::Lock() {
	char current_directory[FILENAME_MAX];
	std::stringstream lock_file;

	if (getcwd(current_directory, sizeof(current_directory)) == NULL) {
		error("Cannot read current directory.");
	} else {
		lock_file << current_directory << "/" << "rokdb.lock";
		if (mutex.CreateMutex(lock_file.str()))
			return;
	}
}

void RokDB::SignalHandler(int signum) {
	if (signum == SIGTERM || signum == SIGINT) {
		debug(0, "\nInterrupt signal received.");
		core.get_server().Stop();
	} else {
		std::stringstream msg;
		msg << "Ignoring unhandled signal " << signum;
		debug(0, msg.str());
	}
}

/* Will be called if mutex is succesfully created. */
void RokDB::start(void) {
	if (mutex.is_locked())
		server.Start();
}

RokServer RokDB::get_server() {
	return server;
}

Config RokDB::get_config() {
	return config;
}

ProtocolV1 RokDB::get_parser() {
	return parser;
}

void RokDB::AcquireLock() {
	pthread_mutex_lock(&lock);
	std::cout << "Critical Section adquirido!" << std::endl;
}

void RokDB::FreeLock() {
	pthread_mutex_unlock(&lock);
	std::cout << "Critical Section finalizado" << std::endl;
}
