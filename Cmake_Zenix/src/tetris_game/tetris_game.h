#pragma once

#include <chrono>
#include <stdint.h>
#include <thread>
#include <unordered_map>
#include "../common/keyboard_input.h"
#include "../video_core/video_core.h"
#include "../tetris/game_module.h"
#include "../common/zmath.h"
#include "../common/rng.h"
#include "../common/game_clock.h"

namespace TetrisGame
{
	template<bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
	class Tetris
	{
		constexpr ::Tetris::Input get_input(const int& keypress)
		{
			std::unordered_map<int, ::Tetris::Input> key2inp =
			{
				{-1, ::Tetris::Input::NONE},
				{80, ::Tetris::Input::DOWN}, //Down arrow
				{72, ::Tetris::Input::UP}, //Up arrow
				{75, ::Tetris::Input::LEFT}, //Left arrow
				{77, ::Tetris::Input::RIGHT }, //Right arrow
				{32, ::Tetris::Input::CAST_DOWN}, //space
				{122, ::Tetris::Input::ROTATE} //z button
			};
			return key2inp.at(keypress);
		}
		static constexpr uint64_t frametime_micros = (1000 * 1000 / target_framerate);
		using KeyboardModule = Common::KeyboardListener<false, frametime_micros, std::chrono::microseconds>;
		void start_game();
	public:
		Common::ZMath::UInt64RNG::RNGSeed rng;
		bool exit;
		VideoCore::VideoHandler videocore;
		Common::GameClock::Instance game_clock;
		Tetris();
		void start();
	};
}

#include "tetris_game.ipp"