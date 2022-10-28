#include "app.h"

#include <iostream>

#include "constants.h"


namespace App {
MainApp::~MainApp() {
	glfwDestroyWindow(m_Window);
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void MainApp::init() {
	init_opengl();
	gui::Init(m_Window, m_IO);

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

	if(m_Keys[GLFW_KEY_LEFT_CONTROL] && m_Keys[GLFW_KEY_EQUAL]) {
		m_FontScale += 0.2;
		if(m_FontScale > 3.0)
			m_FontScale = 3.0;
	}
	if(m_Keys[GLFW_KEY_LEFT_CONTROL] && m_Keys[GLFW_KEY_MINUS]) {
		m_FontScale -= 0.2;
		if(m_FontScale < 0.5)
			m_FontScale = 0.5;
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

	// Display all labels in side menu
	for(auto& label : CONSTS::MENU_LABELS) {
		if(ImGui::Selectable(label.c_str())) {
			process_label(label);
			break;
		}
	}

	ImGui::End();

}

void MainApp::draw_table_window(const bool& is_active) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

	// Create table window
	ImGui::SetNextWindowSize(ImVec2(m_WindowWidth-SIDE_MENU_WIDTH, m_WindowHeight));
	ImGui::SetNextWindowPos(ImVec2(SIDE_MENU_WIDTH, 0));
	ImGui::Begin("Table Test", nullptr, flags);

	// If table window is selected, then render tables in it
	if(is_active) 
	{
		ImGui::SetWindowFontScale(m_FontScale);
		if (ImGui::BeginTable("table2", 3))
		{	
			static char inputs[4][256];
			for (int row = 0; row < 4; row++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				
				ImGui::PushID(row);
				ImGui::InputText("Input me", inputs[row], IM_ARRAYSIZE(inputs[row]));
				ImGui::PopID();

				ImGui::TableNextColumn();
				ImGui::Text("Some contents");
				ImGui::TableNextColumn();
				ImGui::Text("123.456");
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();
	// ImGui::ShowDemoWindow();
}

void MainApp::process_label(const std::string& label) {
	// FIXME: VEEEERY imperformant way to check
	// TODO: Make a function with enums and use switch
	if(label == CONSTS::LABEL_SHOW_TABLE) {
		
	}
	else if (label == CONSTS::LABEL_INSERT_DATA) {

	}
	else if (label == CONSTS::LABEL_QUERY_1) {

	}
	else if (label == CONSTS::LABEL_QUERY_2) {
		
	}
	else if (label == CONSTS::LABEL_QUERY_3) {
		
	}
	else if (label == CONSTS::LABEL_QUERY_4) {
		
	}
	else if (label == CONSTS::LABEL_QUERY_5) {
		
	}
	else if (label == CONSTS::LABEL_QUERY_6) {
		
	}
	else if (label == CONSTS::LABEL_REMOVE_DATA) {

	}
}
} // namespace App