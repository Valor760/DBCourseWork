#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>


struct CONSTS {
	// Table names
	enum TABLES {
		TABLE_PASSENGER,
		TABLE_BAGGAGE,
		TABLE_AIRPLANE,
		TABLE_HANGAR,
		TABLE_EMPLOYEE,
		TABLE_EMPLOYEEADDRESS,
		TABLE_FLIGHT,
		TABLE_FLIGHTREGISTER,
		TABLE_EMPLOYEEONBOARD
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
		LABEL_QUERY_7,
		LABEL_REMOVE_DATA,
	};
	static std::string ConvertLabelName(const LABELS& label);
	static LABELS ConvertLabelName(const std::string& label);
	static const std::vector<std::string> ALL_CREATE_TABLES;
	static const std::vector<std::string> MENU_LABELS;
	static const std::vector<std::string> TABLE_NAMES;
};