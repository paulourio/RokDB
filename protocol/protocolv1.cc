/*
 * protocolv1.cc
 *
 *  Created on: 26/11/2011
 */

#include "protocolv1.h"
#include <unicode/regex.h>

using namespace rokdb;

ProtocolV1::ProtocolV1() :
	insert_callback(NULL), delete_callback(NULL), update_callback(NULL),
			create_callback(NULL), use_callback(NULL) {
	const UnicodeString insert_regex(
			"^\\s*in\\s+(\\S+)\\s+insert\\s+(.+)\\s*;\\s*$");

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

void ProtocolV1::OnInsert(ProtocolEventInsert &callback) {
	insert_callback = callback;
}
void ProtocolV1::OnDelete(ProtocolEventDelete &callback) {
}
void ProtocolV1::OnUpdate(ProtocolEventUpdate &callback) {
}
void ProtocolV1::OnCreate(ProtocolEventCreate &callback) {
}
void ProtocolV1::OnUse(ProtocolEventUse &callback) {
}
void ProtocolV1::OnCommit(ProtocolEventCommit &callback) {
}
