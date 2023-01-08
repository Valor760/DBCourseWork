#include "constants.h"


const std::string create_table_passengers =
	"CREATE TABLE IF NOT EXISTS Passenger ("
	"P_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"P_Name TEXT NOT NULL,"
	"P_Surname TEXT NOT NULL,"
	"P_Gender TEXT,"
	"P_Phone_Num TEXT,"
	"CHECK(P_Gender IN (\"M\", \"F\"))"
	");"
;

const std::string create_table_baggage =
	"CREATE TABLE IF NOT EXISTS Baggage ("
	"B_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"B_Weight_KG REAL NOT NULL,"
	"B_IsFragile BOOLEAN NOT NULL,"
	"P_ID INTEGER NOT NULL,"
	
	"CHECK(B_Weight_KG < 10),"
	"FOREIGN KEY (P_ID) REFERENCES Passenger(P_ID)"
	");"
;

const std::string create_table_airplanes = 
	"CREATE TABLE IF NOT EXISTS Airplane ("
	"Plane_ID TEXT PRIMARY KEY,"
	"Plane_Capacity INTEGER NOT NULL,"
	"Plane_Type TEXT NOT NULL,"
	"Plane_Company TEXT,"

	"CHECK(length(Plane_ID) = 10),"
	"CHECK(Plane_Capacity IN (10, 50, 95)),"
	"CHECK(length(Plane_Type) = 3),"
	"CHECK((Plane_Capacity = 10 AND Plane_Type = \"PRV\") OR (Plane_Capacity = 50 AND Plane_Type = \"MID\") OR (Plane_Capacity = 95 AND Plane_Type = \"REG\"))"
	");"
;

const std::string create_table_hangars =
	"CREATE TABLE IF NOT EXISTS Hangar ("
	"H_ID TEXT PRIMARY KEY,"
	"H_Size INTEGER NOT NULL,"
	"Plane_ID TEXT,"
	
	"CHECK(length(H_ID) = 3 AND length(Plane_ID) = 10),"
	"CHECK(H_Size in (10, 50, 95)),"
	"FOREIGN KEY (Plane_ID) REFERENCES Airplane(Plane_ID)"
	");"
;

const std::string create_table_employees = 
	"CREATE TABLE IF NOT EXISTS Employee ("
	"Emp_ID TEXT PRIMARY KEY,"
	"Emp_Name TEXT NOT NULL,"
	"Emp_Surname TEXT NOT NULL,"
	"Emp_Gender TEXT,"
	"Emp_Phone_Num TEXT,"
	"Emp_Position TEXT NOT NULL,"
	"Emp_Salary REAL NOT NULL,"
	"Emp_Experience_Years REAL NOT NULL,"
	"Emp_Degree TEXT,"
	"H_ID TEXT,"
	
	"CHECK(length(Emp_ID) = 4),"
	"CHECK(H_ID IS NULL OR (length(H_ID) = 3 AND H_ID IS NOT NULL)),"
	"CHECK(Emp_Position IN (\"Pilot\", \"Stewardess\", \"Technician\", \"Cashier\", \"Worker\")),"
	"CHECK(Emp_Gender IN (\"M\", \"F\")),"
	"FOREIGN KEY (H_ID) REFERENCES Hangar(H_ID)"
	");"
;

const std::string create_table_employeeaddress = 
	"CREATE TABLE IF NOT EXISTS EmployeeAddress ("
	"Emp_ID TEXT PRIMARY KEY,"
	"Ad_City TEXT,"
	"Ad_Street_Name TEXT NOT NULL,"
	"Ad_House_Nr INTEGER NOT NULL,"
	"Ad_Appart_Nr INTEGER,"
	"Ad_Post_Code TEXT,"
	
	"CHECK(length(Emp_ID) = 4),"
	"FOREIGN KEY (Emp_ID) REFERENCES Employee(Emp_ID)"
	");"
;

const std::string create_table_flights = 
	"CREATE TABLE IF NOT EXISTS Flight ("
	"FL_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"FL_Takeoff_City TEXT NOT NULL,"
	"FL_Land_City TEXT NOT NULL,"
	"FL_Takeoff_Time DATETIME NOT NULL,"
	"FL_Land_Time DATETIME NOT NULL,"
	"Plane_ID TEXT NOT NULL,"

	"CHECK(FL_Takeoff_City NOT IN (\"Rome\", \"Moscow\", \"London\")),"
	"CHECK(FL_Land_City NOT IN (\"Rome\", \"Moscow\", \"London\")),"
	
	"FOREIGN KEY (Plane_ID) REFERENCES Plane(Plane_ID)"
	");"
;

const std::string create_table_flightregister = 
	"CREATE TABLE IF NOT EXISTS FlightRegister ("
	"FL_ID INTEGER,"
	"P_ID INTEGER,"
	"PRIMARY KEY (FL_ID, P_ID),"
	"FOREIGN KEY (FL_ID) REFERENCES Flight(FL_ID),"
	"FOREIGN KEY (P_ID) REFERENCES Passenger(P_ID)"
	");"
;

const std::string create_table_employeeonboard =
	"CREATE TABLE IF NOT EXISTS EmployeeOnboard ("
	"FL_ID INTEGER,"
	"Emp_ID TEXT CHECK(length(Emp_ID) = 4),"
	"PRIMARY KEY (FL_ID, Emp_ID),"
	"FOREIGN KEY (FL_ID) REFERENCES Flight(FL_ID),"
	"FOREIGN KEY (Emp_ID) REFERENCES Employee(Emp_ID)"
	");"
;

const std::string STR_LABEL_SHOW_TABLE			= "Show Table";
const std::string STR_LABEL_INSERT_DATA			= "Insert Data";
const std::string STR_LABEL_QUERY_1				= "Query N1";
const std::string STR_LABEL_QUERY_2				= "Query N2";
const std::string STR_LABEL_QUERY_3				= "Query N3";
const std::string STR_LABEL_QUERY_4				= "Query N4";
const std::string STR_LABEL_QUERY_5				= "Query N5";
const std::string STR_LABEL_QUERY_6				= "Query N6";
const std::string STR_LABEL_QUERY_7				= "Query N7";
const std::string STR_LABEL_REMOVE_DATA			= "Remove Data";

const std::unordered_map<std::string, CONSTS::LABELS> LABEL_TO_ENUM =
	{
		{STR_LABEL_SHOW_TABLE,		CONSTS::LABEL_SHOW_TABLE},
		{STR_LABEL_INSERT_DATA,		CONSTS::LABEL_INSERT_DATA},
		{STR_LABEL_QUERY_1,			CONSTS::LABEL_QUERY_1},
		{STR_LABEL_QUERY_2,			CONSTS::LABEL_QUERY_2},
		{STR_LABEL_QUERY_3,			CONSTS::LABEL_QUERY_3},
		{STR_LABEL_QUERY_4,			CONSTS::LABEL_QUERY_4},
		{STR_LABEL_QUERY_5,			CONSTS::LABEL_QUERY_5},
		{STR_LABEL_QUERY_6,			CONSTS::LABEL_QUERY_6},
		{STR_LABEL_QUERY_7,			CONSTS::LABEL_QUERY_7},
		{STR_LABEL_REMOVE_DATA,		CONSTS::LABEL_REMOVE_DATA}
	};

const std::string STR_TABLE_PASSENGER			= "Passenger";
const std::string STR_TABLE_BAGGAGE				= "Baggage";
const std::string STR_TABLE_AIRPLANE			= "Airplane";
const std::string STR_TABLE_HANGAR				= "Hangar";
const std::string STR_TABLE_EMPLOYEE			= "Employee";
const std::string STR_TABLE_EMPLOYEEADDRESS		= "EmployeeAddress";
const std::string STR_TABLE_FLIGHT				= "Flight";
const std::string STR_TABLE_FLIGHTREGISTER		= "FlightRegister";
const std::string STR_TABLE_EMPLOYEEONBOARD		= "EmployeeOnboard";

const std::unordered_map<std::string, CONSTS::TABLES> TABLE_TO_ENUM = 
	{
		{STR_TABLE_PASSENGER,		CONSTS::TABLE_PASSENGER},
		{STR_TABLE_BAGGAGE,			CONSTS::TABLE_BAGGAGE},
		{STR_TABLE_AIRPLANE,		CONSTS::TABLE_AIRPLANE},
		{STR_TABLE_HANGAR,			CONSTS::TABLE_HANGAR},
		{STR_TABLE_EMPLOYEE,		CONSTS::TABLE_EMPLOYEE},
		{STR_TABLE_EMPLOYEEADDRESS,	CONSTS::TABLE_EMPLOYEEADDRESS},
		{STR_TABLE_FLIGHT,			CONSTS::TABLE_FLIGHT},
		{STR_TABLE_FLIGHTREGISTER,	CONSTS::TABLE_FLIGHTREGISTER},
		{STR_TABLE_EMPLOYEEONBOARD, CONSTS::TABLE_EMPLOYEEONBOARD}
	};



// CONSTS struct stuff initialization
const std::vector<std::string> CONSTS::ALL_CREATE_TABLES =
	{
		create_table_passengers, create_table_baggage, create_table_airplanes,
		create_table_hangars, create_table_employees, create_table_employeeaddress,
		create_table_flights, create_table_flightregister, create_table_employeeonboard
	};

const std::vector<std::string> CONSTS::TABLE_NAMES =
	{
		STR_TABLE_PASSENGER, STR_TABLE_BAGGAGE, STR_TABLE_AIRPLANE,
		STR_TABLE_HANGAR, STR_TABLE_EMPLOYEE, STR_TABLE_EMPLOYEEADDRESS,
		STR_TABLE_FLIGHT, STR_TABLE_FLIGHTREGISTER, STR_TABLE_EMPLOYEEONBOARD
	};

const std::vector<std::string> CONSTS::MENU_LABELS = 
	{
		STR_LABEL_SHOW_TABLE,	STR_LABEL_INSERT_DATA,	STR_LABEL_QUERY_1,
		STR_LABEL_QUERY_2,		STR_LABEL_QUERY_3,		STR_LABEL_QUERY_4,
		STR_LABEL_QUERY_5,		STR_LABEL_QUERY_6,		STR_LABEL_QUERY_7,
		STR_LABEL_REMOVE_DATA
	};

std::string CONSTS::ConvertLabelName(const CONSTS::LABELS& label) {
	for(auto& [key, value] : LABEL_TO_ENUM) {
		if(value == label)
			return key;
	}
	throw std::runtime_error("ERROR: No label with number " + std::to_string(label) + " found!");
}

CONSTS::LABELS CONSTS::ConvertLabelName(const std::string& label) {
	for(auto& [key, value] : LABEL_TO_ENUM) {
		if(key == label)
			return value;
	}
	throw std::runtime_error("ERROR: No label \"" + label + "\" found in LABEL_TO_ENUM");
}

std::string CONSTS::ConvertTableName(const CONSTS::TABLES& table) {
	for(auto& [key, value] : TABLE_TO_ENUM){
		if(value == table)
			return key;
	}
	throw std::runtime_error("ERROR: No table with number " + std::to_string(table) + " found!");
}

CONSTS::TABLES CONSTS::ConvertTableName(const std::string& table) {
	for(auto& [key, value] : TABLE_TO_ENUM) {
		if(key == table)
			return value;
	}
	throw std::runtime_error("ERROR: No table \"" + table + "\" found in TABLE_TO_ENUM");
}