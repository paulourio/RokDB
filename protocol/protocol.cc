/*
 * protocol.h
 *
 *  Created on: 26/11/2011
 */
#include <utility> /* make_pair */

#include <debug.h>
#include "protocol.h"

typedef void (* ProtocolTrigger)(RegexMatcher);

using namespace rokdb;

void Protocol::RegisterTrigger(const UnicodeString &expression,
		ProtocolTrigger callback) {
	triggers.insert(std::make_pair(expression, callback));
}

bool Protocol::ProcessCommand(UnicodeString command) {
	TriggerMap::iterator it;
	RegexMatcher *matcher;
	bool success = false;

	for (it = triggers.begin(); it != triggers.end(); it++) {
		matcher = Match(it->first, command);
		if (matcher != NULL) {
			if (matcher->find())
				if (it->second(matcher))
					success = true;
			delete matcher;
		}
	}
	return success;
}

RegexMatcher *Protocol::Match(const UnicodeString expression,
		const UnicodeString value) {
	UErrorCode status(U_ZERO_ERROR);
	RegexMatcher *matcher;

	matcher = new RegexMatcher(expression, UREGEX_CASE_INSENSITIVE, status);
	if (U_FAILURE(status))
		error("Regular expression failure (regexp).");
	if (matcher != NULL)
		matcher->reset(value);
	return matcher;
}
