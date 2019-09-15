#pragma once

template<bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
void TetrisGame::Tetris<threaded, target_framerate, rng_seed>::start_game()
{
	do
	{
		::Tetris::GameModule game(this->rng);
		game_clock = Common::GameClock::Instance();
		videocore.start_async_display(&videocore, game);
		std::thread r_upd_th;
		bool physics_updated = true;
		if (threaded)
		{
			r_upd_th = std::thread([&, this]()
				{
					while (!game.lost)
					{
						if (physics_updated)
						{
							Renderer::MainRenderer::try_update(game, videocore.display_data);
							physics_updated = false;
							videocore.keep_displaying_data = true;
						}
					}
				});
		}
		else
		{
			Renderer::MainRenderer::try_update(game, videocore.display_data);
		}
		while (!game.lost)
		{
			int key = KeyboardModule::get_key();
			auto diff = game_clock.nano_time_diff();
			unsigned char n_burned;
			unsigned char y_burned[4];

			double time_diff = static_cast<double>(diff.count() / (1000.0 * 1000.0 * 1000.0));
			game.update(get_input(key), y_burned, n_burned, true, time_diff);
			if (threaded)
			{
				physics_updated = true;
			}
			if (!threaded)
			{
				Renderer::MainRenderer::try_update(game, videocore.display_data);
			}
			game_clock.reset_then();
		}
		//On game lost
		this->rng = game.current_seed.get_value();
		videocore.stop_displaying();

		if (threaded)
		{
			r_upd_th.join();
		}
	} while (!exit);

}
template<bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
void TetrisGame::Tetris<threaded, target_framerate, rng_seed>::threaded_update(bool& str_upd_done,
	const ::Tetris::GameModule& game, Renderer::MainRenderer::RenderData& rd)
{
	str_upd_done = false;
	Renderer::MainRenderer::try_update(game, rd);
	str_upd_done = true;
}
template<bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
TetrisGame::Tetris<threaded, target_framerate, rng_seed>::Tetris():
	rng(rng_seed), exit(false)
{
	videocore = VideoCore::VideoHandler();
}

template<bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
void TetrisGame::Tetris<threaded, target_framerate, rng_seed>::start()
{
	if (threaded)
	{
		std::thread new_thread = std::thread([this]() {start_game();});
		new_thread.join();
	}
	else
	{
		start_game();
	}
}
