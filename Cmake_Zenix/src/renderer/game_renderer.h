#pragma once

#include "renderer_specs.h"
#include <vector>
#include <unordered_map>
#include <stdlib.h>
#include <array>
#ifdef __cplusplus
#include <iostream>
#include <string>
#endif
namespace Renderer
{
	/**Renderers must be implemented with the following contents:
	- Encapsulated within ifdef
	- public typedef: RenderData
	- using MainRenderer = <RendererName>
	- static void clear_screen() //As means to clear the current display
	- static bool try_update(const ::Tetris::GameModule&, RenderData&)
	- static bool try_display(const RenderData&)
	*/
#ifdef USE_TXT_CONSOLE_RENDERER
	class StdTxtRenderer
	{
	public:
		//strings are separated from lines
		typedef std::vector<std::string> RenderStrings;
		typedef std::string RenderData;
		static void clear_screen();
		static bool try_update(Tetris::GameModule const& mod, RenderData& new_data);
		static bool try_display(const RenderData& data);
		static char body_type_2_char(Tetris::BodyType const& body);
		static void assign_render_string(Tetris::GameModule const& mod, const bool& shadow, std::string& str);
	private:
		typedef int CharPosition;
		using RendererMath = RendererExt::Math<CharPosition>;
		//static std::vector
		//This supports shadowing
		static RenderStrings tetris_field_string(Tetris::GameModule const& mod);
		//Does not support shadowing
		static RenderStrings tetris_field_string(Tetris::TetrisField const& game_field,
			Tetris::TetrisBody const& controlling_piece);
		static RenderStrings tetris_upcoming_pieces(std::queue<Tetris::BodyType> coming_pieces);
		static RenderStrings tetris_upcoming_pieces(Tetris::GameModule const& mod);
		static RenderStrings tetris_scoreboard(Tetris::GameModule const& mod);
		static RenderStrings tetris_game_info(Tetris::GameModule const& mod);
		typedef RenderStrings (*StringGetFunction)(Tetris::GameModule const&);
		typedef std::unordered_map<RendererExt::Priority, StringGetFunction> get_func_map;
	};
	using MainRenderer = StdTxtRenderer;
#endif
}