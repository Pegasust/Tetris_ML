#include "keyboard_input.h"
#include <iostream>
int main()
{
	for (;;)
	{
		std::cout << Common::KeyboardListener<false, std::chrono::seconds, 3>::get_key() << std::endl;
	}
	return 0;
}