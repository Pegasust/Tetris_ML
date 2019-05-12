// Zenix_ML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Zenxis.h"
#include <conio.h>
int main()
{
	auto x = TetrisML::SeedHelper::generate_random_seed();
	TetrisML::Seed s(*(TetrisML::Seed*) &x);
	TetrisML::Zenxis zenxis(s);
	//std::cout.precision(std::numeric_limits<double>::max_digits10);
	std::cout.precision(5);
	
	std::ofstream file_output("C:\\Users\\Pegasus\\source\\repos\\Pegasust\\Tetris_ML\\Core\\Release\\zenxis_experiment.txt");
	file_output.precision(std::numeric_limits<double>::max_digits10);
	if (!file_output)
	{
		std::cerr << "Can't open file zenxis_experiment.txt" << std::endl;
		return -1;
	}
	bool exit = false;
	do
	{
		//zenxis.big_bang();
		zenxis.experiment(3, file_output);
		if (_kbhit())
		{
			int key = _getch();
			if (key == 27) //ESCAPE KEY
			{
				exit = true;
				break;
			}
		}
	} while (!exit);
	file_output.close();
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
