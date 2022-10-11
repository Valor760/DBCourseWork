#pragma once

namespace App {
class MainApp {
	public:
		MainApp();
		
		// Initialize all program components
		//
		// return 0 if success, -1 otherwise
		int init();
		void run();
	
	private:
};
} // namespace App