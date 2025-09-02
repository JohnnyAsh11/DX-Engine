#include <iostream>
#include "Application.h"

int main(void)
{
	// Cosntructing the Application.
	Application* app = new Application();

	// Running the Application.
	app->Run();

	// Reallocating the memory used by the application.
	SafeDelete(app);

	// Ensuring that there are no memory leaks.
	if (_CrtDumpMemoryLeaks())
	{
		std::cout << "ERROR: Memory leaks present." << std::endl;
	}

	return 0;
}