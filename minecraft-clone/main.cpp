#include "Engine.h"

int main() {
	try
	{
		Engine engine;
		engine.start();
		engine.update();
		engine.cleanup();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
	
}