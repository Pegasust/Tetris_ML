#pragma once
//#define USE_CONIO //VC++ does not support conio's clrscr().

#if not defined(USE_SYSTEM_CLEAR) && not defined(USE_CONIO)
#define USE_SYSTEM_CLEAR
#endif

#ifdef __cplusplus__
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#include <string>
#include "lite_module.h"
namespace Renderer
{
	const unsigned int CHAR_FIELD_LEN = ((LGEngine::TetrisField::WIDTH + 1) * LGEngine::TetrisField::HEIGHT) + 1;
	const std::string TETRIS_CHARS = " 1ZSOTLJ#";
	std::string get_score_info(const LiteGameModule::LiteModule& mod);
	std::string get_upcoming_str(const LiteGameModule::LiteModule& mod);
	std::string get_renderer_string(const LiteGameModule::LiteModule& mod);
	std::string get_field_str(const LiteGameModule::LiteModule& mod);
	void clear_console();
	class RenderUnit
	{
	public:
		std::string field;
		std::string info;
		std::string upcoming;
		void update_string(const LiteGameModule::LiteModule& mod);
		void render();
		RenderUnit(const LiteGameModule::LiteModule& mod);
	};
}