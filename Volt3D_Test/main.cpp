#include <iostream>

#include <Volt3D.h>

int main()
{
	try
	{
		v3d::Engine engine;
		if (engine.init("Volt3D Test"))
		{
			engine.run();
		}
	}
	catch (vk::SystemError err)
	{
		std::cout << "vk::SystemError: " << err.what() << std::endl;
		exit(-1);
	}
	catch (std::runtime_error err)
	{
		std::cout << "std::runtime_error: " << err.what() << std::endl;
		exit(-1);
	}
	catch (...)
	{
		std::cout << "unknown error\n";
		exit(-1);
	}

	return 0;
}