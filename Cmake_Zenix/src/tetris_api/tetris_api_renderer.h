#pragma once
#include "tetris_core.h"
#include <unordered_map>
#include "../common/zmath.h"

namespace TetrisAPI
{
	using namespace Tetris;

	//is solid or not for each tile
	typedef bool ReducedField[TetrisField::LENGTH];
	constexpr unsigned int SEGMENTED_FIELD_LENGTH =
		Common::ZMath::compile_time_ceil(
			static_cast<float>(TetrisField::LENGTH) /
			static_cast<float>(
				// number of bits in unsigned long
				sizeof(unsigned long) * 8));
	// I acknowledge that some platforms will
	// put long as 32 or 64 bit value; however, it should
	// still work. My intend is to create such structure
	// that can be processed the quickest in the same amount of
	// allocated memory. Each bit represents whether that position
	// of the field is solid.
	// Segmented Reduced Field is similar to ReducedField, but is
	// compressed to integer values.
	typedef unsigned long SegmentedReducedField[
		SEGMENTED_FIELD_LENGTH
	];

	template<class out_T>
	void render_field(const TetrisField& field,
		out_T render_output[TetrisField::HEIGHT][TetrisField::WIDTH],
		const std::unordered_map<BodyType, out_T>& translator)
	{
		for (unsigned char i = 0; i < TetrisField::HEIGHT; i++)
		{
			for (unsigned char j = 0; j < TetrisField::WIDTH; j++)
			{
				render_output[i][j] = translator.at(
					field.collider[TetrisField::xy2i(i, j)]);
			}
		}
	}

	template<class out_T>
	void render_field(const ReducedField& field,
		out_T render_output[TetrisField::HEIGHT][TetrisField::WIDTH],
		const std::unordered_map<bool, out_T>& translator)
	{
		for (unsigned char i = 0; i < TetrisField::HEIGHT; i++)
		{
			for (unsigned char j = 0; j < TetrisField::WIDTH; j++)
			{
				render_output[i][j] = translator.at(
					field[TetrisField::xy2i(i, j)]);
			}
		}
	}


	template<class out_T>
	void render_field(const SegmentedReducedField& field,
		out_T render_output[TetrisField::HEIGHT][TetrisField::WIDTH],
		const std::unordered_map<bool, out_T> translator)
	{
		for (unsigned char i = 0; i < TetrisField::HEIGHT; i++)
		{
			for (unsigned char j = 0; j < TetrisField::WIDTH; j++)
			{
				unsigned int segment = TetrisField::xy2i(i, j) / SEGMENTED_FIELD_LENGTH;
				unsigned int bit_pos = TetrisField::xy2i(i, j) - (segment * SEGMENTED_FIELD_LENGTH);
				// Translate the bit to the correct render_output element
				render_output[i][j] = translator.at(
					// Get bit at bit_pos of the segment of the field.
					static_cast<bool>(field[segment]>>bit_pos & 1));
			}
		}
	}
	void reduce_field(const TetrisField& field,
		ReducedField& output);

	//copies to output
	void reduce_segment_field(const TetrisField& field,
		SegmentedReducedField& output);
	
}