#include "game_renderer.h"
#include "../common/game_clock.h"
using namespace std;
///Application entry point
int main(int argc, char** argv)
{
	Common::ZMath::UInt64RNG::RNGSeed rng_seed = 0xDEADBEEF;
	bool exit = false;
	while (!exit)
	{
		Tetris::GameModule game(rng_seed);
		Common::GameClock::Instance game_clock;
		Renderer::StdTxtRenderer::try_update(game);
		while (!game.lost)
		{
			auto diff = game_clock.nano_time_diff();
			std::this_thread::sleep_for(diff);
			unsigned char n_burned, y_burned;

			game.try_update(Tetris::Input::NONE, n_burned, y_burned, true, 
				static_cast<double>(diff.count())/1000000.0);
			Renderer::StdTxtRenderer::try_update(game);
			game_clock.reset_then();
		}
	}
	return 0;
}