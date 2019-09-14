#include "game_renderer.h"
#include "game_renderer.h"

#ifdef USE_TXT_CONSOLE_RENDERER
void Renderer::StdTxtRenderer::clear_screen()
{
#ifdef __cplusplus
	std::cout << std::string(100, '\n')<<std::endl;
#else
	int n;
	for (n = 0; n < 10; n++)
	{
		printf("\n\n\n\n\n\n\n\n\n\n");
	}
#endif
}

bool Renderer::StdTxtRenderer::try_update(Tetris::GameModule const& mod, RenderData& new_data)
{
	//VERY SIMPLISTIC
	clear_screen();
	//RenderStrings tetris_field = //tetris_field_string(mod.game_field, mod.controlling_piece);
	//	tetris_field_string(mod);
	//RenderStrings scoreboard = tetris_scoreboard(mod);
	//RenderStrings game_info = tetris_game_info(mod);
	//RenderStrings coming_pieces = tetris_upcoming_pieces(mod.coming_pieces);

	std::string new_str;
#define ITERATE(str_vec, empty_str) for(std::vector<std::string>::const_iterator  it = str_vec.begin(); it != str_vec.end(); ++it)\
{																																\
	empty_str += *it;																												\
	empty_str += '\n';																											\
}
	//ITERATE(tetris_field, new_str);
	//new_str += '\n';
	//ITERATE(scoreboard, new_str);
	//new_str += '\n';
	//ITERATE(game_info, new_str);
	//new_str += '\n';
	//ITERATE(coming_pieces, new_str);

	//new_data = new_str;
	assign_render_string(mod, true, new_str);
	new_data = new_str;
	//clear_screen();
	//RenderStrings tetris_field = tetris_field_string(mod.game_field, mod.controlling_piece);
	//RenderStrings scoreboard = tetris_scoreboard(mod);
	//RenderStrings game_info = tetris_game_info(mod);
	//RenderStrings upcoming_pieces = tetris_upcoming_pieces(mod.coming_pieces);

	//Not yet implemented the priority yet

	return true;
}

bool Renderer::StdTxtRenderer::try_display(const RenderData& data)
{
	std::cout << data << std::endl;
	return true;
}

char Renderer::StdTxtRenderer::body_type_2_char(Tetris::BodyType const& body)
{
	static const std::unordered_map<Tetris::BodyType, char> char_val
	{
		{Tetris::BLANK, ' '},
		{Tetris::I, 'I'},
		{Tetris::Z, 'Z'},
		{Tetris::S, 'S'},
		{Tetris::O, 'O'},
		{Tetris::T, 'T'},
		{Tetris::L, 'L'},
		{Tetris::J, 'J'},
		{Tetris::BORDER, '#'}
	};
	//std::cout << "Before char_val.at" << std::endl;
//#ifdef DEBUG_DEFINED
//	if (char_val.find(body) == char_val.end())
//	{
//		return '?';
//	}
//#endif
	return char_val.at(body);
}

void Renderer::StdTxtRenderer::assign_render_string(Tetris::GameModule const& mod, const bool& shadow, std::string& str)
{
	static const get_func_map func_map
	{
		{RendererExt::GAME_FIELD::PRIORITY, tetris_field_string},
		{RendererExt::GAME_INFO::PRIORITY, tetris_game_info},
		{RendererExt::SCORE_BOARD::PRIORITY, tetris_scoreboard},
		{RendererExt::UPCOMING_PIECES::PRIORITY, tetris_upcoming_pieces}
	};
	static RendererExt::Priority priority_queue[] = { RendererExt::GAME_FIELD::PRIORITY,
	RendererExt::GAME_INFO::PRIORITY, RendererExt::SCORE_BOARD::PRIORITY, RendererExt::UPCOMING_PIECES::PRIORITY };
	static bool sorted = false;
	if (!sorted)
	{
		//bubblesort is nlogn or 3+2+1 = 6 cycles
		//mergesort is 4 cycles
		//UNSAFE_SWAP works because there is nothing much on priority
#define UNSAFE_SWAP(x, y) x = x + y; y = x - y; x = x - y;
#define UNSAFE_CONDITIONAL_SWAP(smaller, larger) if(smaller > larger) UNSAFE_SWAP(smaller, larger);
	//inline mergesort
		UNSAFE_CONDITIONAL_SWAP(priority_queue[0], priority_queue[1]);
		UNSAFE_CONDITIONAL_SWAP(priority_queue[2], priority_queue[3]);
		UNSAFE_CONDITIONAL_SWAP(priority_queue[0], priority_queue[2]);
		UNSAFE_CONDITIONAL_SWAP(priority_queue[1], priority_queue[3]);
		UNSAFE_CONDITIONAL_SWAP(priority_queue[1], priority_queue[2]);
#undef UNSAFE_CONDITIONAL_SWAP
#undef UNSAFE_SWAP
		sorted = true;
	}
	//TODO: Positioning hasn't been implemented.
	static const int QUEUE_LENGTH = 4;
	for (int i = 0; i < QUEUE_LENGTH; i++)
	{
		auto x = func_map.at(priority_queue[i])(mod);
		ITERATE(x, str);
	}
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_field_string(Tetris::GameModule const& mod)
{
	const int n_lines = Tetris::TetrisField::HEIGHT;
	const int str_length = Tetris::TetrisField::WIDTH;
	int controlling_x0 = RoundingExt::position_round<int>(mod.controlling_piece.current_position.x);
	int controlling_y0 = RoundingExt::position_round<int>(mod.controlling_piece.current_position.y);
	auto shadow_y0_d = mod.down_cast(mod.controlling_piece, mod.game_field) + mod.controlling_piece.current_position.y;
	int shadow_y0 = RoundingExt::position_round<int>(shadow_y0_d);
	RenderStrings rs;
	rs.reserve(n_lines);
	for (int i = 0; i <
#ifdef RELEASE_DEFINED
		n_lines - 1
#else //DEBUG_DEFINED //The last one will be all '#' if it's worked correctly, micro optimization
		n_lines
#endif
		; i++)
	{
		std::string str;
		for (int j = 0; j < str_length; j++)
		{
			//str += body_type_2_char(game_field.collider[Tetris::TetrisField::xy2i(i, j)]);
			unsigned char index = Tetris::TetrisField::xy2i(j, i);
			Tetris::BodyType this_type = mod.game_field.collider[index];
			//Shadow casting first
			bool ctr_piece_in_x_range = j >= controlling_x0 && j < controlling_x0 + Tetris::T_COLLIDER_WID;

			//check for controlling piece
			if (this_type == Tetris::BodyType::BLANK &&
				i >= controlling_y0 && i < controlling_y0 + Tetris::T_COLLIDER_HEIGHT && //within y range
				ctr_piece_in_x_range 
				)
			{
				if (mod.controlling_piece.collider[Tetris::TetrisBody::xy2i(j - controlling_x0, i - controlling_y0)])
				{
					this_type = mod.controlling_piece.type;
				}
			}
			else if (i >= shadow_y0 && i < shadow_y0 + Tetris::T_COLLIDER_HEIGHT && ctr_piece_in_x_range)
			{
				if (mod.controlling_piece.collider[Tetris::TetrisBody::xy2i(j - controlling_x0, i - shadow_y0)])
				{
					str += RendererExt::SHADOW_CHAR;
					continue;
				}
			}
			str += body_type_2_char(this_type);
		}
		rs.push_back(str);
	}
#ifdef RELEASE_DEFINED
	rs.push_back(std::string(str_length, body_type_2_char(Tetris::BodyType::BORDER)));
#endif
	return rs;

}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_field_string
	(Tetris::TetrisField const& game_field,
	Tetris::TetrisBody const& controlling_piece)
{
	const int n_lines = Tetris::TetrisField::HEIGHT;
	const int str_length = Tetris::TetrisField::WIDTH;
	int controlling_x0 = RoundingExt::position_round<int>(controlling_piece.current_position.x);
	int controlling_y0 = RoundingExt::position_round<int>(controlling_piece.current_position.y);
	RenderStrings rs;
	rs.reserve(n_lines);
	for (int i = 0; i < 
#ifdef RELEASE_DEFINED
		n_lines-1
#else //DEBUG_DEFINED //The last one will be all '#' if it's worked correctly, micro optimization
		n_lines
#endif
		; i++) 
	{
		std::string str;
		for (int j = 0; j < str_length; j++)
		{
			//str += body_type_2_char(game_field.collider[Tetris::TetrisField::xy2i(i, j)]);
			unsigned char index = Tetris::TetrisField::xy2i(j, i);
			Tetris::BodyType this_type = game_field.collider[index];
			//check for controlling piece
			if (this_type == Tetris::BodyType::BLANK &&
				i >= controlling_y0 && i < controlling_y0 + Tetris::T_COLLIDER_HEIGHT&& //within y range
				j >= controlling_x0 && j < controlling_x0 + Tetris::T_COLLIDER_WID//within x range
				)
			{
				if (controlling_piece.collider[Tetris::TetrisBody::xy2i(j - controlling_x0, i - controlling_y0)])
				{
					this_type = controlling_piece.type;
				}
			}
			str += body_type_2_char(this_type);
		}
		rs.push_back(str);
	}
#ifdef RELEASE_DEFINED
	rs.push_back(std::string(str_length, body_type_2_char(Tetris::BodyType::BORDER)));
#endif
	return rs;
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_upcoming_pieces(std::queue<Tetris::BodyType> coming_pieces)
{
	RenderStrings rs;
	if (RendererExt::UPCOMING_PIECES::MODE == RendererExt::RenderMode::VERTICAL)
	{
		rs.reserve(RendererExt::UPCOMING_PIECES::MOD_Y); //the number of lines
		rs.push_back("UPCOMING");
		rs.push_back(""); //For new line.
		for (int i = 0; i < Tetris::GameModule::N_PIECE_AHEAD; i++)
		{
			char c = body_type_2_char(coming_pieces.front());
			int cols_ind = Tetris::TetrisBody::
				get_min_index(coming_pieces.front());
			coming_pieces.pop();
			for (int j = 0; j < Tetris::T_COLLIDER_HEIGHT; j++)
			{
				std::string str(2, ' '); //offset: 2
				for (int k = 0; k < Tetris::T_COLLIDER_WID; k++)
				{
					if ((Tetris::TetrisBody::colliders[cols_ind])[Tetris::TetrisBody::xy2i(k, j)])
					{
						str += c;
					}
					else
					{
						str += ' ';
					}
				}
				rs.push_back(str);
			}
			if (i != Tetris::GameModule::N_PIECE_AHEAD - 1)
			{
				rs.push_back(""); //one line that separates the pieces
			}
			else
			{
				rs.push_back("########");
			}
		}
	}
	return rs;
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_upcoming_pieces(Tetris::GameModule const& mod)
{
	return tetris_upcoming_pieces(mod.coming_pieces);
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_scoreboard(Tetris::GameModule const& mod)
{
	RenderStrings rs;
	if (RendererExt::SCORE_BOARD::MODE == RendererExt::RenderMode::VERTICAL)
	{
		rs.reserve(12);
		rs.push_back("Level");
		rs.push_back(std::to_string(mod.current_level));
		rs.push_back("Score");
		rs.push_back(std::to_string(mod.score));
		rs.push_back("Rows Remaining");
		rs.push_back(std::to_string(mod.n_rows));
		rs.push_back("Highest");
		rs.push_back(std::to_string(mod.highest_score));
		rs.push_back("Tetris Count");
		rs.push_back(std::to_string(mod.tetris_scored));
		rs.push_back("Rows Cleared");
		rs.push_back(std::to_string(mod.n_rows_burned));
	}
	return rs;
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_game_info(Tetris::GameModule const& mod)
{
	RenderStrings rs;
	if (RendererExt::GAME_INFO::MODE == RendererExt::RenderMode::VERTICAL)
	{
#ifdef DEBUG_DEFINED
		rs.push_back("Seed");
		//Seed display
		std::string seed = std::to_string(mod.current_seed.seed);
		seed += " (";
		seed += std::to_string(Common::ZMath::UInt64RNG::to_double(mod.current_seed.seed));
		seed += ")";
		rs.push_back(seed);
#endif
		rs.push_back("Fall Velocity");
		rs.push_back(std::to_string(Tetris::v_fall_at(mod.current_level)));
		rs.push_back("Controlling Prop.");

		//Controlling piece properties
		std::string props = "{";
		props += std::to_string(mod.controlling_piece.current_position.x);
		props += ", ";
		props += std::to_string(mod.controlling_piece.current_position.y);
		props += ", ";
		props += std::to_string(mod.controlling_piece.current_rot);
		props += "}";

		rs.push_back(props);
	}
	return rs;
}
#undef ITERATE
#endif