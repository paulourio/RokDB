/*
 * protocolv1.h
 *
 *  Created on: 26/11/2011
 */

#ifndef PROTOCOLV1_H_
#define PROTOCOLV1_H_

#include <map>
#include <unicode/unistr.h>
#include "protocol.h"

namespace rokdb {

typedef std::map<UnicodeString, UnicodeString>  StringPair;

struct CommandCreate {
	UnicodeString table_name;
	StringPair columns; /* <column name, type> */
};

struct CommandInsert {
	UnicodeString table_name;
	StringPair columns; /* <column name, value> */
};

struct CommandDelete {
	UnicodeString table_name;
	StringPair conditions; /* <column, value> */
};

struct CommandUpdate {
	UnicodeString table_name;
	StringPair conditions; /* <column, value> */
	StringPair new_values; /* <column, value> */
};

struct CommandUse {
	UnicodeString database;
};

typedef void (*ProtocolEventInsert)(const struct CommandInsert *);
typedef void (*ProtocolEventDelete)(const struct CommandDelete *);
typedef void (*ProtocolEventUpdate)(const struct CommandUpdate *);
typedef void (*ProtocolEventCreate)(const struct CommandCreate *);
typedef void (*ProtocolEventUse)(const struct CommandUse *);
typedef void (*ProtocolEventCommit)();

class ProtocolV1: public Protocol {
public:
	ProtocolV1();
	~ProtocolV1();

	void OnInsert(ProtocolEventInsert);
	void OnDelete(ProtocolEventDelete);
	void OnUpdate(ProtocolEventUpdate);
	void OnCreate(ProtocolEventCreate);
	void OnUse(ProtocolEventUse);
	void OnCommit(ProtocolEventCommit);

	static void CommandInsert(RegexMatcher *);

	/* looool */
	ProtocolEventInsert insert_callback;
	ProtocolEventDelete delete_callback;
	ProtocolEventUpdate update_callback;
	ProtocolEventCreate create_callback;
	ProtocolEventUse use_callback;
	ProtocolEventCommit commit_callback;
};

}

#endif /* PROTOCOLV1_H_ */
