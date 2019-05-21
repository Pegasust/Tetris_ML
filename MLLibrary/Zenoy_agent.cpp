#include "Zenoy_agent.h"

ZenoyAgent::UObservation ZenoyAgent::get_unscaled_observation(const LGEngine::TetrisField& field)
{
	UObservation return_value;
	for (unsigned char i = 0; i < Zenoy::CoefficientVector::LENGTH; i++)
	{
		return_value.values[i].double_expr = 0.0;
	}
	unsigned char last_top_y = field.FIELD_BOTTOM;
	for (unsigned char x = field.FIELD_LEFT; x <= field.FIELD_RIGHT; x++)
	{
		unsigned char current_top_y = field.FIELD_BOTTOM;
		bool previously_occupied = false;
		for (unsigned char y = field.FIELD_TOP; y <= field.FIELD_BOTTOM; y++)
		{
			if (field.collider[field.xy2i(x, y)] != TEngine::BLANK) //When that collider is not blank
			{
				if (current_top_y > y) //If the current y is less than current top record					
				{
					current_top_y = y;
				}
				previously_occupied = true;
			}
			else
			{
				//hole check
				if (previously_occupied) //If the upper tile is occupied
				{
					return_value.values[Zenoy::CoefficientVector::HOLES].double_expr+=1.0; //HOLES COMPLETED
					previously_occupied = false;
				}
			}
		}
		return_value.values[Zenoy::CoefficientVector::AGGREGATE_HEIGHT].double_expr += static_cast<double>(field.FIELD_BOTTOM - current_top_y); //AGGREGATE HEIGHT COMPLETED
		if (x != field.FIELD_LEFT) //Ignore the first column because there is no data from its left
		{
			return_value.values[Zenoy::CoefficientVector::BULKINESS].double_expr += static_cast<double>(abs(current_top_y - last_top_y)); //BULKINESS COMPLETED
		}
		last_top_y = current_top_y;
	}
	return return_value;
}
