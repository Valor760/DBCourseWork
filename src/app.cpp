#include "app.h"

#include <iostream>


namespace App {
MainApp::MainApp() {}

int MainApp::init() {
	std::cout << "Hello World!\n";
	return 0;
}

void MainApp::run() {

}
} // namespace App