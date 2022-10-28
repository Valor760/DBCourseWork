#pragma once
#include <vector>
#include <unordered_map>


struct CONSTS {
	static const std::vector<std::string> ALL_CREATE_TABLES;
	static const std::unordered_map<std::string, int> TABLE_COLUMN_COUNT;
	static const std::vector<std::string> MENU_LABELS;

	// Table names
	static const std::string TABLE_PASSENGERS;
	static const std::string TABLE_BAGGAGE;
	static const std::string TABLE_AIRPLANES;
	static const std::string TABLE_HANGARS;
	static const std::string TABLE_EMPLOYEES;
	static const std::string TABLE_EMPLOYEEADDRESS;
	static const std::string TABLE_FLIGHTS;
	static const std::string TABLE_FLIGHTREGISTER;

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

		LABEL_NONE
	};
	static std::string ConvertLabel(const LABELS& label);
	static LABELS ConvertLabel(const std::string& label);
};