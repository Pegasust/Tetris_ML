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
#include <iostream>
#include <string>
#include "lite_module.h"
namespace Renderer
{
	const unsigned int CHAR_FIELD_WIDTH = (LGEngine::TetrisField::WIDTH + 1);
	const unsigned int CHAR_FIELD_LEN = (CHAR_FIELD_WIDTH * LGEngine::TetrisField::HEIGHT) + 1; //1 is the null-term
	const std::string TETRIS_CHARS = " 1ZSOTLJ#";
	//TODO: CHANGE THESE TO DIRECTLY MODIFY A VALUE (add to parameter an & instead of returning a string)
	std::string get_score_info(const LiteGameModule::LiteModule& mod);
	std::string get_upcoming_str(const LiteGameModule::LiteModule& mod);
	std::string get_renderer_string(const LiteGameModule::LiteModule& mod);
	std::string get_field_str(const LiteGameModule::LiteModule& mod);
	void clear_console();
	class LiteRenderUnit
	{
	public:
		unsigned long long frames = 0;
		std::string field;
		std::string info;
		std::string upcoming;
		void update_string(const LiteGameModule::LiteModule& mod);
		void render();
		LiteRenderUnit(const LiteGameModule::LiteModule& mod);
	};
}