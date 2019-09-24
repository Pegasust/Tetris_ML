#pragma once

#include <chrono>
#include <stdint.h>
#include <thread>
#include <unordered_map>
#include <experimental/filesystem>
#include "../common/keyboard_input.h"
#include "../video_core/video_core.h"
#include "../tetris/game_module.h"
#include "../common/zmath.h"
#include "../common/rng.h"
#include "../common/game_clock.h"
#include "../verbosity/framerate.h"



namespace TetrisGame
{
	template<bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
	class Tetris
	{
		static constexpr ::Tetris::Input get_input(const int& keypress)
		{
			std::unordered_map<int, ::Tetris::Input> key2inp =
			{
				{-1, ::Tetris::Input::NONE},
				{80, ::Tetris::Input::DOWN}, //Down arrow
				{72, ::Tetris::Input::UP}, //Up arrow
				{75, ::Tetris::Input::LEFT}, //Left arrow
				{77, ::Tetris::Input::RIGHT }, //Right arrow
				{32, ::Tetris::Input::CAST_DOWN}, //space
				{122, ::Tetris::Input::ROTATE_BACK}, //z button
				{120, ::Tetris::Input::ROTATE_FORTH } //x button
			};
			if (key2inp.find(keypress) == key2inp.end()) return ::Tetris::Input::NONE;
			return key2inp.at(keypress);
		}
		static constexpr uint64_t frametime_micros = (1000 * 1000 / target_framerate);
		using KeyboardModule = Common::KeyboardListener<false, frametime_micros/3*2, std::chrono::microseconds>;
		void start_game();
	public:
		Common::ZMath::UInt64RNG::RNGSeed rng;
		bool exit;
		VideoCore::VideoHandler videocore;
		Common::GameClock::Instance game_clock;
		Tetris();
		void start();
	private:
		static void threaded_update(bool& str_upd_done,
			const ::Tetris::GameModule& game, Renderer::MainRenderer::RenderData& rd);
	};
}

#include "tetris_game.ipp"