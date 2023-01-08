#include "app.h"

#include <iostream>


namespace App {
MainApp::~MainApp() {
	glfwDestroyWindow(m_Window);
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	for(auto& arr : m_InputFields)
		delete[] arr;
}

void MainApp::init() {
	init_opengl();
	gui::Init(m_Window);
	m_DB.init();

	// Allocate all m_InputFields
	for(auto& arr : m_InputFields) {
		arr = new char[256];
		memset(arr, 0, 256);
	}
}

void MainApp::init_opengl() {
	// Init OpenGL and ImGui stuff
	if(!glfwInit())
		throw std::runtime_error("Cannot initialize GLFW");
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Airport Database", nullptr, nullptr);
	if(m_Window == NULL)
		throw std::runtime_error("Failed to create window");

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); // Enable VSYNC

	glfwSetKeyCallback(m_Window, gl_key_callback);
	glfwSetWindowSizeCallback(m_Window, gl_window_size_callback);
}

void MainApp::gl_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode) {
	if(action == GLFW_PRESS)
		if(!m_ProcessedKeys[key])
			m_Keys[key] = true;
	
	if(action == GLFW_RELEASE) {
		m_Keys[key] = false;
		m_ProcessedKeys[key] = false;
	}
}

void MainApp::gl_window_size_callback(GLFWwindow* window, int width, int height) {
	m_WindowWidth = width;
	m_WindowHeight = height;
}

void MainApp::process_keys() {
	if(m_Keys[GLFW_KEY_ESCAPE] && !m_ProcessedKeys[GLFW_KEY_ESCAPE]) {
		if(m_ErrorOccurred) {
			m_ErrorOccurred = false;
		}
		else {
			glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}
		m_ProcessedKeys[GLFW_KEY_ESCAPE] = true;
	}

	// FIXME: Remove font scaling, since it is needed only for large monitors
	if(m_Keys[GLFW_KEY_LEFT_CONTROL] && m_Keys[GLFW_KEY_EQUAL] && !m_ProcessedKeys[GLFW_KEY_EQUAL]) {
		m_ProcessedKeys[GLFW_KEY_EQUAL] = true;
		m_FontScale += 0.2;
		if(m_FontScale > FONT_SCALE_MAX)
			m_FontScale = FONT_SCALE_MAX;
	}
	if(m_Keys[GLFW_KEY_LEFT_CONTROL] && m_Keys[GLFW_KEY_MINUS] && !m_ProcessedKeys[GLFW_KEY_MINUS]) {
		m_ProcessedKeys[GLFW_KEY_MINUS] = true;
		m_FontScale -= 0.2;
		if(m_FontScale < FONT_SCALE_MIN)
			m_FontScale = FONT_SCALE_MIN;
	}
}

void MainApp::run() {
	while(!glfwWindowShouldClose(m_Window)) {
		glfwPollEvents();
		process_keys();

		gui::RenderBegin();

		draw_side_panel_window();
		draw_table_window();
		if(m_ErrorOccurred)
			show_error();

		gui::RenderEnd(m_Window, m_WindowWidth, m_WindowHeight);
	}
}

void MainApp::draw_side_panel_window() {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	
	// Create side menu window
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(SIDE_MENU_WIDTH, m_WindowHeight));

	ImGui::Begin("Side Panel", nullptr, flags);
	ImGui::SetWindowFontScale(m_FontScale);
	
	// Display all labels in side menu
	for(auto& label : CONSTS::MENU_LABELS) {
		bool is_selected = (m_CurrentLabel == CONSTS::ConvertLabelName(label));
		if(ImGui::Selectable(label.c_str(), is_selected)) {
			m_CurrentLabel = CONSTS::ConvertLabelName(label);
		}
		if (is_selected) {
			ImGui::SetItemDefaultFocus();
		}
	}

	ImGui::End();
}

void MainApp::draw_table_window() {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

	// Create table(right) window
	ImGui::SetNextWindowSize(ImVec2(m_WindowWidth-SIDE_MENU_WIDTH, m_WindowHeight));
	ImGui::SetNextWindowPos(ImVec2(SIDE_MENU_WIDTH, 0));
	ImGui::Begin("##Table Test", nullptr, flags);
	ImGui::SetWindowFontScale(m_FontScale);
	
	switch(m_CurrentLabel) {
		case CONSTS::LABEL_SHOW_TABLE:
			draw_table_combobox();
			draw_table();
			break;
		case CONSTS::LABEL_INSERT_DATA:
			draw_table_combobox();
			insert_data();
			break;
		case CONSTS::LABEL_QUERY_1:
		case CONSTS::LABEL_QUERY_2:
		case CONSTS::LABEL_QUERY_3:
		case CONSTS::LABEL_QUERY_4:
		case CONSTS::LABEL_QUERY_5:
		case CONSTS::LABEL_QUERY_6:
		case CONSTS::LABEL_QUERY_7:
			execute_query();
			break;
		case CONSTS::LABEL_REMOVE_DATA:
			draw_table_combobox();
			delete_data();
			break;
		default:
			break;
	}
	ImGui::End();
	// ImGui::ShowDemoWindow();
}

void MainApp::draw_table_combobox() {
	// Make combobox full table window width
	ImGui::SetNextItemWidth(m_WindowWidth - SIDE_MENU_WIDTH - 15);

	// Draw combobox with different table names
	if(ImGui::BeginCombo("##Tables", m_CurrentTable.c_str())) 
	{
		for(auto& table : CONSTS::TABLE_NAMES) 
		{
			const bool is_selected = (table == m_CurrentTable);
			if(ImGui::Selectable(table.c_str(), is_selected)) 
			{
				m_CurrentTable = table;
			}
		}
		ImGui::EndCombo();
		if(m_CurrentTable != m_LastTable)
			m_ReceivedColNames = false;
	}
}

void MainApp::draw_table() {
	if(m_CurrentTable != m_LastTable) {
		m_DB.execute("SELECT * FROM %s", m_CurrentTable.c_str());
		m_LastQuery_Columns = m_DB.GetLastQueryColumns();
		m_LastQuery_Rows = m_DB.GetLastQueryResult();
		m_LastTable = m_CurrentTable;
	}

	if(m_LastQuery_Rows.empty()) {
		ImGui::Text("No data available in this table! Please insert data...");
	}
	else {
		// Draw table with data
		if(ImGui::BeginTable("##DBTable", m_LastQuery_Columns.size())) 
		{	
			// Print headers
			for(auto& column_name : m_LastQuery_Columns) 
			{
				ImGui::TableSetupColumn(column_name.c_str());
			}
			ImGui::TableHeadersRow();
			for(auto& row : m_LastQuery_Rows)
			{
				ImGui::TableNextRow();
				for(auto& record : row)
				{
					ImGui::TableNextColumn();
					ImGui::Text(record.c_str());
				}
			}
			ImGui::EndTable();
		}
	}
}

void MainApp::insert_data() {
	// ImGui::Spacing();

	if(m_CurrentTable != m_LastTable) {
		m_ReceivedColNames = false;
	} 

	if(!m_ReceivedColNames) {
		m_LastTableInfo = m_DB.GetTableInfo(m_CurrentTable);
		m_ReceivedColNames = true;
	}

	// Draw columns for insert
	if(m_LastTableInfo.size() && ImGui::BeginTable("##TableInsert", m_LastTableInfo.size()))
	{
		for(auto& column_name : m_LastTableInfo) {
			ImGui::TableSetupColumn(column_name[1].c_str());
		}
		ImGui::TableHeadersRow();

		bool id_col_active = false;
		if(m_CurrentTable == "Employee" || m_CurrentTable == "Hangar" || m_CurrentTable == "Airplane" || m_CurrentTable == "FlightRegister" || m_CurrentTable == "EmployeeAddress") {
			id_col_active = true;
		}

		const float cell_width = (m_WindowWidth - SIDE_MENU_WIDTH - 10) / (float)m_LastTableInfo.size();
		for(int cell_idx = 0; cell_idx < m_LastTableInfo.size(); cell_idx++) {
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(cell_width);
			ImGui::PushID(cell_idx);

			const bool is_disabled = (!id_col_active && cell_idx == 0);
			if(is_disabled) ImGui::BeginDisabled();
			ImGui::InputText("##cell", m_InputFields[cell_idx], 256, 0);
			if(is_disabled) ImGui::EndDisabled();

			ImGui::PopID();
		}

		ImGui::EndTable();

		// Insertion button
		if(ImGui::Button("Insert to Table", ImVec2(-FLT_MIN, 0))) {
			insert_button_click(id_col_active);
		}
		ImGui::Spacing();
	}

	draw_table();
}

void MainApp::insert_button_click(const bool& id_col_active) {
	std::string values = "", columns = "";
	// It will be always executed after MainApp::insert_data
	// which executes table info
	auto table_info = m_DB.GetTableInfo(m_CurrentTable);
	const int column_count = table_info.size();
	int start_idx = id_col_active ? 0 : 1;

	for(int i = start_idx; i < column_count; i++) {
		std::string value = m_InputFields[i];
		values += std::string(value.empty() ? "NULL" : ("\"" + value + "\"")) + ",";
		columns += table_info[i][1] + ",";
	}
	// Always remove last comma
	values.pop_back();
	columns.pop_back();


	if(!m_DB.execute("INSERT INTO %s(%s) VALUES (%s);", m_CurrentTable.c_str(), columns.c_str(), values.c_str())) {
		m_ErrorOccurred = true;
	}

	// Clear input row after insertion
	for(auto& arr : m_InputFields) {
		memset(arr, 0, 256);
	}
	m_LastTable = "";
}
 
void MainApp::show_error() {
	const int win_err_height = 300;
	const int win_err_width = m_WindowWidth - SIDE_MENU_WIDTH;
	ImGuiWindowFlags win_err_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2(SIDE_MENU_WIDTH, m_WindowHeight - win_err_height));
	ImGui::SetNextWindowSize(ImVec2(win_err_width, win_err_height));
	ImGui::Begin("##Error", &m_ErrorOccurred, win_err_flags);
	ImGui::SetWindowFontScale(m_FontScale);
	ImGui::Text(m_DB.GetLastErrorMsg().c_str());
	ImGui::End();
}

void MainApp::delete_data() {
	if(m_CurrentTable != m_LastTable) {
		m_DB.execute("SELECT * FROM %s", m_CurrentTable.c_str());
		m_LastQuery_Columns = m_DB.GetLastQueryColumns();
		m_LastQuery_Rows = m_DB.GetLastQueryResult();
		m_LastTable = m_CurrentTable;
	}

	if(m_LastQuery_Rows.empty()) {
		ImGui::Text("No data available in this table! Please insert data...");
	}
	else {
		ImGui::Text("Select row's primary key to remove:");
		ImGui::Text("%s:", m_LastQuery_Columns[0].c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(m_WindowWidth - SIDE_MENU_WIDTH - 100. * 2);

		// Draw combobox with different primary keys
		if(ImGui::BeginCombo("##IDS", m_SelectedIDCol.c_str()))
		{
			for(auto& row : m_LastQuery_Rows)
			{
				const bool is_selected = (m_SelectedIDCol == row[0]);
				if(ImGui::Selectable(row[0].c_str(), is_selected))
				{
					m_SelectedIDCol = row[0];
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if(ImGui::Button("Delete", ImVec2(-FLT_MIN, 0))) {
			// Remove selected row
			if(!m_DB.execute("DELETE FROM %s WHERE %s == \"%s\";",
						 m_CurrentTable.c_str(), m_LastQuery_Columns[0].c_str(), m_SelectedIDCol.c_str()))
			{
				m_ErrorOccurred = true;
			}
			m_LastTable = ""; // Force table update
		}

		// Draw table with data
		if(ImGui::BeginTable("##DBTable", m_LastQuery_Columns.size()))
		{
			// Print headers
			for(auto& column_name : m_LastQuery_Columns) 
			{
				ImGui::TableSetupColumn(column_name.c_str());
			}
			ImGui::TableHeadersRow();
			for(auto& row : m_LastQuery_Rows)
			{
				ImGui::TableNextRow();
				for(auto& record : row)
				{
					ImGui::TableNextColumn();
					ImGui::Text(record.c_str());
				}
			}
			ImGui::EndTable();
		}
	}
}

void MainApp::execute_query() {
	switch(m_CurrentLabel) {
		case CONSTS::LABEL_QUERY_1:
		{
			ImGui::Text("Input Flight ID to see how many people registered on the flight");

			static char input[256];
			ImGui::SetNextItemWidth(m_WindowWidth - SIDE_MENU_WIDTH - 150);
			ImGui::InputText("##input", input, 256);

			const char* query =
				"SELECT COUNT(*) as \"Number of people on ID-%s flight\" "
				"FROM FlightRegister "
				"WHERE FL_ID = \"%s\";"
			;
			ImGui::SameLine();
			if(ImGui::Button("Query", ImVec2(-FLT_MIN, 0))) {
				m_DB.execute(query, input, input);
				m_LastQuery_Rows = m_DB.GetLastQueryResult();
				m_LastQuery_Columns = m_DB.GetLastQueryColumns();
			}
			if(m_LastQuery_Columns.size() == 1) {
				draw_table();
			}
			break;
		}
		case CONSTS::LABEL_QUERY_2:
		{
			ImGui::Text("Show how many flights done by each pilot");

			const std::string query =
				"SELECT	EmployeeOnboard.Emp_ID as \"Pilot ID\", "
				"		Employee.Emp_Name as \"Pilot Name\", "
				"		Employee.Emp_Surname as \"Pilot Surname\", "
				"		COUNT(EmployeeOnboard.FL_ID) as \"Pilot Flights\" "
				"FROM EmployeeOnboard "
				"INNER JOIN Employee "
				"ON EmployeeOnboard.Emp_ID = Employee.Emp_ID "
				"WHERE Employee.Emp_Position = \"Pilot\" "
				// "GROUP BY EmployeeOnboard.Emp_ID , Employee.Emp_Name, Employee.Emp_Surname "
				// "HAVING Employee.Emp_Position = \"Pilot\";"
			;

			ImGui::SameLine();
			if(ImGui::Button("Query", ImVec2(-FLT_MIN, 0))) {
				m_DB.execute(query.c_str());
				m_LastQuery_Rows = m_DB.GetLastQueryResult();
				m_LastQuery_Columns = m_DB.GetLastQueryColumns();
			}
			if(m_LastQuery_Columns.size() == 4) {
				draw_table();
			}
			break;
		}
		case CONSTS::LABEL_QUERY_3:
		{
			ImGui::Text("Show average baggage weight among all flights for each plane");

			const std::string query =
				"SELECT "
				"		Airplane.Plane_ID as \"Airplane ID\", "
				"		AVG(Baggage.B_Weight_KG) as \"Average Baggage Weight (KG)\" "
				"FROM Airplane "
				"INNER JOIN Flight ON Flight.Plane_ID = Airplane.Plane_ID "
				"INNER JOIN FlightRegister ON FlightRegister.FL_ID = Flight.FL_ID "
				"INNER JOIN Passenger ON FlightRegister.P_ID = Passenger.P_ID "
				"INNER JOIN Baggage ON Passenger.P_ID = Baggage.P_ID "
			;

			ImGui::SameLine();
			if(ImGui::Button("Query", ImVec2(-FLT_MIN, 0))) {
				m_DB.execute(query.c_str());
				m_LastQuery_Rows = m_DB.GetLastQueryResult();
				m_LastQuery_Columns = m_DB.GetLastQueryColumns();
			}
			if(m_LastQuery_Columns.size() == 2) {
				draw_table();
			}
			break;
		}
		case CONSTS::LABEL_QUERY_4:
		{
			ImGui::Text("Show who did use Riga airport for more than 5 flights");

			const std::string query =
				"SELECT "
				"		Passenger.P_ID as \"Passenger ID\", "
				"		Passenger.P_Name as \"Passenger Name\", "
				"		Passenger.P_Surname as \"Passenger Surname\" "
				"FROM Passenger "
				"INNER JOIN FlightRegister ON Passenger.P_ID = FlightRegister.P_ID "
				"GROUP BY Passenger.P_ID "
				"HAVING COUNT(FlightRegister.FL_ID) >= 5 "
			;

			ImGui::SameLine();
			if(ImGui::Button("Query", ImVec2(-FLT_MIN, 0))) {
				m_DB.execute(query.c_str());
				m_LastQuery_Rows = m_DB.GetLastQueryResult();
				m_LastQuery_Columns = m_DB.GetLastQueryColumns();
			}
			if(m_LastQuery_Columns.size() == 3) {
				draw_table();
			}
			break;
		}
		case CONSTS::LABEL_QUERY_5:
		{
			ImGui::Text("Show number of flights which had Riga airport as a destination");

			const std::string query =
				"SELECT "
				"		COUNT(*) as \"Number of fligths with destination city - Riga\" "
				"FROM Flight "
				"WHERE FL_Land_City = \"Riga\" "
			;

			ImGui::SameLine();
			if(ImGui::Button("Query", ImVec2(-FLT_MIN, 0))) {
				m_DB.execute(query.c_str());
				m_LastQuery_Rows = m_DB.GetLastQueryResult();
				m_LastQuery_Columns = m_DB.GetLastQueryColumns();
			}
			if(m_LastQuery_Columns.size() == 1) {
				draw_table();
			}
			break;
		}
		case CONSTS::LABEL_QUERY_6:
		{
			ImGui::Text("Show how many times each pilot has taken off from Riga airport");

			const std::string query =
				"SELECT "
				"		Employee.Emp_ID as \"Pilot ID\", "
				"		Employee.Emp_Name as \"Pilot Name\", "
				"		Employee.Emp_Surname as \"Pilot Surname\", "
				"		COUNT(*) as \"Number of takeoffs from Riga\" "
				"FROM Employee "
				"INNER JOIN EmployeeOnboard ON Employee.Emp_ID = EmployeeOnboard.Emp_ID "
				"INNER JOIN Flight ON EmployeeOnboard.FL_ID = Flight.FL_ID "
				"GROUP BY Employee.Emp_ID "
				"HAVING Employee.Emp_Position = \"Pilot\" AND Flight.FL_Takeoff_City = \"Riga\""
			;

			ImGui::SameLine();
			if(ImGui::Button("Query", ImVec2(-FLT_MIN, 0))) {
				m_DB.execute(query.c_str());
				m_LastQuery_Rows = m_DB.GetLastQueryResult();
				m_LastQuery_Columns = m_DB.GetLastQueryColumns();
			}
			if(m_LastQuery_Columns.size() == 4) {
				draw_table();
			}
			break;
		}
		case CONSTS::LABEL_QUERY_7:
		{
			ImGui::Text("Show each technician who currently maintains a plane which transported a fragile baggage");

			const std::string query =
				"SELECT "
				"		Employee.Emp_ID as \"Employee ID\", "
				"		Employee.Emp_Name as \"Employee Name\", "
				"		Employee.Emp_Surname as \"Employee Surname\" "
				"FROM Employee "
				"INNER JOIN Hangar ON Employee.H_ID = Hangar.H_ID "
				"INNER JOIN Airplane ON Hangar.Plane_ID = Airplane.Plane_ID "
				"INNER JOIN Flight ON Airplane.Plane_ID = Flight.Plane_ID "
				"INNER JOIN FlightRegister ON Flight.FL_ID = FlightRegister.FL_ID "
				"INNER JOIN Passenger ON FlightRegister.P_ID = Passenger.P_ID "
				"INNER JOIN Baggage ON Passenger.P_ID = Baggage.P_ID "
				"WHERE Baggage.B_IsFragile = \"TRUE\" AND Employee.Emp_Position = \"Technician\" "
			;

			ImGui::SameLine();
			if(ImGui::Button("Query", ImVec2(-FLT_MIN, 0))) {
				m_DB.execute(query.c_str());
				m_LastQuery_Rows = m_DB.GetLastQueryResult();
				m_LastQuery_Columns = m_DB.GetLastQueryColumns();
			}
			if(m_LastQuery_Columns.size() == 3) {
				draw_table();
			}
			break;
		}
		default:
			throw std::runtime_error("execute_query() - Not a query label provided!");
	}
}
} // namespace App