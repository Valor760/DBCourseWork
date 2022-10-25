#pragma once

#include "gui.h"
#include "database.h"

#define SIDE_MENU_WIDTH 200


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
		static void gl_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);
		void process_keys();

		// A seperate function, to not create mess in the while() loop
		void run_main();
	
	private:
		DB::DataBase m_DB;
		GLFWwindow* m_Window = nullptr;
		static inline bool m_Keys[1024] = {0};

		// FIXME: Maybe change this to dynamic?
		int m_WindowWidth = 1600;
		int m_WindowHeight = 900;
		static inline float m_FontScale = 1.;

		ImGuiIO* m_IO = nullptr;
};
} // namespace App