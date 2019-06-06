#include "renderer.h"

void Renderer::RenderUnit::update_string(const LiteGameModule::LiteModule& mod)
{
	field = get_field_str(mod);
	//Should now be done with reassigning renderer
	info = get_score_info(mod);
	upcoming = get_upcoming_str(mod);
}

void Renderer::RenderUnit::show_shadow(const LiteGameModule::LiteModule& mod)
{
	double dist = LiteGameModule::LiteModule::down_cast(mod.controlling_piece, mod.field);
	LGEngine::Position2D shadow_pos = { mod.controlling_piece.current_position.x, mod.controlling_piece.current_position.y + dist };
	std::string _field = field;
#define WIDTH 4
	for (char y = 0; y < WIDTH; y++)
	{
		int index_y_base = ((int)(shadow_pos.y) + y) * CHAR_FIELD_WIDTH;
		for (char x = 0; x < WIDTH; x++)
		{
			if (mod.controlling_piece.collider[LGEngine::TetrisBody::xy2i(x, y)])
			{
				int ind = index_y_base + (int)(shadow_pos.x) + x;
				if (field[ind] == ' ')
				{
					field[ind] = SHADOW_CHAR;
				}
			}
		}
	}
	render();
	field = _field;
#undef WIDTH
}

void Renderer::RenderUnit::show_burns(const unsigned char& n_burned, const unsigned char& burn_y, const int& time_ms)
{
	for (char i = 0; i < n_burned; i++)
	{
		int pos_y = burn_y + 1 - n_burned;
		pos_y *= CHAR_FIELD_WIDTH;
		for (int x = LGEngine::TetrisField::FIELD_LEFT; x <= LGEngine::TetrisField::FIELD_RIGHT; x++)
		{
			int index = (pos_y + x);
			field[index] = BURNED_CHAR;
		}
	}
	render();
	std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
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
