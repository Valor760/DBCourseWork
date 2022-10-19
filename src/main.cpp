#include "app.h"

#include <iostream>


int main(int argc, char** argv)
{
	try {
		App::MainApp app;
		app.init();
		app.run();
	}
	catch(const std::exception& e) {
		std::cout << e.what() << '\n';
		return 1;
	}
	return 0;
}
