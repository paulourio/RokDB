/*
 * appmutex.cc
 *
 *  Created on: 26/11/2011
 */
#include <sstream>
#include <debug.h>
#include "appmutex.h"

#include <cstdio>
#include <cerrno>
#include <fcntl.h>

using namespace rokdb;

extern int errno;

const int UNLOCKED = -1;

AppMutex::AppMutex() : lock_file(""), lock_id(UNLOCKED) {
	/* VOID */
}

AppMutex::~AppMutex() {
	RemoveLock();
}

bool AppMutex::is_locked() const {
	return lock_id != UNLOCKED;
}

void AppMutex::RemoveLock() {
	if (is_locked()) {
		close(lock_id);
		remove(lock_file.c_str());
		lock_id = UNLOCKED;
	}
}

bool AppMutex::CreateMutex(std::string filename) {
	lock_file = filename;
	lock_id = open(lock_file.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0222);
	if (!is_locked()) {
		switch (errno) {
		case EEXIST:
			ShowBlockMessage();
			break;
		case EACCES:
			error("Permission denied to create mutex.");
			break;
		default:
			std::stringstream msg;
			msg << "Unkown error while trying to create mutex. (errno=";
			msg << errno << ")";
			error(msg.str());
		}
		return false;
	}
	std::stringstream msg;
	msg << "Lock file: " << lock_file;
	debug(4, msg.str());
	return true;
}

void AppMutex::ShowBlockMessage() {
	std::stringstream filestr;

	error("Cannot start server: application is blocked.");
	error("If you are sure there is no server running, remove manually ");
	filestr << "this file: " << lock_file;
	error(filestr.str());
}
