/**
* OLD Purpose: To easily see what information is intended for normal players
* to see, and the output potentials
* NEW: To easilly communicate between the AI's output and the game's inputs
*
*/
#include "tetris_core.h"
#include "../common/game_clock.h"
#include <limits>
namespace TetrisAPI
{
	/*
	Time will keep on running as long as the game is not lost.
	Delta time is calculated from the difference in time from last
	game update to this call of game update (which includes time of
	game physics update)
	* This engine will also keeps track of the inputs flooded into
	update() function.
	*/
	class TetrisExtendedEngine
	{
	protected:
		TetrisEngine engine;
		Common::GameClock::Instance game_clock;
		bool time_initialized;
        Tetris::InputCollection input_collection;
	public:
		/*initialize with a random seed*/
		TetrisExtendedEngine();
		/*initialize with a given seed*/
		TetrisExtendedEngine(const unsigned long long& seed);
		/*
		Outputs:
			burn_y[4]: the meaningful information is from 0 to n_burned -1.
				elmeents from n_burned to 4 is preserved and might be inaccurate.
		*/
		Tetris::TetrisField& update(
			//inputs
			const Tetris::Input& input,
			//outputs (including the returning field
			bool& update_staticized_last_piece, //whether this update
									// staticized the last controlling piece
			TetrisBody& controlling_piece,
			unsigned char burn_y[4], //The y-position that were burned
									 //in the last update
			unsigned char& n_burned, //The total amount of number burned
									 //from the last update
			double& delta_time // (Output) Difference in time since last update in seconds
		);
		
		void reset(const unsigned long long& seed);
		inline void reset() {
            int random = rand();
            reset(random);
		}
	};
}