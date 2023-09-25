#include <iostream>

#include "application.hpp"

#define PROJECT_NAME "robotrampage"

int main(int argc, char **argv)
{
	if(argc != 1)
	{
		std::cerr << argv[0] << "takes no arguments.\n";
		return 1;
	}

	try
	{
		Application app;
		app.run();
		return 0;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		return 1;
	}
}
