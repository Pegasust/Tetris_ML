#include "lite_renderer.h"

std::string Renderer::get_score_info(const LiteGameModule::LiteModule& mod)
{
	//score and highest score
	std::string score_str = "_score: ";
	score_str += std::to_string(mod.score);
	score_str += "\nhscore: ";
	score_str += std::to_string(mod.highest_score);
	score_str += "\n_level: ";
	score_str += std::to_string(mod.current_level);
	score_str += "\n_requirements: ";
	score_str += std::to_string(mod.n_level_up_rows);
	score_str += "\n_current_seed: ";
	score_str += std::to_string(mod.current_seed);
	score_str += "\n_current_x: ";
	score_str += std::to_string(mod.controlling_piece.current_position.x);
	score_str += ", ";
	score_str += std::to_string(mod.controlling_piece.current_position.y);
	return score_str;
}

std::string Renderer::get_upcoming_str(const LiteGameModule::LiteModule& mod)
{
	std::queue<LGEngine::BodyType> upcoming_copy = mod.coming_pieces;
	std::string upcoming_str = "upcoming pieces: ";
	while (!upcoming_copy.empty())
	{
		upcoming_str += TETRIS_CHARS[upcoming_copy.front()];
		upcoming_str += '\t';
		upcoming_copy.pop();
	}
	upcoming_str += "\ncurrent piece: ";
	upcoming_str += TETRIS_CHARS[mod.controlling_piece.type];
	return upcoming_str;
}

std::string Renderer::get_renderer_string(const LiteGameModule::LiteModule& mod)
{
	std::string tetris_game_field(get_field_str(mod));

	//The field should now complete
	
	std::string score_str(get_score_info(mod));

	//upcoming pieces:
	std::string upcoming_str(get_upcoming_str(mod));

	tetris_game_field += score_str;
	tetris_game_field += '\n';
	tetris_game_field += upcoming_str;
	return tetris_game_field;
}

std::string Renderer::get_field_str(const LiteGameModule::LiteModule& mod)
{
	std::string tetris_game_field;
	//Draw the static objects
	for (unsigned char y = LGEngine::TetrisField::FIELD_TOP; y <= LGEngine::TetrisField::FIELD_BOTTOM + 1; y++)
	{
		unsigned int y_index0 = y * LGEngine::TetrisField::WIDTH;
		//Assign from leftmost border to rightmost border
		unsigned char x;
		for (x = LGEngine::TetrisField::FIELD_LEFT - 1; x <= LGEngine::TetrisField::FIELD_RIGHT + 1; x++)
		{
			unsigned int index = y_index0 + x;
			//tetris_game_field[index + y] = TETRIS_CHARS[mod.field.collider[index]];
			tetris_game_field += TETRIS_CHARS[mod.field.collider[index]];
		}
		//tetris_game_field[y_index0 + x + 1 + y] = '\n'; //Add it at the end
		tetris_game_field += '\n'; //Add it at the end
	}
	//tetris_game_field[CHAR_FIELD_LEN - 1] = '\0';
	tetris_game_field += '\0';
	//Finished with the field collider
	int current_piece_y0 = (int)mod.controlling_piece.current_position.y;
	int current_piece_x0 = (int)mod.controlling_piece.current_position.x;

	//Draw the current piece
	for (unsigned char i = 0; i < LGEngine::T_COLLIDER_LEN; i++)
	{
		if (mod.controlling_piece.collider[i]) //If there is collider
		{
			unsigned char localx, localy;
			mod.controlling_piece.i2xy(i, localx, localy);
			int renderer_y = current_piece_y0 + localy;
			int renderer_x = current_piece_x0 + localx;
			unsigned int index = renderer_y * (LGEngine::TetrisField::WIDTH + 1) + renderer_x;

			//Overdraw it at the correct position
			tetris_game_field[index] = TETRIS_CHARS[mod.controlling_piece.type];
			//tetris_game_field += TETRIS_CHARS[mod.controlling_piece.type];
		}
	}
	return tetris_game_field;
}

void Renderer::clear_console()
{
#ifdef USE_SYSTEM_CLEAR
	if (system("CLS")) system("clear");
#elif defined USE_CONIO
	clrscr();
	Don't use this unless tested.
#else
	Something went wrong...
#endif
}

void Renderer::RenderUnit::update_string(const LiteGameModule::LiteModule& mod)
{
	//Reassign 
	//for (unsigned char y = LGEngine::TetrisField::FIELD_TOP; y <= LGEngine::TetrisField::FIELD_BOTTOM; y++)
	//{
	//	unsigned int str_y_index0 = y * (LGEngine::TetrisField::WIDTH + 1);
	//	for (unsigned char x = LGEngine::TetrisField::FIELD_LEFT; x <= LGEngine::TetrisField::FIELD_RIGHT; x++)
	//	{
	//		field[str_y_index0 + x] = mod.field.collider[mod.field.xy2i(x, y)];
	//	}
	//}
	field = get_field_str(mod);
	//Should now be done with reassigning renderer
	info = get_score_info(mod);
	upcoming = get_upcoming_str(mod);
}

void Renderer::RenderUnit::render()
{
	std::string render(field);
	render += info;
	render += '\n';
	render += upcoming;
	std::cout << render << std::endl;
	std::cout << "frames: " << std::to_string(++frames) << std::endl;
}

Renderer::RenderUnit::RenderUnit(const LiteGameModule::LiteModule& mod) :
	field(get_field_str(mod)), info(get_score_info(mod)), upcoming(get_upcoming_str(mod))
{

}
