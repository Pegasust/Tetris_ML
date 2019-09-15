#include "game_renderer.h"
#include "../common/game_clock.h"
using namespace std;
#if defined DEBUG_DEFINED
double time_multiplier = 3.0;
#else
#endif
///Application entry point
int main(int argc, char** argv)
{
	Common::ZMath::UInt64RNG::RNGSeed rng_seed = 0xDEADBEEF;
	bool exit = false;
	while (!exit)
	{
		Tetris::GameModule game(rng_seed);
		Renderer::MainRenderer::RenderData rd;
		Renderer::MainRenderer::try_update(game, rd);		
		Renderer::MainRenderer::try_display(rd);
		Common::GameClock::Instance game_clock;

		while (!game.lost)
		{
			auto diff = game_clock.nano_time_diff();
			//std::this_thread::sleep_for(diff);
			unsigned char n_burned;
			unsigned char y_burned[4];
			double time_diff = static_cast<double>(diff.count() / 1000000.0)
#ifdef DEBUG_DEFINED
				* time_multiplier
#endif
				;
			//game.fast_deprecated_try_update(Tetris::Input::NONE, n_burned, y_burned, true, 
			//	static_cast<double>(diff.count())/1000000.0);
			game.update(Tetris::Input::NONE, y_burned, n_burned, true, time_diff);
			Renderer::MainRenderer::try_update(game, rd);
			Renderer::MainRenderer::try_display(rd);
			game_clock.reset_then();
		}
		rng_seed = game.current_seed.get_value();
	}
	return 0;
}