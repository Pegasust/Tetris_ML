// Cmake_Zenix.cpp : Defines the entry point for the application.
//

#include "Cmake_Zenix.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;
	std::cout << "There are " << N_PROCESSOR_DETECTED << " processors detected." << std::endl;
	char* input = new char[16];

	std::cin >> input;
#if defined DEBUG_DEFINED
#else
#error
#endif
	std::cout << "Input was rounded to nearest: " << Common::ZMath::round_nearest(atof(input)) << std::endl;
	return 0;
}
