#pragma once
#include <vector>
#include <unordered_map>


struct CONSTS {
	static const std::vector<const char*> ALL_CREATE_TABLES;
	static const std::unordered_map<const char*, int> TABLE_COLUMN_COUNT;

	static const char* TABLE_PASSENGERS;
	static const char* TABLE_BAGGAGE;
	static const char* TABLE_AIRPLANES;
	static const char* TABLE_HANGARS;
	static const char* TABLE_EMPLOYEES;
	static const char* TABLE_EMPLOYEEADDRESS;
	static const char* TABLE_FLIGHTS;
	static const char* TABLE_FLIGHTREGISTER;
};