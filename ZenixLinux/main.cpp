#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <limits>
#include "ZConsole.h"

int main(int argc, char ** argv)
{
	////Run it in xterm by default
	//if (execl("usr/bin/xterm", "xterm", "-e", argv[0], (char*)NULL))
	//{
	//	std::perror("execl");
	//	std::cout << "Cannot open xterm." << std::endl;
	//	return 1;
	//}
	auto x = TetrisML::SeedHelper::generate_random_seed();
	TetrisML::Seed s(*(TetrisML::Seed*) & x);
	TetrisML::Zenxis zenxis(s);

#ifdef _SAVING_DIRECTORY_
	std::string save_directory = _SAVING_DIRECTORY_;
#else
	std::string save_directory = "./coefs";	
#endif
	if (!isDirExist(save_directory))
	{
		if (!makePath(save_directory))
		{
			std::cout << "Error trying to create directory: " << save_directory << std::endl;
			return -1;
		}
	}
	save_directory += "/";
	save_directory += std::to_string(TetrisML::N_INDIVIDUALS_PER_GEN);
	save_directory += "_";
	save_directory += std::to_string(TetrisML::EXPERIMENTS_PER_LIFE);
	save_directory += "_";
	//get time as string
	auto t = std::time(nullptr);
	auto tm = std::localtime(&t);
	const uint32_t SIZE = 50;
	char time[SIZE];
	strftime(time, sizeof(time), "%d-%m-%Y %H-%M-%S", tm);
	//oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	std::string str = std::string(time);

	save_directory += str;
	save_directory += ".txt";
	std::ofstream file_output(save_directory);
	file_output.precision(std::numeric_limits<double>::max_digits10);
	if (!file_output)
	{
		std::cerr << "Can't open " << save_directory << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Writing in path " << save_directory << std::endl;
	}
#define PRINT_TETRIS_ML_PROPERTY(prop) std::cout<<#prop<<": "<<std::to_string(prop)
#ifdef MULTI_THREAD
	PRINT_TETRIS_ML_PROPERTY(TetrisML::THREADS);
	std::cout << '\t';
#endif
	PRINT_TETRIS_ML_PROPERTY(TetrisML::N_INDIVIDUALS_PER_GEN);
	std::cout << '\t';
	PRINT_TETRIS_ML_PROPERTY(TetrisML::EXPERIMENTS_PER_LIFE);
	std::cout << '\t';
	PRINT_TETRIS_ML_PROPERTY(TetrisML::SURVIVORS_PER_GEN);
	std::cout << '\t';
	PRINT_TETRIS_ML_PROPERTY(TetrisML::N_COMPLETE_RANDOM);
	std::cout << std::endl;
#undef  PRINT_TETRIS_ML_PROPERTY
#ifdef _DEBUG
	std::cout << "Debug mode." << std::endl;
#endif
	bool exit = false;
	do
	{
		//zenxis.big_bang();
		zenxis.experiment(TetrisML::EXPERIMENTS_PER_LIFE, file_output);
		//No implementation for a way to exit yet.bones of 
		//bool reply_once = false;
		//while (_kbhit())
		//{
		//	int key = _getch();
		//	if (key == 27) //ESCAPE KEY
		//	{
		//		exit = true;
		//		break;
		//	}
		//	else if (!reply_once) //Havne't update on whether Zenix people are still living
		//	{
		//		//reply
		//		std::cout << "currently at G" << zenxis.generation << std::endl;
		//		reply_once = true;
		//	}
		//}
	} while (!exit);
	file_output.close();
    return 0;
}