/*
 * protocolv1.cc
 *
 *  Created on: 26/11/2011
 */

#include "protocolv1.h"
#include <unicode/regex.h>

using namespace rokdb;

ProtocolV1::ProtocolV1() {
	const UnicodeString insert_regex("^in\\s+(\\S+)\\s+insert\\s+(.+)\\s*;\\s*$");

	RegisterTrigger(insert_regex, &CommandInsert);
}

void processar_campos(UnicodeString campos) {

	RegexMatcher matcher(campos_regex, UREGEX_CASE_INSENSITIVE, status);

	matcher.reset(campos);
	while (matcher.find()) {
		UnicodeString grupo = matcher.group(1, status);
		grupo.extract(0, 255, buffer, 255);
		cout << "Campo: " << buffer;

		grupo = matcher.group(2, status);
		grupo.extract(0, 255, buffer, 255);
		cout << "\tValor: " << buffer << endl;
	}
}

void ProtocolV1::CommandInsert(RegexMatcher *matcher) {
	const UnicodeString fields_regex("(\\S+)=\"(\\S+)\"[,;]?");
	struct CommandInsert info;
	UnicodeString fields;

	info.table_name = matcher.group(1, NULL);
	RegexMatcher *fields = Match(fields, matcher.group(2, NULL));
	while (matcher.find()) {
		UnicodeString field, value;

		field = matcher.group(1, NULL);
		value = matcher.group(2, NULL);
		info.columns.insert(make_pair(field, value));
	}
	delete fields;
	delete matcher;
}
