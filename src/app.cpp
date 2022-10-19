#include "app.h"

#include <iostream>


namespace App {
MainApp::~MainApp() {
	glfwDestroyWindow(m_Window);
}

void MainApp::init() {
	init_opengl();
	init_imgui();

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

void MainApp::init_imgui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	// Init imgui platform backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
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

void MainApp::run_main() {
	ImGui::SetNextWindowSize(ImVec2(m_WindowWidth, m_WindowHeight));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Table Test");
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

void MainApp::run() {
	while(!(m_WindowShouldClose || glfwWindowShouldClose(m_Window))) {
		glfwPollEvents();

		// Start Imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		run_main();

		// Render
		ImGui::Render();
		glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		glClearColor(0.5, 0.33, 0.7, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_Window);
	}
}
} // namespace App