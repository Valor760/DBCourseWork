#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>


struct CONSTS {
	static const std::vector<std::string> ALL_CREATE_TABLES;
	// FIXME?: Probably no need in column count map
	// We can just query to DB and count how many tables we got
	// static const std::unordered_map<std::string, int> TABLE_COLUMN_COUNT;
	static const std::vector<std::string> MENU_LABELS;
	static const std::vector<std::string> TABLE_NAMES;

	// Table names
	enum TABLES {
		TABLE_PASSENGERS,
		TABLE_BAGGAGE, 
		TABLE_AIRPLANES,
		TABLE_HANGARS,
		TABLE_EMPLOYEES, 
		TABLE_EMPLOYEEADDRESS, 
		TABLE_FLIGHTS,
		TABLE_FLIGHTREGISTER
	};
	static std::string ConvertTableName(const TABLES& table);
	static TABLES ConvertTableName(const std::string& table);

	// Menu labels
	enum LABELS {
		LABEL_SHOW_TABLE,
		LABEL_INSERT_DATA,
		LABEL_QUERY_1,
		LABEL_QUERY_2,
		LABEL_QUERY_3,
		LABEL_QUERY_4,
		LABEL_QUERY_5,
		LABEL_QUERY_6, 
		LABEL_REMOVE_DATA,
	};
	static std::string ConvertLabelName(const LABELS& label);
	static LABELS ConvertLabelName(const std::string& label);
	static std::string TableInsertFormat(TABLES table_name);
};