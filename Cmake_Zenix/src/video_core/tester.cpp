#include "video_core.h"
#include "../common/game_clock.h"

int main(int argc, char** argv)
{
	Common::ZMath::UInt64RNG::RNGSeed rng_seed = 0xDEADBEEF;
	VideoCore::VideoHandler handler;
	bool exit = false;
	while (!exit)
	{
		Tetris::GameModule game(rng_seed);
		Renderer::MainRenderer::try_update(game, handler.display_data);
		Common::GameClock::Instance game_clock;
		handler.start_async_display(&handler, game);
		while (!game.lost)
		{
			auto diff = game_clock.nano_time_diff();
			unsigned char n_burned;
			unsigned char y_burned[4];

			double time_diff = static_cast<double>(diff.count() / 1000000.0);
			game.update(Tetris::Input::NONE, y_burned, n_burned, true, time_diff);
			Renderer::MainRenderer::try_update(game, handler.display_data);
			game_clock.reset_then();
		}
		rng_seed = game.current_seed.get_value();
		handler.stop_displaying();
	}
	return 0;
}