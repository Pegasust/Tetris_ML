#pragma once

#include "renderer_specs.h"
#include <vector>
#include <unordered_map>
#ifdef __cplusplus
#include <iostream>
#include <string>
#else
#include <stdio.h>
#endif
namespace Renderer
{

#ifdef USE_TXT_CONSOLE_RENDERER
	class StdTxtRenderer
	{
	public:
		//strings are separated from lines
		typedef std::vector<std::string> RenderStrings;
		static void clear_screen();
		static bool try_update(Tetris::GameModule const& mod);
		static char body_type_2_char(Tetris::BodyType const& body);
	private:
		//static std::vector
		static RenderStrings tetris_field_string(Tetris::TetrisField const& game_field,
			Tetris::TetrisBody const& controlling_piece);
		static RenderStrings tetris_upcoming_pieces(std::queue<Tetris::BodyType> coming_pieces);
		static RenderStrings tetris_scoreboard(Tetris::GameModule const& mod);
		static RenderStrings tetris_game_info(Tetris::GameModule const& mod);
	};
#endif
}