// Zenix_ML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Zenxis.h"
#include <conio.h>
#include <iomanip>

int main()
{
	auto x = TetrisML::SeedHelper::generate_random_seed();
	TetrisML::Seed s(*(TetrisML::Seed*) &x);
	TetrisML::Zenxis zenxis(s);
	//std::cout.precision(std::numeric_limits<double>::max_digits10);
	std::cout.precision(5);
	//std::ofstream file_output("C:\\Users\\Pegasus\\source\\repos\\Pegasust\\Tetris_ML\\Core

	//get time as string

	auto t = std::time(nullptr);
	//auto tm = *std::localtime_s(&t);
	tm tm;
	localtime_s(&tm, &t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	std::string str = oss.str();

	std::string file_name = "D:\\Senior Project\\Senior Project proofs\\FINAL\\coefs\\";
	file_name += std::to_string(TetrisML::N_INDIVIDUALS_PER_GEN);
	file_name += "_";
	file_name += std::to_string(TetrisML::EXPERIMENTS_PER_LIFE);
	file_name += "_";
	file_name +=
#ifdef _DEBUG
		"d_zenxis_";
#else

		"r_zenxis_";
#endif
	file_name += str;
	file_name += ".txt";
	std::ofstream file_output(file_name);
	file_output.precision(std::numeric_limits<double>::max_digits10);
	if (!file_output)
	{
		std::cerr << "Can't open "<<file_name << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Writing in path " << file_name << std::endl;
	}
	bool exit = false;
	do
	{
		//zenxis.big_bang();
		zenxis.experiment(TetrisML::EXPERIMENTS_PER_LIFE, file_output);
		bool reply_once = false;
		while (_kbhit())
		{
			int key = _getch();
			if (key == 27) //ESCAPE KEY
			{
				exit = true;
				break;
			}
			else if(!reply_once) //Havne't update on whether Zenix people are still living
			{
				//reply
				std::cout << "currently at G" << zenxis.generation << std::endl;
				reply_once = true;
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
