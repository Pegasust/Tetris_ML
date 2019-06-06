#pragma once
#include "lite_tetris_field.h"
#include <queue>
#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#endif

#define SCALE_BURN //makes levelling up easier and more satisfying by scaling burn by current level
#define SCALE_SCORE //encourages going to higher level by scaling score by current level
#ifdef USE_INITIAL_SETTINGS
#define PUNISH_USELESS_MOVE //minus score for every frame updated
#else
#endif
using namespace std;
namespace LiteGameModule
{
	typedef uint32_t Level;
	constexpr Level MAX_LEVEL = std::numeric_limits<Level>::max();
	constexpr Level MIN_LEVEL = std::numeric_limits<Level>::min();;
	constexpr double v_gravity0 = 10.0; // blocks per second
	constexpr double v_gravity_max = 98.0; //blocks per second
	constexpr double seconds_per_update = 1.0 / (v_gravity_max+ 21.0);
	constexpr double dragdown_gravity_mult = 4.0;
	//constexpr double expr_s_per_up = seconds_per_update;
	constexpr double v_gravity_at(Level level)
	{
		//Lienar scaling: m * x + b
		return (v_gravity_max - v_gravity0) / static_cast<double>(MAX_LEVEL - MIN_LEVEL) // slope
			* level + v_gravity0;
	}
	const unsigned long long required_rows0 = 7;
	constexpr unsigned long long required_rows(const Level& level)
	{
		return required_rows0 + (level / 3);
	}
	enum Input
	{
		//0
		DOWN, //immediately places the block downwards
		//1
		LEFT,
		//2
		RIGHT,
		//3
		ROTATE, //Rot++
		//4
		DRAG_DOWN,
		//5
		NONE
	};
	struct InputInfo
	{
		Input input;
		int n_frames_update;
	};


	class LiteModule
	{
	public:
		typedef LGEngine::TetrisBody LTetrisBody;
		Level current_level;
		unsigned long long n_level_up_rows; //The amount of rows (scaled) needs to be burned until level up
		const unsigned char WIDTH = 10, HEIGHT = 20; //Field specifications
		const unsigned char N_PIECE_AHEAD = 3;
		LGEngine::TetrisField field;
		TMath::GameRNG::RNGSeed current_seed;
		LGEngine::TetrisBody controlling_piece;
		std::queue<LGEngine::BodyType> coming_pieces;
		double score;
		double highest_score;
	public:
		bool try_update(InputInfo& info); //return false when game over
		bool try_flex_update(const Input & input, 
			unsigned char& n_burned,
			unsigned char& burn_y,
			bool& useful_input, const bool& reassign_controlling_piece);
		LiteModule(TMath::GameRNG::RNGSeed initial_seed[1]);
		//returns distance
		static double down_cast(const LGEngine::TetrisBody& body, LGEngine::TetrisField const& field);
		//LiteModule();
	private:
		//LGEngine::TetrisBody<10, 20>* create_new_body(const TMath::GameRNG::RNGSeed & seed);
		static unsigned char scaled_burn(const unsigned char& burned, const Level& current_level);
		static constexpr double scaled_score_increment(const unsigned char& scaled_burn, const Level& current_level);
		static constexpr double useless_move_decrement(const unsigned char& current_level);
	};
}