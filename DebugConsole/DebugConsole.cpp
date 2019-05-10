// DebugConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define USER_TEST
//#define TIME_BASED
#ifndef USER_TEST
#define COMMAND_TEST
#endif // !USER_TEST



#include <iostream>
#include "lite_renderer.h"
#include <thread>
#include <conio.h>
#include <stdio.h>
#include "keyboard_input.h"
//GameCore::GameModule* tetris_ptr;
namespace Module = LiteGameModule;
namespace Engine = LGEngine;
//using namespace std;


Renderer::RenderUnit* r_unit;
unsigned long long frames = 0;

bool child_func(Module::LiteModule& l_module)
{
	//std::cout << "updating..." << std::endl;
	//std::this_thread::sleep_for(std::chrono::duration<double>(Module::seconds_per_update));
	//capture input
	int input;
#ifndef TIME_BASED
	input = _getch();
#else
	auto now = std::chrono::high_resolution_clock::now();
	while (!_kbhit())
	{
		if (((std::chrono::high_resolution_clock::now() - now).count() ) > (long)(Module::seconds_per_update))
		{
			break;
		}
	}
	input = _getch();
#endif
	Module::Input i;
	switch (input)
	{
		case keyboard_input::DOWN_ARROW:
			i = Module::DOWN;
			break;
		case keyboard_input::RIGHT_ARROW:
			i = Module::RIGHT;
			break;
		case keyboard_input::LEFT_ARROW:
			i = Module::LEFT;
			break;
		case keyboard_input::ENTER:
			i = Module::ROTATE;
			break;
		default:
			i = Module::NONE;
			break;
	}
	Module::InputInfo info = { i, 1 };
	if (l_module.try_update(info)) //Not lost
	{
		//render
//#ifdef _DEBUG
//		std::cout << "not lost yet. \0" << std::endl;
//#endif
		Renderer::clear_console();
		r_unit->update_string(l_module);
		r_unit->render();
		std::cout << "Frames: " << std::to_string(++frames) << std::endl;
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

int main()
{
	TMath::GameRNG::RNGSeed rng[1] = { TMath::GameRNG::generate_random_seed() };
	Module::LiteModule l_module = Module::LiteModule(rng);
	std::cout << "Created l_module." << std::endl;
	r_unit = new Renderer::RenderUnit(l_module);
	std::cout << "Successfully created r_unit." << std::endl;
	//Renderer::clear_console();
	r_unit->render();
	//_getch();

#ifdef USER_TEST
	while (child_func(l_module)) {};
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


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
