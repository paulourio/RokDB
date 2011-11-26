/*
 * config.cc
 *
 *  Created on: 24/11/2011
 */
#include <debug.h>
#include <unicode/regex.h>
#include <config.h>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>

using namespace rokdb;

Config::Config() :
	port_regexp("^ *port *= *([\\d]+) *$"), debug_regexp(
			"^ *debug *= *([\\d]+) *$"), status(U_ZERO_ERROR), port(3666),
			debug_level(1) {
	/* VOID */
}

Config::~Config() {
}

int Config::get_port() {
	return port;
}

void Config::set_port(const int new_value) {
	port = new_value;
}

int Config::get_debug() {
	return debug_level;
}

void Config::set_debug(const int new_value) {
	debug_level = new_value;
}

bool Config::ReadFromFile(const std::string &file) {
	char str[255];

	std::fstream file_op(file.c_str(), std::ios::in);
	if (!file_op.is_open()) {
		error("Error: can't load config file.");
		return false;
	}
	while (!file_op.eof()) {
		file_op.getline(str, 255);
		ProcessLine(str);
	}
	file_op.close();
	return true;
}

void Config::ProcessLine(const std::string raw) {
	UnicodeString line;

	line += raw.c_str();
	line = line.trim();
	if (line.isEmpty())
		return;
	if (line.charAt(0) == '#')
		return;
	if (CommandPort(line))
		return;
	if (CommandDebug(line))
		return;
	std::stringstream message;
	message << "CONFIG: Unknown value \"" << raw << "\"";
	error(message.str());
}

RegexMatcher *Config::Match(const UnicodeString expression,
		const UnicodeString value) {
	RegexMatcher *matcher;

	matcher = new RegexMatcher(expression, UREGEX_CASE_INSENSITIVE, status);
	if (U_FAILURE(status))
		error("Regular expression failure (regexp).");
	matcher->reset(value);
	return matcher;
}

bool Config::CommandPort(const UnicodeString command) {
	RegexMatcher *matcher = Match(port_regexp, command);
	if (matcher->find()) {
		int value = 0;
		std::stringstream message("");

		if (ReadInt(matcher->group(1, status), &value)) {
			set_port(value);
			message << "CONFIG: Port updated to " << value;
			::debug(3, message.str());
			delete matcher;
			return true;
		}
	}
	delete matcher;
	return false;
}

bool Config::CommandDebug(const UnicodeString command) {
	RegexMatcher *matcher = Match(debug_regexp, command);
	if (matcher->find()) {
		int value = 0;
		std::stringstream message("");

		if (ReadInt(matcher->group(1, status), &value)) {
			set_debug(value);
			message << "CONFIG: Debug level updated to " << value;
			::debug(3, message.str());
			delete matcher;
			return true;
		}
	}
	delete matcher;
	return false;
}

/**
 * Read an integer value after "="
 */
bool Config::ReadInt(const UnicodeString content, int *value) {
	const int BUFFER_SIZE = 255;
	char buffer[BUFFER_SIZE + 1];

	memset(buffer, 0, BUFFER_SIZE + 1);
	content.extract(0, BUFFER_SIZE, (char *) &buffer, BUFFER_SIZE);
	return sscanf(buffer, "%d", value) == 1;
}
