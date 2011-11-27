/*
 * protocol.h
 *
 *  Created on: 24/11/2011
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <map>
#include <list>
#include <unicode/unistr.h>
#include <unicode/regex.h>

namespace rokdb {

typedef void (* ProtocolTrigger)(RegexMatcher *);
typedef std::map<UnicodeString, ProtocolTrigger> TriggerMap;

class Protocol {
private:
protected:
	TriggerMap triggers;

	void RegisterTrigger(const UnicodeString &, ProtocolTrigger);
public:
	void ProcessCommand(UnicodeString);
	static RegexMatcher *Match(const UnicodeString expression,
			const UnicodeString value);
};

}

#endif /* PROTOCOL_H_ */
