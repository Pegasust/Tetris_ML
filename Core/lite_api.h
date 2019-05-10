#pragma once
#include "lite_renderer.h"
namespace LiteAPI
{
	//Returns the position of y if casted down
	double down_cast(LGEngine::TetrisBody const& body, LGEngine::TetrisField const& field);

	//Displacement: to the right is positive, to the left is negative.
	//Returns the displacement of the body after the cast
	//char side_cast(LGEngine::TetrisBody const& body, char const& displacement, LGEngine::TetrisField const& field);
	
	//Returns when lose or controlling_piece is reassigned or input is failed to execute.
	//Return value: number of burns (-1 if it's input error)
	//Use down_cast if you want to down_cast instead!
	char simulate(LiteGameModule::LiteModule& mod, bool& game_not_over, LiteGameModule::InputInfo& info);
};
