#include "app.h"
#include "common.h"


int main(int argc, char** argv)
{
	try {
		App::MainApp app;
		app.init();
		app.run();
	}
	catch(const std::exception& e) {
		LOG(LOG_ERR, e.what());
		return 1;
	}
	return 0;
}
