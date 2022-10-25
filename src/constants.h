#pragma once
#include <vector>
#include <unordered_map>

namespace CONSTS {
const char* TABLE_PASSENGERS		= "Passengers";
const char* TABLE_BAGGAGE			= "Baggage";
const char* TABLE_AIRPLANES			= "Airplanes";
const char* TABLE_HANGARS			= "Hangars";
const char* TABLE_EMPLOYEES			= "Employees";
const char* TABLE_EMPLOYEEADDRESS	= "EmployeeAddress";
const char* TABLE_FLIGHTS			= "Flights";
const char* TABLE_FLIGHTREGISTER	= "FlightRegister";

const std::vector<const char*> ALL_CREATE_TABLES;
const std::unordered_map<const char*, int> TABLE_COLUMN_COUNT;
} // namespace CONSTS