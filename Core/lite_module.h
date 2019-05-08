#pragma once
#include "tetris_body.h"
#include <queue>

using namespace std;
namespace LiteGameModule
{
	typedef unsigned long long Level;
	constexpr Level MAX_LEVEL = std::numeric_limits<Level>::max();
	constexpr Level MIN_LEVEL = std::numeric_limits<Level>::min();;
	constexpr double v_gravity0 = 0.3333; // blocks per second
	constexpr double v_gravity_max = 98.0; //blocks per second
	constexpr double seconds_per_update = 1.0 / (v_gravity_max+ 10.0);
	//constexpr double expr_s_per_up = seconds_per_update;
	constexpr double v_gravity_at(Level level);
	const unsigned long long required_rows0 = 7;
	constexpr unsigned long long required_rows(const Level& level);
	enum Input
	{
		DOWN, //immediately places the block downwards
		LEFT,
		RIGHT,
		ROTATE, //Rot++
		NONE
	};
	struct InputInfo
	{
		Input input;
		int n_frames_update;
	};


	struct LiteModule
	{
		typedef LiteGameEngine::TetrisBody<10, 20> LTetrisBody;
		Level current_level = MIN_LEVEL;
		unsigned long long n_level_up_rows = required_rows(current_level); //The amount of rows (scaled) needs to be burned until level up
		const unsigned char WIDTH = 10, HEIGHT = 20; //Field specifications
		const unsigned char N_PIECE_AHEAD = 3;
		LiteGameEngine::TetrisField<10, 20> field;
		TMath::GameRNG::RNGSeed current_seed;
		LiteGameEngine::TetrisBody<10, 20> controlling_piece;
		std::queue<LiteGameEngine::BodyType> coming_pieces;
		double score = 0.0;
		double highest_score = 0.0;
	public:
		bool try_update(InputInfo& info); //return false when game over
		LiteModule(const TMath::GameRNG::RNGSeed& initial_seed);
		//LiteModule();
	private:
		//LiteGameEngine::TetrisBody<10, 20>* create_new_body(const TMath::GameRNG::RNGSeed & seed);
	};
}