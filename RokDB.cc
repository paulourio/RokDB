#include <cstdio>
#include <cstdlib>
#include <csignal>

#include "rokdb.h"

using namespace rokdb;

extern RokDB core;

RokDB::RokDB() : config_file("bin/rokdb.conf") {
	signal(SIGTERM, (sighandler_t) &RokDB::SignalHandler);
	signal(SIGINT, (sighandler_t) &RokDB::SignalHandler);

	get_config().ReadFromFile(config_file);
	Lock();
}

RokDB::~RokDB() {

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
	exit(EXIT_FAILURE);
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
	server.Start();
}

RokServer RokDB::get_server() {
	return server;
}

Config RokDB::get_config() {
	return config;
}
