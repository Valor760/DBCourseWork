#include "app.h"

#include <iostream>


int main(int argc, char** argv)
{
	App::MainApp app;
	if(app.init() != 0) {
		std::cout << "Error occured during main app initialization!\n";
		return 1;
	}
	app.run();

	return 0;
}
