/*
 * protocol.h
 *
 *  Created on: 26/11/2011
 */
#include <protocol.h>

typedef void (* ProtocolTrigger)(RegexMatcher);

using namespace rokdb;

void Protocol::RegisterTrigger(UnicodeString expression,
		ProtocolTrigger callback) {
	triggers.insert(make_pair(expression, callback));
}

void Protocol::ProcessCommand(UnicodeString command) {
	TriggerMap::iterator it;
	RegexMatcher *matcher;

	for (it = triggers.begin(); it != triggers.end(); it++) {
		matcher = Match(it->first, command);
		if (matcher.find())
			triggers->second(matcher);
	}
}

RegexMatcher *Protocol::Match(const UnicodeString expression,
		const UnicodeString value) {
	RegexMatcher *matcher;

	matcher = new RegexMatcher(expression, UREGEX_CASE_INSENSITIVE, status);
	if (U_FAILURE(status))
		error("Regular expression failure (regexp).");
	matcher->reset(value);
	return matcher;
}
