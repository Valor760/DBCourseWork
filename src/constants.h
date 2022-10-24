#pragma once
#include <vector>
#include <unordered_map>

const char* create_table_passengers =
	"CREATE TABLE IF NOT EXISTS Passengers ("
	"P_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"P_Name TEXT NOT NULL,"
	"P_Surname TEXT NOT NULL,"
	"P_Gender TEXT,"
	"P_Phone_Num TEXT,"
	"CHECK(P_Gender IN (\"M\", \"F\"))"
	");"
;

const char* create_table_baggage =
	"CREATE TABLE IF NOT EXISTS Baggage ("
	"B_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"B_Weight_KG REAL NOT NULL,"
	"B_IsFragile BOOLEAN NOT NULL CHECK (B_IsFragile IN (0, 1)),"
	"P_ID INTEGER,"
	
	"CHECK(B_Weight_KG < 10),"
	"FOREIGN KEY (P_ID) REFERENCES Passengers(P_ID)"
	");"
;

const char* create_table_airplanes = 
	"CREATE TABLE IF NOT EXISTS Airplanes ("
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

const char* create_table_hangars =
	"CREATE TABLE IF NOT EXISTS Hangars ("
	"H_ID TEXT PRIMARY KEY,"
	"H_Size INTEGER NOT NULL,"
	"Plane_ID TEXT,"
	
	"CHECK(length(H_ID) = 3 AND length(Plane_ID) = 10),"
	"FOREIGN KEY (Plane_ID) REFERENCES Airplanes(Plane_ID)"
	");"
;

const char* create_table_employees = 
	"CREATE TABLE IF NOT EXISTS Employees ("
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
	"FOREIGN KEY (H_ID) REFERENCES Hangars(H_ID)"
	");"
;

const char* create_table_employeeaddress = 
	"CREATE TABLE IF NOT EXISTS EmployeeAddress ("
	"Emp_ID TEXT PRIMARY KEY,"
	"Ad_City TEXT,"
	"Ad_Street_Name TEXT NOT NULL,"
	"Ad_House_Nr INTEGER NOT NULL,"
	"Ad_Appart_Nr INTEGER,"
	"Ad_Post_Code TEXT,"
	
	"CHECK(length(Emp_ID) = 4),"
	"FOREIGN KEY (Emp_ID) REFERENCES Employees (Emp_ID)"
	");"
;

const char* create_table_flights = 
	"CREATE TABLE IF NOT EXISTS Flights ("
	"FL_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"FL_Takeoff_City TEXT NOT NULL,"
	"FL_Land_City TEXT NOT NULL,"
	"FL_Takeoff_Time DATETIME NOT NULL,"
	"FL_Land_Time DATETIME NOT NULL,"
	"Pilot_One_ID TEXT NOT NULL,"
	"Pilot_Two_ID TEXT NOT NULL,"
	"Stew_One_ID TEXT,"
	"Stew_Two_ID TEXT,"
	"Stew_Three_ID TEXT,"
	"Plane_ID TEXT NOT NULL,"

	"CHECK(length(Pilot_One_ID) = 4 AND length(Pilot_Two_ID) = 4 AND length(Stew_One_ID) = 4 AND length(Stew_Two_ID) = 4 AND length(Stew_Three_ID) = 4 AND length(Plane_ID) = 10),"
	"CHECK(Pilot_One_ID <> Pilot_Two_ID),"
	"CHECK(FL_Takeoff_City NOT IN (\"Rome\", \"Moscow\", \"London\")),"
	"CHECK(FL_Land_City NOT IN (\"Rome\", \"Moscow\", \"London\")),"
	
	"FOREIGN KEY (Pilot_One_ID) REFERENCES Employees(Emp_ID),"
	"FOREIGN KEY (Pilot_Two_ID) REFERENCES Employees(Emp_ID),"
	"FOREIGN KEY (Stew_One_ID) REFERENCES Employees(Emp_ID),"
	"FOREIGN KEY (Stew_Two_ID) REFERENCES Employees(Emp_ID),"
	"FOREIGN KEY (Stew_Three_ID) REFERENCES Employees(Emp_ID),"
	"FOREIGN KEY (Plane_ID) REFERENCES Planes(Plane_ID)"
	");"
;

const char* create_table_flightregister = 
	"CREATE TABLE IF NOT EXISTS FlightRegister ("
	"FL_ID INTEGER,"
	"P_ID INTEGER,"
	"PRIMARY KEY (FL_ID, P_ID),"
	"FOREIGN KEY (FL_ID) REFERENCES Flights(FL_ID),"
	"FOREIGN KEY (P_ID) REFERENCES Passengers(P_ID)"
	");"
;

const std::vector<const char*> ALL_CREATE_TABLES =
	{
		create_table_passengers, create_table_baggage, create_table_airplanes,
		create_table_hangars, create_table_employees, create_table_employeeaddress,
		create_table_flights, create_table_flightregister
	};

const char* TABLE_PASSENGERS		= "Passengers";
const char* TABLE_BAGGAGE			= "Baggage";
const char* TABLE_AIRPLANES			= "Airplanes";
const char* TABLE_HANGARS			= "Hangars";
const char* TABLE_EMPLOYEES			= "Employees";
const char* TABLE_EMPLOYEEADDRESS	= "EmployeeAddress";
const char* TABLE_FLIGHTS			= "Flights";
const char* TABLE_FLIGHTREGISTER	= "FlightRegister";

const std::unordered_map<const char*, int> TABLE_COLUMN_COUNT =
	{
		{TABLE_PASSENGERS,		5},
		{TABLE_BAGGAGE,			4},
		{TABLE_AIRPLANES,		4},
		{TABLE_HANGARS,			3},
		{TABLE_EMPLOYEES,		10},
		{TABLE_EMPLOYEEADDRESS,	6},
		{TABLE_FLIGHTS,			11},
		{TABLE_FLIGHTREGISTER,	2}
	};