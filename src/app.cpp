#include "app.h"

#include <iostream>


namespace App {
MainApp::~MainApp() {
	glfwDestroyWindow(m_Window);
}

void MainApp::init() {
	init_opengl();
	gui::Init(m_Window, m_IO);

	// m_DB.init("");
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
	if(action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_ESCAPE:
				// Our app will have only one window, so this variable is enough
				m_WindowShouldClose = true;
				break;
		}
	}
}

void MainApp::run() {
	while(!(m_WindowShouldClose || glfwWindowShouldClose(m_Window))) {
		glfwPollEvents();
		
		gui::RenderBegin();
		// FIXME: Transfer this to gui namespace somehow
		run_main();
		gui::RenderEnd(m_Window, m_WindowWidth, m_WindowHeight);
	}
}

void MainApp::run_main() {
	// Flags for both windows
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	
	// Create side menu window
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(SIDE_MENU_WIDTH, m_WindowHeight));
	ImGui::Begin("Side Panel", nullptr, flags);
	ImGui::End();

	// Create table window
	ImGui::SetNextWindowSize(ImVec2(m_WindowWidth-SIDE_MENU_WIDTH, m_WindowHeight));
	ImGui::SetNextWindowPos(ImVec2(SIDE_MENU_WIDTH, 0));
	ImGui::Begin("Table Test", nullptr, flags);
	if (ImGui::BeginTable("table2", 3))
        {
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Row %d", row);
                ImGui::TableNextColumn();
                ImGui::Text("Some contents");
                ImGui::TableNextColumn();
                ImGui::Text("123.456");
            }
            ImGui::EndTable();
        }
	ImGui::End();
}
} // namespace App