//#pragma once
//#include "game_core.h"
//#include <stdio.h>
//#include <unordered_set>
//#define _WINDOWS_CONSOLE_
//#ifdef _WINDOWS_CONSOLE_
//#include <Windows.h>
//#endif
//namespace Renderer
//{
//	typedef int R_Int;
//	typedef unsigned int R_Uint;
//
//	const wchar_t TETRIMINO_DISPLAY_CHAR[] = L" XMNCOZS#";
//	const R_Uint BOUNDARY_INDEX = 8;
//	const GameFeatures::TetrisPiece::PositionIndex WIDTH = 50, HEIGHT = 100;
//	const GameFeatures::TetrisPiece::PositionIndex X_OFFSET = 5, Y_OFFSET = 10;
//	struct RenderUnit
//	{
//		GameFeatures::TetrisPiece::PieceType* abstract_screen;
//		wchar_t* screen_text;
//
//		RenderUnit(GameCore::GameRule& rule);
//	};
//	//Windows rendering
//	void draw_to_console(GameCore::GameInfo& info);
//	static R_Uint xy2i(R_Uint x, R_Uint y)
//	{
//		return y * WIDTH + x;
//	}
//}