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
		
		gui::RenderBegin();
		// FIXME: Transfer this to gui namespace somehow
		draw_side_panel_window();
		draw_table_window();

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
		if(ImGui::Selectable(label.c_str())) {
			m_CurrentLabel = CONSTS::ConvertLabelName(label);
			break;
		}
	}

	ImGui::End();

}

void MainApp::draw_table_window() {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

	// Create table window
	ImGui::SetNextWindowSize(ImVec2(m_WindowWidth-SIDE_MENU_WIDTH, m_WindowHeight));
	ImGui::SetNextWindowPos(ImVec2(SIDE_MENU_WIDTH, 0));
	ImGui::Begin("Table Test", nullptr, flags);
	
	switch(m_CurrentLabel) {
		case CONSTS::LABEL_SHOW_TABLE:
			draw_table();
			break;
		default:
			break;
	}
	ImGui::End();
	// ImGui::ShowDemoWindow();
}

void MainApp::draw_table() {
	ImGui::SetWindowFontScale(m_FontScale);

	// Draw combobox with different table names
	if(ImGui::BeginCombo("qwe", CONSTS::TABLE_NAMES[0].c_str())) {
		for(auto& table : CONSTS::TABLE_NAMES) {
			bool x;
			if(ImGui::Selectable(table.c_str(), x));
		}
		ImGui::EndCombo();
	}

	switch(m_CurrentTable) {
		case CONSTS::TABLE_AIRPLANES:
			break;
		case CONSTS::TABLE_BAGGAGE:
			break;
		case CONSTS::TABLE_EMPLOYEEADDRESS:
			break;
		case CONSTS::TABLE_EMPLOYEES:
			break;
		case CONSTS::TABLE_FLIGHTREGISTER:
			break;
		case CONSTS::TABLE_FLIGHTS:
			break;
		case CONSTS::TABLE_HANGARS:
			break;
		case CONSTS::TABLE_PASSENGERS:
			break;
	}

// if (ImGui::BeginTable("table2", 3))
// 	{	
// 		static char inputs[4][256];
// 		for (int row = 0; row < 4; row++)
// 		{
// 			ImGui::TableNextRow();
// 			ImGui::TableNextColumn();
			
// 			ImGui::PushID(row);
// 			ImGui::InputText("Input me", inputs[row], IM_ARRAYSIZE(inputs[row]));
// 			ImGui::PopID();

// 			ImGui::TableNextColumn();
// 			ImGui::Text("Some contents");
// 			ImGui::TableNextColumn();
// 			ImGui::Text("123.456");
// 		}
// 		ImGui::EndTable();
// 	}
}
} // namespace App