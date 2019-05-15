// ZenxPlayer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ZenxPlayer.h"

Renderer::RenderUnit* rdr;
//bool rdr_is_sim = false;
//TetrisML::Zenx* player;
void print_dna_array(TetrisML::DNAArray& zenx)
{
	for (unsigned int i = 0; i < TetrisML::DNAArray::ARR_LENGTH; i++)
	{
		std::cout << zenx.arr[i];
		if (i != TetrisML::DNAArray::ARR_LENGTH - 1)
		{
			std::cout << ",";
		}
	}
}


void render_move(LiteGameModule::LiteModule const& mod)
{
	rdr->update_string(mod);
	Renderer::clear_console();
	rdr->render();
}

void render_sim(LiteGameModule::LiteModule const& mod)
{
#if _DEBUG
	rdr->update_string(mod);
	Renderer::clear_console();
	rdr->render();
#endif
}

void render_ext()
{

}
int main()
{
	std::cout.precision(std::numeric_limits<double>::max_digits10);
	std::string format_string = " {";
	for (unsigned int i = 0; i < TetrisML::DNAArray::ARR_LENGTH; i++)
	{
		format_string += "%lf";
		if (i != TetrisML::DNAArray::ARR_LENGTH - 1)
		{
			format_string += ",";
		}
	}
	format_string += "}";
	std::cout << format_string << std::endl;
	TMath::GameRNG::RNGSeed rdr_seed[1] = { 0xDEADBEEF };
	LiteGameModule::LiteModule rdr_mod(rdr_seed);
	rdr = new Renderer::RenderUnit(rdr_mod);
	TMath::GameRNG::RNGUnion init_seed[1] = { TMath::GameRNG::generate_random_seed() };

	do
	{
		TMath::GameRNG::xorshift64(init_seed);
		std::cout << "Enter coefficient struct (DNA). Format: \"{d0, d1, d2, d3, d4...\}\"" << std::endl;

		TetrisML::DNAArray zenx; //= { 0.0, 0.0, 0.0, 0.0, 0.0 };
		scanf_s(format_string.c_str(), &zenx.arr[0], &zenx.arr[1], &zenx.arr[2], &zenx.arr[3], &zenx.arr[4]);
		print_dna_array(zenx);
		TetrisML::Zenx player = TetrisML::Zenx(zenx);
	

		player.experiment(init_seed, render_move, render_sim);
		std::cout << "Press escape to stop. Press other keys to continue." << std::endl;
		int key = _getch();
		if (key == 27) //Escape;
			break;
	} while (1);
	delete rdr;
	//delete player;
}