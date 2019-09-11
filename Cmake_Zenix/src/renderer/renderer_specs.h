#pragma once

#include "..//tetris/game_module.h"
#include <type_traits>
//Here is the list of macros that can be defined.
//If multiple flags are set, the program will *HOPEFULLY* run every flag set
//by the program

#define USE_TXT_CONSOLE_RENDERER
//#define USE_OPENCL_RENDERER //NOT IMPLEMENTED YET






//Here is the region which the design is set.

#ifdef USE_TXT_CONSOLE_RENDERER
//Top left is {0, 0}, bot right is {x, y} (unsigned)
namespace RendererExt
{
	constexpr float TXT_RENDERER_WIDTH = (Tetris::TetrisField::WIDTH * 4);
	constexpr float TXT_RENDERER_HEIGHT = (Tetris::TetrisField::HEIGHT * 3);
	constexpr float TXT_RENDERER_MID_WID = TXT_RENDERER_WIDTH / 2.0f;
	constexpr float TXT_RENDERER_MID_HEIGHT = TXT_RENDERER_HEIGHT / 2.0f;

	const int cx = Common::ZMath::round_nearest(TXT_RENDERER_WIDTH / 2.0f);
	const int cy = Common::ZMath::round_nearest(TXT_RENDERER_HEIGHT / 2.0f);
	//constexpr int CENTER_X = Common::ZMath::round_nearest(TXT_RENDERER_MID_WID);
	const char SHADOW_CHAR = '-';
	enum RenderPosition
	{
		TOP_LEFT,
		TOP_MID,
		TOP_RIGHT,
		MID_LEFT,
		CENTER,
		MID_RIGHT,
		BOT_LEFT,
		BOT_MID,
		BOT_RIGHT
	};
	template<typename T>
	class Math
	{
	public:
		inline void general_get(RenderPosition const& r_pos, T& ox, T& oy, int const& mod_x, int const& mod_y)
		{
			switch (r_pos)
			{//should have used macros, silly me
			case TOP_LEFT:
				get_top_left(ox, oy, mod_x, mod_y);
				break;
			case TOP_MID:
				get_top_mid(ox, oy, mod_x, mod_y);
				break;
			case TOP_RIGHT:
				get_top_right(ox, oy, mod_x, mod_y);
				break;
			case MID_LEFT:
				get_mid_left(ox, oy, mod_x, mod_y);
				break;
			case CENTER:
				get_center(ox, oy, mod_x, mod_y);
				break;
			case MID_RIGHT:
				get_mid_right(ox, oy, mod_x, mod_y);
				break;
			case BOT_LEFT:
				get_bot_left(ox, oy, mod_x, mod_y);
				break;
			case BOT_MID:
				get_bot_mid(ox, oy, mod_x, mod_y);
				break;
			case BOT_RIGHT:
				get_bot_right(ox, oy, mod_x, mod_y);
				break;

			}
		}
		
		inline void get_center(T& center_x, T& center_y, int const& obj_width, int const& obj_height)
		{
			center_x = static_cast<T>(cx - obj_width / 2);
			center_y = static_cast<T>(cy - obj_height / 2);
		}
		inline void get_top_left(T& out_x, T& out_y, int const& offset_x, int const& offset_y)
		{
			out_x = static_cast<T>(offset_x);
			out_y = static_cast<T>(offset_y);
		}
		inline void get_top_right(T& out_x, T& out_y, int const& offset_x, int const& offset_y)
		{
			out_x = static_cast<T>(static_cast<int>(TXT_RENDERER_WIDTH) - offset_x);
			out_y = static_cast<T>(offset_y);
		}
		inline void get_top_mid(T& out_x, T& out_y, int const& obj_width, int const& offset_y)
		{
			out_x = static_cast<T>(cx - obj_width / 2);
			out_y = static_cast<T>(offset_y);
		}
		inline void get_mid_left(T& out_x, T& out_y, int const& offset_x, int const& obj_height)
		{
			out_x = static_cast<T>(offset_x);
			out_y = static_cast<T>(cy - obj_height / 2);
		}
		inline void get_mid_right(T& out_x, T& out_y, int const& offset_x, int const& obj_height)
		{
			out_x = static_cast<T>(TXT_RENDERER_WIDTH - offset_x);
			out_y = static_cast<T>(cy - obj_height / 2);
		}
		inline void get_bot_left(T& out_x, T& out_y, int const& offset_x, int const& offset_y)
		{
			out_x = static_cast<T>(offset_x);
			out_y = static_cast<T>(TXT_RENDERER_HEIGHT - offset_y);
		}
		inline void get_bot_mid(T& out_x, T& out_y, int const& obj_width, int const& offset_y)
		{
			out_x = static_cast<T>(cx - obj_width / 2);
			out_y = static_cast<T>(TXT_RENDERER_HEIGHT - offset_y);
		}
		inline void get_bot_right(T& out_x, T& out_y, int const& offset_x, int const& offset_y)
		{
			out_x = static_cast<T>(TXT_RENDERER_WIDTH - offset_x);
			out_y = static_cast<T>(TXT_RENDERER_HEIGHT - offset_y);
		}

	};

	enum RenderMode
	{
		HORIZONTAL,
		VERTICAL
	};
	//The higher it is, the more in front this is. (Which means it will be rendered later)
	typedef unsigned int Priority;

#define DECLARE_RENDER_ENTRY(name, render_mode, render_position, mod_x, mod_y, priority) \
const RenderMode name##_MODE = render_mode;\
const RenderPosition name##_POSITION = render_position; \
constexpr int name##_MOD_X = mod_x; \
constexpr int name##_MOD_Y = mod_y; \
constexpr Priority name##_PRIORITY = priority;

	DECLARE_RENDER_ENTRY(GAME_FIELD, RenderMode::VERTICAL, 
		RenderPosition::CENTER, Tetris::TetrisField::WIDTH, 
		Tetris::TetrisField::HEIGHT, 4);
	//const RenderMode GAME_FIELD_MODE = VERTICAL; //It would be weird if otherwise
	//const RenderPosition GAME_FIELD_POSITION = CENTER;
	//constexpr int GAME_FIELD_MOD_X = Tetris::TetrisField::WIDTH;
	//constexpr int GAME_FIELD_MOD_Y = Tetris::TetrisField::HEIGHT;

	DECLARE_RENDER_ENTRY(UPCOMING_PIECES, RenderMode::VERTICAL,
		RenderPosition::MID_RIGHT, 5, (2 + 5 * Tetris::GameModule::N_PIECE_AHEAD), 0);
	/*
	1 2 345678 9 10
	#UPCOMING#	1
	#					 #		2
	#    XX			 #		3
	#	   XX         #		4
	#					 #		5
	#					 #		6
	#					 #		7
	#	   X			 #		8
	#	   XX			 #		9
	#	     X			 #		10
	#					 #		11
	#					 #		12
	#	  X			 #		13
	#	  X			 #		14
	#	  X			 #		15
	#	  X			 #		16
	########		17
	*/

	DECLARE_RENDER_ENTRY(SCORE_BOARD, RenderMode::VERTICAL,
		RenderPosition::TOP_LEFT, 5, 5, 3); //Includes current score, high score, tetris scored, rows burned, level

	DECLARE_RENDER_ENTRY(GAME_INFO, RenderMode::VERTICAL,
		RenderPosition::BOT_LEFT, 5, 5, 2); //Includes current_seed, fall_velocity, current position, etc.

#undef DECLARE_RENDER_ENTRY



	//===============Priority=================
	//class Prioritizing
	//{
	//public:
	//	template <typename ReturnType,typename Arg, typename... Types, typename std::enable_if<std::is_integral<Arg>::value>::type * = nullptr>
	//	inline ReturnType* get_order_of(int order, Arg obj_ptr, uint)
	//};
}
#endif