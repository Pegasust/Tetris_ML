#pragma once
//This header is responsible for input from the machine learning to the game
#include "lite_api.h"

namespace ZenixAgent
{
	namespace TModule = LiteGameModule;
	namespace TEngine = LGEngine;
	typedef TModule::InputInfo UpdateInfo;

	struct RawObservation
	{
		double
			score,
			bulkiness, //c
			burn,
			aggregate_height, //c
			holes //c
			;
	};

	bool attempt_move_piece(TModule::LiteModule& mod, const char& x,
		const unsigned char& y, TEngine::Rotation const& rotation, unsigned char & burned);

	bool apply_moveset(TModule::LiteModule& mod, const char& x,
		const unsigned char& y, TEngine::Rotation const& rotation);

	bool can_move_piece(const TModule::LiteModule& mod, char& x,
		unsigned char& y, TEngine::Rotation const& rotation);

	////Sum of absolute difference of adjacent pieces
	//double calculate_builkiness(const TModule::LiteModule& mod);
	////Sum of height in each column
	//double calculate_aggregate_height(const TModule::LiteModule& mod);
	////How many holes in the field
	//double calculate_holes(const TModule::LiteModule& mod);

	//Calculate the observation of the field (exclusive of the controlling piece ofc)
	//This is without the number of rows cleared (burns) and score
	RawObservation get_raw_observation(const TEngine::TetrisField& field);
}