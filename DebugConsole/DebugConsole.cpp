// DebugConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "lite_renderer.h"
#include <thread>

//GameCore::GameModule* tetris_ptr;
namespace Module = LiteGameModule;
namespace Engine = LGEngine;
//using namespace std;
void child_func(Module::LiteModule& l_module)
{
	std::this_thread::sleep_for(std::chrono::duration<double>(Module::seconds_per_update));
	//capture input
	Module::InputInfo info = { Module::NONE, 1 };
	if (l_module.try_update(info))
	{
		//render

	}
}

int main()
{
	Module::LiteModule l_module = Module::LiteModule(TMath::GameRNG::generate_random_seed());

	child_func(l_module);
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
