#pragma once
#include "game_core.h"
//This one uses std::cout to render, should be able to support many platforms.
namespace CrossRenderer
{
	typedef int RInt;
	typedef unsigned int RUint;
	struct RenderUnit
	{
		static const RUint DOWNSCALE = 8;
		static const RUint WIDTH = 1280 / DOWNSCALE;
		static const RUint HEIGHT = 720 / DOWNSCALE;
		static const RUint SIZE = WIDTH * HEIGHT;
		static const RUint X_OFFSET = 3, Y_OFFSET = 3;
		char screen_text[SIZE];
		GameFeatures::TetrisPiece::PieceType* abstract_screen;
		RenderUnit(GameCore::GameRule& rule);
		void init_screen_text(GameCore::GameInfo& info);
		void update_screen_text(GameCore::GameInfo& info);
	};
}