#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "database.h"


namespace App {
class MainApp {
	public:
		MainApp() : m_DB() {};
		~MainApp();
		
		// Initialize all program components
		//
		// return 0 if success, -1 otherwise
		void init();
		void run();

	private:
		void init_opengl();
		void init_imgui();
		static void gl_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);

		// A seperate function, to not create mess in the while() loop
		void run_main();
	
	private:
		DB::DataBase m_DB;
		GLFWwindow* m_Window = nullptr;
		static inline bool m_WindowShouldClose = false;

		// FIXME: Maybe change this?
		int m_WindowWidth = 1600;
		int m_WindowHeight = 900;
};
} // namespace App