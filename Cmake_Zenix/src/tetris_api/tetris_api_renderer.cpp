#include "tetris_api_renderer.h"
#include "../common/assertion.hpp"
void TetrisAPI::reduce_field(const TetrisField& field,
	ReducedField& output)
{
	for (unsigned char i = 0; i < TetrisField::LENGTH; i++)
	{
		output[i] = (field.collider[i] != 0);
	}
}

void TetrisAPI::reduce_segment_field(const TetrisField& field,
	SegmentedReducedField& output)
{
	for (int seg = 0; seg < SEGMENTED_FIELD_LENGTH; seg++)
	{
		int seg_pos0 = seg * sizeof(unsigned long)*8;
		Common::ASSERT(output[seg] == 0, "output[seg] is not 0");
		for (int bit_pos = 0; bit_pos < sizeof(unsigned long)*8; bit_pos++)
		{
			output[seg] |= (static_cast<unsigned long>(
				field.collider[seg_pos0 + bit_pos])
				<< bit_pos);
		}
	}
}
