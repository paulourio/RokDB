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
	port_regexp("^\\s*port *=\\s*([\\d]+)\\s*$"),
	debug_regexp("^\\s*debug\\s*=\\s*([\\d]+)\\s*$"),
	timeout_regexp("^\\s*ConnectionTimeOut\\s*=\\s*([\\d]+)\\s*$"),
	status(U_ZERO_ERROR), port(3666), debug_level(5) {
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

int Config::get_timeout() {
	return timeout;
}

void Config::set_timeout(const int new_value) {
	timeout = new_value;
}

bool Config::ReadFromFile(const std::string &file) {
	char str[256];

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

void Config::ProcessLine(const char *rawline) {
	UnicodeString line;

	line += rawline;
	if (line.isEmpty() || line.startsWith('#'))
		return;
	if (CommandPort(line))
		return;
	if (CommandDebug(line))
		return;
	if (CommandTimeOut(line))
		return;
	std::stringstream message;
	char buffer[255];
	line.extract(0, 255, buffer, 255);
	message << "CONFIG: Unknown value \"" << buffer << "\"";
	error(message.str());
}

RegexMatcher *Config::Match(const UnicodeString expression,
		const UnicodeString value) {
	RegexMatcher *matcher;

	matcher = new RegexMatcher(expression, UREGEX_CASE_INSENSITIVE, status);
	if (U_FAILURE(status))
		error("Regular expression failure (regexp).");
	if (matcher != NULL)
		matcher->reset(value);
	return matcher;
}

bool Config::CommandPort(const UnicodeString command) {
	RegexMatcher *matcher = Match(port_regexp, command);
	bool result = false;

	if (matcher == NULL)
		return false;
	if (matcher->find()) {
		int value = 0;

		if (ReadInt(matcher->group(1, status), &value)) {
			std::stringstream message;

			set_port(value);
			message << "CONFIG: Port updated to " << value;
			::debug(3, message.str());
			result = true;
		}
	}
	delete matcher;
	return result;
}

bool Config::CommandDebug(const UnicodeString command) {
	RegexMatcher *matcher = Match(debug_regexp, command);
	bool result = false;

	if (matcher == NULL)
		return false;
	if (matcher->find()) {
		int value = 0;

		if (ReadInt(matcher->group(1, status), &value)) {
			std::ostringstream message;

			set_debug(value);

			message << "CONFIG: Debug level updated to " << value;
			::debug(3, message.str());
			result = true;
		}
	}
	delete matcher;
	return result;
}

bool Config::CommandTimeOut(const UnicodeString command) {
	RegexMatcher *matcher = Match(timeout_regexp, command);
	bool result = false;

	if (matcher == NULL)
		return false;
	if (matcher->find()) {
		int value = 0;

		if (ReadInt(matcher->group(1, status), &value)) {
			std::stringstream message;

			set_timeout(value);
			message << "CONFIG: Connection timeout updated to " << value << "s";
			::debug(3, message.str());
			result = true;
		}
	}
	delete matcher;
	return result;
}

/**
 * Read an integer value after "="
 */
bool Config::ReadInt(const UnicodeString content, int *value) {
	const int BUFFER_SIZE = 10;
	char buffer[BUFFER_SIZE + 1];

	memset(buffer, 0, BUFFER_SIZE + 1);
	content.extract(0, BUFFER_SIZE, (char *) &buffer, BUFFER_SIZE);
	return sscanf(buffer, "%d", value) == 1;
}
