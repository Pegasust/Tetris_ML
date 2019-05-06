#include "renderer.h"

CrossRenderer::RenderUnit::RenderUnit(GameCore::GameRule& rule)
{
	memset(screen_text, ' ', SIZE);
	//The code below should set all to 0u, which is TetrisPiece::PieceType::BLANK
	abstract_screen = (TetrisPiece::PieceType*) calloc(rule.max_x * rule.max_y,
		sizeof(TetrisPiece::PieceType));

}

void CrossRenderer::RenderUnit::update_screen_text(GameCore::GameInfo& info)
{

}
