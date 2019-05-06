//#include "w_renderer.h"
//using namespace Renderer;
//void Renderer::draw_to_console(GameCore::GameInfo& info)
//{
//	TetrisPiece::PieceType *abstract_screen = (TetrisPiece::PieceType*) calloc(sizeof(TetrisPiece::PieceType),
//		info.rule->max_x * info.rule->max_y);
//	std::unordered_set<GameFeatures::TetrisPiece::Indexor> occupied_indexes;
//	for (unsigned char i = 0; i < info.built_pieces.size(); i++)
//	{
//		GameFeatures::TetrisPiece*& checking_piece = info.built_pieces[i];
//		//top left position
//		R_Uint x, y;
//		GameFeatures::TetrisPiece::posId2xy(checking_piece->positionId, x, y, info.rule->max_x, info.rule->max_y);
//		//Local loop
//		for (TetrisPiece::Indexor _y = 0; _y < TetrisPiece::TETRIMINO_HEIGHT; _y++)
//		{
//			for (TetrisPiece::Indexor _x = 0; _x < TetrisPiece::TETRIMINO_WIDTH; _x++)
//			{
//				TetrisPiece::Indexor local_index = TetrisPiece::xy2i(_x, _y);
//				if (checking_piece->data[local_index]) //if it occupies the space
//				{
//					//Assign it to the screen
//					abstract_screen[xy2i(x + _x, y + _y)] = checking_piece->type;
//					occupied_indexes.insert(local_index);
//				}
//			}
//		}
//	}
//	for (TetrisPiece::Indexor i = 0; i < info.rule->max_x * info.rule->max_y; i++)
//	{
//		if (occupied_indexes.count(i) == 0) //if hashset does not have the value
//		{
//			abstract_screen[i] = TetrisPiece::BLANK;
//		}
//	}
//	//wchar_t screen_text[WIDTH*HEIGHT];
//	//memset(screen_text, L' ', 2 * WIDTH * HEIGHT);
//	//Initialize blank space instead of NULL value
//	//wchar_t* screen_text = (wchar_t*) calloc(L' ', sizeof(wchar_t) * WIDTH * HEIGHT);
//	wchar_t* screen_text = new wchar_t[WIDTH * HEIGHT];
//	//for (int i = 0; i < WIDTH * HEIGHT; i++) screen_text[i] = L' ';
//	wmemset(screen_text, L' ', WIDTH * HEIGHT);
//	for (R_Int x = -1; x < WIDTH+1; x++)
//	{
//		for (R_Uint y = 0; y < HEIGHT+1; y++)
//		{
//			R_Uint render_x = x + X_OFFSET;
//			R_Uint render_y = y + Y_OFFSET;
//			if (x == -1 || x == WIDTH + 1 || y == HEIGHT+1) //boundaries
//			{
//				screen_text[xy2i(render_x, render_y)] = TETRIMINO_DISPLAY_CHAR[BOUNDARY_INDEX];
//			}
//			else
//			{
//				screen_text[xy2i(render_x, render_y)] = TETRIMINO_DISPLAY_CHAR[abstract_screen[xy2i(x, y)]];
//			}
//		}
//	}
//	//free(abstract_screen);
//
//#ifdef _WINDOWS_
//	HANDLE h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	SetConsoleActiveScreenBuffer(h_console);
//	DWORD bytes_written = 0;
//	//Display
//	WriteConsoleOutputCharacter(h_console, screen_text, WIDTH * HEIGHT, { 0,0 }, &bytes_written);
//#endif
//	//delete[] screen_text;
//}
//
//Renderer::RenderUnit::RenderUnit(GameCore::GameRule& rule) : 
//	abstract_screen((TetrisPiece::PieceType*)calloc(
//	rule.max_x * rule.max_y, sizeof(TetrisPiece::PieceType))), //abstract screen is all PieceType.BLANK
//	screen_text(new wchar_t[WIDTH * HEIGHT])
//{
//	wmemset(screen_text, L' ', WIDTH * HEIGHT);
//}
