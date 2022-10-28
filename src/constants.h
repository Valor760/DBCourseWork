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
	static const std::string LABEL_SHOW_TABLE;
	static const std::string LABEL_INSERT_DATA;
	static const std::string LABEL_QUERY_1;
	static const std::string LABEL_QUERY_2;
	static const std::string LABEL_QUERY_3;
	static const std::string LABEL_QUERY_4;
	static const std::string LABEL_QUERY_5;
	static const std::string LABEL_QUERY_6;
	static const std::string LABEL_REMOVE_DATA;

	
};