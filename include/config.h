/*
 * config.h
 *
 *  Created on: 24/11/2011
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <unicode/unistr.h>
#include <unicode/regex.h>

namespace rokdb {

/**
 * RokDB configuration.  It parses the config file, and all
 * set of configuration should be handled in here.
 */
class Config {
private:
	const UnicodeString port_regexp;
	const UnicodeString debug_regexp;
	const UnicodeString timeout_regexp;

	UErrorCode status;
	int port;
	int debug_level;

	void ProcessLine(const char *);

	bool CommandPort(const UnicodeString);
	bool CommandDebug(const UnicodeString);
	bool CommandTimeOut(const UnicodeString);

	RegexMatcher *Match(const UnicodeString, const UnicodeString);
	bool ReadInt(const UnicodeString, int *);
public:
	int get_port();
	void set_port(const int);

	int get_debug();
	void set_debug(const int);

	bool ReadFromFile(const std::string &);

	Config();
	~Config();
};

}

#endif /* CONFIG_H_ */
