#include "keyboard_input.h"
#include <iostream>
int main()
{
	for (;;)
	{
		std::cout << Common::KeyboardListener<false, 3, std::chrono::seconds>::get_key() << std::endl;
	}
	return 0;
}