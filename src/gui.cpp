#include "gui.h"


namespace gui {
void RenderBegin() {
	// Start Imgui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderEnd(GLFWwindow* window, int width, int height) {
	// Render
	ImGui::Render();
	glViewport(0, 0, width, height);
	glClearColor(0.5, 0.33, 0.7, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Init(GLFWwindow* window) {//, ImGuiIO* io) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// io = &ImGui::GetIO();
	
	// Init imgui platform backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}
} // namespace gui