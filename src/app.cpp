#include "app.h"

#include <iostream>


namespace App {
MainApp::~MainApp() {
	glfwDestroyWindow(m_Window);
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void MainApp::init() {
	init_opengl();
	gui::Init(m_Window);//, m_IO);

	m_DB.init();

	// Allocate all m_InputFields
	// FIXME: MAKE CLEANUP!!!! AND MAKE BETTER
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
	// FIXME: Make window resizable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Airport Database", nullptr, nullptr);
	if(m_Window == NULL)
		throw std::runtime_error("Failed to create window");

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); // Enable VSYNC

	glfwSetKeyCallback(m_Window, gl_key_callback);
}

void MainApp::gl_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode) {
	if(action == GLFW_PRESS)
		m_Keys[key] = true;
	
	if(action == GLFW_RELEASE)
		m_Keys[key] = false;
}

void MainApp::process_keys() {
	if(m_Keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
	}

	// FIXME: Remove font scaling, since it is needed only for large monitors
	if(m_Keys[GLFW_KEY_LEFT_CONTROL] && m_Keys[GLFW_KEY_EQUAL]) {
		m_FontScale += 0.2;
		if(m_FontScale > FONT_SCALE_MAX)
			m_FontScale = FONT_SCALE_MAX;
	}
	if(m_Keys[GLFW_KEY_LEFT_CONTROL] && m_Keys[GLFW_KEY_MINUS]) {
		m_FontScale -= 0.2;
		if(m_FontScale < FONT_SCALE_MIN)
			m_FontScale = FONT_SCALE_MIN;
	}
}

void MainApp::run() {
	while(!glfwWindowShouldClose(m_Window)) {
		glfwPollEvents();
		process_keys();

		// TODO: This can be drawn once in every event
		// no need to update every frame
		gui::RenderBegin();

		// FIXME: Transfer this to gui namespace somehow
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
		// TODO: Make one function to execute user query based on label provided
		case CONSTS::LABEL_QUERY_1:
		case CONSTS::LABEL_QUERY_2:
		case CONSTS::LABEL_QUERY_3:
		case CONSTS::LABEL_QUERY_4:
		case CONSTS::LABEL_QUERY_5:
		case CONSTS::LABEL_QUERY_6:
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
	// m_InsertRow = false; // Disable row insertion if the table is changed

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

			// FIXME: What this does? Should it be in a program?
			// if (is_selected) {
			// 	ImGui::SetItemDefaultFocus();
			// }
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
		m_LastTable = m_CurrentTable;
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
		if(m_CurrentTable == "Employee" || m_CurrentTable == "Hangar" || m_CurrentTable == "Airlane") {
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
		// ImGui::SetNextItemWidth(del_button_size.x);
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

				// FIXME: What this does? Should it be in a program?
				// if (is_selected) {
				// 	ImGui::SetItemDefaultFocus();
				// }
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
			
			break;
		case CONSTS::LABEL_QUERY_2:
			break;
		case CONSTS::LABEL_QUERY_3:
			break;
		case CONSTS::LABEL_QUERY_4:
			break;
		case CONSTS::LABEL_QUERY_5:
			break;
		case CONSTS::LABEL_QUERY_6:
			break;
		default:
			throw std::runtime_error("execute_query() - Not a query label provided!");
	}
}
} // namespace App