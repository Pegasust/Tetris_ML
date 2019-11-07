

namespace MLInterface
{
	/*
	ML's input or the game's output will be:
		- Game's width and height (quite straight forward)
		- The whole grid of Tetris (array) (might be reduced) (essential)
		- The current score, highest score (quite straight forward)
		- Whether the game is lost (not too essential)
		- Upcoming pieces types (array) (straight forward but not essential)
		- The data of controlling piece (might be merged with
			Tetris grid): position, rotation, bodytype (essential)
		- Total number of rows burned (not essential, not straight forward)
		- Total number of Tetris scored (not essential, not straight forward)
		- Number of max stage reached (might not be
			necessary)
		- Whether the last action caused the piece to staticize
			(not straight forward, might be necessary)
		- Last delta time
			(if delta time is used against the player, or game module
			is not frame-based)
		- Number of rows burned from the last update
			(quite straight forward, not necessary)
		- Array of y-positions where burns happened
			(quite straight forward, not necessary)
		- Current level	(straight forward, necessary)

	ML's outputs:
		- Tetris::Input [0,7]
	*/

	constexpr int
		W = 0,
		H = 1,
		CUR_SCORE = 2,
		HI_SCORE = 3,
		LOST = 4,
		UPCOMING_PIECE_TYPE0 = 5,
		CONTROLLING_PIECE = UPCOMING_PIECE_TYPE0 + 1 +
		TetrisAPI::TetrisEngine::N_PIECE_AHEAD,
		CONTROLLING_PIECE_POS = CONTROLLING_PIECE + 1,
		CONTROLLING_PIECE_ROT = CONTROLLING_PIECE_POS + 1,
		CONTROLLING_PIECE_BODY_TYPE = CONTROLLING_PIECE_ROT + 1,
		TOTAL_BURNS = CONTROLLING_PIECE_BODY_TYPE + 1,
		TOTAL_TETRIS = TOTAL_BURNS + 1,
		N_MAX_STAGE = TOTAL_TETRIS + 1,
		CONTROLLING_PIECE_STATICIZED = N_MAX_STAGE + 1,
		LAST_DELTATIME = CONTROLLING_PIECE_STATICIZED + 1,
		LAST_BURNS = LAST_DELTATIME + 1,
		LAST_BURN_POS = LAST_BURNS + 1,
		CURRENT_LEVEL = LAST_BURN_POS + Tetris::T_COLLIDER_HEIGHT,
		GAME_GRID = CURRENT_LEVEL + 1,
		OUTPUT_LENGTH = GAME_GRID + 1 + Tetris::TetrisField::LENGTH + 1;

	constexpr void get_game_output(const TetrisAPI::TetrisEngine& game,
		double outputs[])
	{
		// Width and Height
		outputs[W] = Tetris::TetrisField::WIDTH;
		outputs[H] = Tetris::TetrisField::HEIGHT;
		outputs[CUR_SCORE] = game.score;
		outputs[HI_SCORE] = game.highest_score;
		outputs[LOST] = static_cast<double>(game.lost);


	}
}