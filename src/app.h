#pragma once

#include "gui.h"
#include "database.h"
#include "constants.h"

#include <array>

#define SIDE_MENU_WIDTH 200
#define FONT_SCALE_MAX 2.5
#define FONT_SCALE_MIN 0.5


namespace App {
class MainApp {
	public:
		MainApp() : m_DB() {};
		~MainApp();
		
		void init();
		void run();


	private:
		void init_opengl();
		static void gl_key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);
		void process_keys();

		// A seperate functions, to not create mess in the while() loop
		void draw_side_panel_window();
		void draw_table_window();
		void draw_table();
		void insert_data();
		void draw_table_combobox();


	private:
		DB::DataBase m_DB;
		GLFWwindow* m_Window = nullptr;
		static inline bool m_Keys[1024] = {0};

		// FIXME: Maybe change this to dynamic?
		int m_WindowWidth = 1600;
		int m_WindowHeight = 900;
		static inline float m_FontScale = 1.;

		// ImGuiIO* m_IO = nullptr;
		CONSTS::LABELS m_CurrentLabel = CONSTS::LABEL_SHOW_TABLE;
		std::string m_CurrentTable = CONSTS::TABLE_NAMES[0];
		std::string m_LastTable = "";

		bool m_InsertRow = false;

		// Initialize static char arrays for input on heap
		// 256 characters will be enough, right?
		// FIXME: Hardcoded buffers. Make dynamic buffers.
		std::array<char*, 16> m_InputFields;
};
} // namespace App