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

		// A seperate functions, to not create mess in the while() loop
		void draw_side_panel_window();
		void process_label(const std::string& label);

		// Functions for different windows
		void draw_table_window();

	
	private:
		DB::DataBase m_DB;
		GLFWwindow* m_Window = nullptr;
		static inline bool m_Keys[1024] = {0};

		// FIXME: Maybe change this to dynamic?
		int m_WindowWidth = 1600;
		int m_WindowHeight = 900;
		static inline float m_FontScale = 1.;

		ImGuiIO* m_IO = nullptr;

		// Window control variables
		static inline bool m_ShowTable_IsActive		= false;
		static inline bool m_InsertData_IsActive	= false;
		static inline bool m_Query1_IsActive		= false;
		static inline bool m_Query2_IsActive		= false;
		static inline bool m_Query3_IsActive		= false;
		static inline bool m_Query4_IsActive		= false;
		static inline bool m_Query5_IsActive		= false;
		static inline bool m_Query6_IsActive		= false;
		static inline bool m_RemoveData_IsActive	= false;
};
} // namespace App