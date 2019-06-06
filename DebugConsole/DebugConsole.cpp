#define USER_TEST
//#define TIME_BASED
#ifndef USER_TEST
#define COMMAND_TEST
#endif // !USER_TEST
#include <iostream>
#include "../Core/renderer.h"
#include <thread>
#include <conio.h>
#include <stdio.h>
#include "keyboard_input.h"
namespace Module = LiteGameModule;
namespace Engine = LGEngine;

Renderer::RenderUnit* r_unit;
unsigned long long frames = 0;

bool child_func(Module::LiteModule& l_module)
{
	//std::cout << "updating..." << std::endl;
	//std::this_thread::sleep_for(std::chrono::duration<double>(Module::seconds_per_update));
	//capture input
	int input;
//#ifndef TIME_BASED
//	input = _getch();
//#else
	bool  timed_out = false;
	int upds = 1;
	auto now = std::chrono::high_resolution_clock::now();
	while (!_kbhit())
	{
		auto x = ((std::chrono::high_resolution_clock::now() - now).count());
		//std::cout << x << std::endl;
		if (x > (static_cast<long>(1.0/Module::seconds_per_update)))
		{
			timed_out = true;
			upds = min((x / static_cast<long>(1.0 / Module::seconds_per_update)), 3ll);
			break;
		}
	}
	if (!timed_out)
		input = _getch();
	else
		input = -1;
//#endif
	Module::Input i;
check_input:
	switch (input)
	{
		case 0:
		case -32:
			input = _getch();
		case keyboard_input::DOWN_ARROW:
			i = Module::DRAG_DOWN;
			break;
		case keyboard_input::RIGHT_ARROW:
			i = Module::RIGHT;
			break;
		case keyboard_input::LEFT_ARROW:
			i = Module::LEFT;
			break;
		case keyboard_input::DELETE:
			i = Module::ROTATE;
			break;
		case keyboard_input::ENTER:
			i = Module::DOWN;
			break;
		case keyboard_input::ESCAPE:
			Renderer::clear_console();
			std::cout << "Pausing game....... Press something to resume the game.";
			input = _getch();
			goto check_input;
			break;
		default:
			i = Module::NONE;
			break;
	}
	unsigned char n_burns, burn_y;
	bool useful_input;
	for (; upds > 0; upds--)
	{
		if (l_module.try_flex_update(i, n_burns, burn_y, useful_input, true)) //Not lost
		{
			//render
	//#ifdef _DEBUG
	//		std::cout << "not lost yet. \0" << std::endl;
	//#endif
			Renderer::clear_console();
			if (n_burns > 0)
			{
				r_unit->show_burns(n_burns, burn_y, 200);
				Renderer::clear_console();
			}
			r_unit->update_string(l_module);
			r_unit->show_shadow(l_module);
			return true;
		}
		else
		{
			//do something, player is lost
			Renderer::clear_console();
			std::cout << "You lost.\0" << std::endl;
			return false;
		}
	}
}

int main()
{
	TMath::GameRNG::RNGSeed rng[1] = { TMath::GameRNG::generate_random_seed() };
	Module::LiteModule *l_module = new Module::LiteModule(rng);
	std::cout << "Created l_module." << std::endl;
	Module::LiteModule m = *l_module;
	r_unit = new Renderer::RenderUnit(m);
	std::cout << "Successfully created r_unit." << std::endl;
	//Renderer::clear_console();
	r_unit->render();
	//_getch();

#ifdef USER_TEST
	do
	{
		while (child_func(*l_module)) {};
		std::cout << "Press a key to retry!" << std::endl;
		int key = _getch();
		std::this_thread::sleep_for(100ms); //This will prevent user from pressing down twice!
		delete l_module;
		TMath::GameRNG::xorshift64(rng);
		l_module = new Module::LiteModule(rng);
	} while (1);

#elif defined COMMAND_TEST
	Module::InputInfo info = { Module::DOWN, 7 };
	if(!l_module.try_update(info))
	{
		//If lost
		Renderer::clear_console();
		std::cout << "Lost." << std::endl;
		_getch();
	}
	Renderer::clear_console();
	r_unit->update_string(l_module);
	r_unit->render();

#endif
	delete r_unit;
	do
	{
		
	} while (1);

}