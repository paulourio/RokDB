/*
 * protocolv1.cc
 *
 *  Created on: 26/11/2011
 */
#include <debug.h>
#include <sstream>
#include <string>
#include "protocolv1.h"
#include <unicode/regex.h>
#include <rokdb.h>
#include <utils.h>
#include <utility>

using namespace rokdb;

extern RokDB core;

ProtocolV1::ProtocolV1() :
	insert_callback(NULL), delete_callback(NULL), update_callback(NULL),
			create_callback(NULL), drop_callback(NULL),
			newdatabase_callback(NULL), destroydatabase_callback(NULL),
			select_callback(NULL) {
	UnicodeString newdatabase_regex("^ *GENESIS +(\\S+) *; *");
	UnicodeString destroydatabase_regex("^ *DESTROY +(\\S+) *; *");
	UnicodeString createtable_regex("^ *AT +(\\S+) +CREATE +(\\S+) +WITH +(.+) *; *$");
	UnicodeString droptable_regex("^ *AT +(\\S+) +DROP +(\\S+) *; *$");
	UnicodeString insert_regex("^ *AT +(\\S+) +IN +(\\S+) +INSERT +(.+) *; *$");
	UnicodeString update_regex("^ *AT +(\\S+) +IN +(\\S+) +SET +(.+) +WHERE +(.+) *; *$");
	UnicodeString remove_regex("^ *AT +(\\S+) +IN +(\\S+) +REMOVE +(.+) *; *$");
	UnicodeString select_regex("^ *AT +(\\S+) +IN +(\\S+) +SELECT +(.+)|(ALL) *; *$");
	UnicodeString http_regex("GET \\S+ HTTP");

	RegisterTrigger(insert_regex, (ProtocolTrigger) &ProtocolV1::CommandInsert);
	RegisterTrigger(newdatabase_regex, (ProtocolTrigger) &ProtocolV1::CommandNewDatabase);
	RegisterTrigger(destroydatabase_regex, (ProtocolTrigger) &ProtocolV1::CommandDestroyDatabase);
	RegisterTrigger(createtable_regex, (ProtocolTrigger) &ProtocolV1::CommandCreateTable);
	RegisterTrigger(droptable_regex, (ProtocolTrigger) &ProtocolV1::CommandDropTable);
	RegisterTrigger(select_regex, (ProtocolTrigger) &ProtocolV1::CommandSelect);
	RegisterTrigger(http_regex, (ProtocolTrigger) &ProtocolV1::CommandHTTP);
}

bool ProtocolV1::CommandInsert(RegexMatcher *matcher) {
	const UnicodeString insert_fields_regex("(\\S+)=\"(\\S+)\"[,;]? ?");
	struct CommandInsert info;
	UErrorCode status(U_ZERO_ERROR);

	if (core.get_parser().insert_callback == NULL)
		return false;
	info.database = matcher->group(1, status);
	info.table_name = matcher->group(2, status);

	UnicodeString param = ucopy(matcher->group(3, status));
	RegexMatcher *fields = Match(insert_fields_regex, param);
	if (fields != NULL) {
		while (fields->find())
			info.values.push_back(ucopy(fields->group(2, status)));
		delete fields;
	}
	debug(3, "Inserting");
	core.get_parser().insert_callback(&info);
	return true;
}

bool ProtocolV1::CommandNewDatabase(RegexMatcher *matcher) {
	UErrorCode status(U_ZERO_ERROR);
	struct CommandDatabase info;

	if (core.get_parser().newdatabase_callback == NULL)
		return false;
	info.database_name = ucopy(matcher->group(1, status));
	debug(3, "Creating database");
	core.get_parser().newdatabase_callback(&info);
	return true;
}

bool ProtocolV1::CommandDestroyDatabase(RegexMatcher *matcher) {
	UErrorCode status(U_ZERO_ERROR);
	struct CommandDatabase info;

	if (core.get_parser().destroydatabase_callback == NULL)
		return false;
	info.database_name = ucopy(matcher->group(1, status));
	debug(3, "Destroying database");
	core.get_parser().destroydatabase_callback(&info);
	return true;
}

bool ProtocolV1::CommandCreateTable(RegexMatcher *matcher) {
	const UnicodeString create_fields_regex("(\\S+) (\\S+)[,;]? ?");
	UErrorCode status(U_ZERO_ERROR);
	struct CommandCreate info;

	if (core.get_parser().create_callback == NULL)
		return false;
	info.database = ucopy(matcher->group(1, status));
	info.table_name = ucopy(matcher->group(2, status));

	UnicodeString param = ucopy(matcher->group(3, status));
	RegexMatcher *columns = Match(create_fields_regex, param);
	if (columns != NULL) {
		while (columns->find()) {
			UnicodeString name(ucopy(columns->group(1, status)));
			UnicodeString type(ucopy(columns->group(2, status)));

			info.columns.push_back(std::make_pair(name, type));
		}
		delete columns;
	}
	debug(3, "Creating table");
	core.get_parser().create_callback(&info);
	return true;
}

bool ProtocolV1::CommandDropTable(RegexMatcher *matcher) {
	UErrorCode status(U_ZERO_ERROR);
	struct CommandDrop info;

	if (core.get_parser().drop_callback == NULL)
		return false;
	info.database = ucopy(matcher->group(1, status));
	info.table_name = ucopy(matcher->group(2, status));
	debug(3, "Droping table");
	core.get_parser().drop_callback(&info);
	return true;
}

bool ProtocolV1::CommandSelect(RegexMatcher *matcher) {
	const UnicodeString select_fields_regex("(\\S+)=\"(\\S+)\"[,;]? ?");
	UErrorCode status(U_ZERO_ERROR);
	struct CommandSelect info;

	if (core.get_parser().select_callback == NULL)
		return false;
	info.database = ucopy(matcher->group(1, status));
	info.table_name = ucopy(matcher->group(2, status));
	info.all = !ucopy(matcher->group(4, status)).isEmpty();

	UnicodeString param = ucopy(matcher->group(3, status));
	if (!param.isEmpty()) {
		RegexMatcher *fields = Match(select_fields_regex, param);
		if (fields != NULL) {
			while (fields->find()) {
				UnicodeString column = ucopy(fields->group(1, status));
				UnicodeString value = ucopy(fields->group(2, status));
				info.conditions.push_back(std::make_pair(ucopy(column), ucopy(value)));
			}
			delete fields;
		}
	}
	debug(3, "Select command");
	core.get_parser().select_callback(&info);
	return true;
}

bool ProtocolV1::CommandHTTP(RegexMatcher *matcher) {
	core.AcquireLock();
	core.shouldClose = true;
	core.rawData = true;

	std::stringstream header;
	std::stringstream response;

	response << "<html><body>";
	response << "<h1>RokDB Server running on Linux, modafoca</h1><br/>";
	response << "<br/>Currently active connections: ";
	response << core.get_server().OpenConnections();
	response << "</body></html>\r\n";

	std::string sresp = response.str();
	int length = sresp.length();

	header << "HTTP/1.0 200 OK\r\nContent-Type: text/HTML\r\n";
	header << "Content-Length: " << length << "\r\n\r\n";

	core.responseBuffer = header.str().c_str();
	core.responseBuffer += response.str().c_str();
	return true;
}

void ProtocolV1::OnInsert(ProtocolEventInsert callback) {
	insert_callback = callback;
}

void ProtocolV1::OnDelete(ProtocolEventDelete callback) {
	delete_callback = callback;
}

void ProtocolV1::OnUpdate(ProtocolEventUpdate callback) {
	update_callback = callback;
}

void ProtocolV1::OnCreate(ProtocolEventCreate callback) {
	create_callback = callback;
}

void ProtocolV1::OnDrop(ProtocolEventDrop callback) {
	drop_callback = callback;
}

void ProtocolV1::OnNewDatabase(ProtocolEventDatabase callback) {
	newdatabase_callback = callback;
}

void ProtocolV1::OnDestroyDatabase(ProtocolEventDatabase callback) {
	destroydatabase_callback = callback;
}

void ProtocolV1::OnSelect(ProtocolEventSelect callback) {
	select_callback = callback;
}

int ProtocolV1::IdentifyColumnType(const UnicodeString &value) {
	UnicodeString lower = ucopy(value);
	lower.toLower();
	if (lower.compare(UNICODE_STRING_SIMPLE("int")))
		return 0;
	if (lower.compare(UNICODE_STRING_SIMPLE("char")))
		return 1;
	return 2;
}

