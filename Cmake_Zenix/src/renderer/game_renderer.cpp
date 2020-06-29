#include "game_renderer.h"

#if defined(USE_TXT_CONSOLE_RENDERER) || defined(USE_REVISED_TXT_CONSOLE_RENDERER)
char Renderer::StdTxtRenderer::body_type_2_char(Tetris::BodyType const& body) {
    static const std::unordered_map<Tetris::BodyType, char> char_val{
        {Tetris::BLANK, ' '}, {Tetris::I, 'I'}, {Tetris::Z, 'Z'},
        {Tetris::S, 'S'},     {Tetris::O, 'O'}, {Tetris::T, 'T'},
        {Tetris::L, 'L'},     {Tetris::J, 'J'}, {Tetris::BORDER, '#'}};
    // std::cout << "Before char_val.at" << std::endl;
    //#ifdef DEBUG_DEFINED
    // Safer method to implement lookup.
    //	if (char_val.find(body) == char_val.end())
    //	{
    //		return '?';
    //	}
    //#endif
    return char_val.at(body);
}
#endif

#ifdef USE_TXT_CONSOLE_RENDERER
#ifdef USE_TXT_CONSOLE_RENDERER
void Renderer::StdTxtRenderer::clear_screen() {
#ifdef __cplusplus
    std::cout << std::string(100, '\n') << std::endl;
#else
    int n;
    for (n = 0; n < 10; n++) {
        printf("\n\n\n\n\n\n\n\n\n\n");
    }
#endif
}
#endif

#if defined(USE_TXT_CONSOLE_RENDERER)
bool Renderer::StdTxtRenderer::try_initialize(Tetris::GameModule const& mod, RenderData& new_data) {
    // VERY SIMPLISTIC
    // clear_screen();
    // RenderStrings tetris_field = //tetris_field_string(mod.game_field, mod.controlling_piece);
    //	tetris_field_string(mod);
    // RenderStrings scoreboard = tetris_scoreboard(mod);
    // RenderStrings game_info = tetris_game_info(mod);
    // RenderStrings coming_pieces = tetris_upcoming_pieces(mod.coming_pieces);

#define ITERATE(str_vec, empty_str)                                                                \
    for (std::vector<std::string>::const_iterator it = str_vec.begin(); it != str_vec.end();       \
         ++it) {                                                                                   \
        empty_str += *it;                                                                          \
        empty_str += '\n';                                                                         \
    }

    // ITERATE(tetris_field, new_str);
    // new_str += '\n';
    // ITERATE(scoreboard, new_str);
    // new_str += '\n';
    // ITERATE(game_info, new_str);
    // new_str += '\n';
    // ITERATE(coming_pieces, new_str);

    std::string new_str;
    // new_data = new_str;
    assign_empty_render_string(mod, true, new_str);
    new_data = new_str;
    // clear_screen();
    // RenderStrings tetris_field = tetris_field_string(mod.game_field, mod.controlling_piece);
    // RenderStrings scoreboard = tetris_scoreboard(mod);
    // RenderStrings game_info = tetris_game_info(mod);
    // RenderStrings upcoming_pieces = tetris_upcoming_pieces(mod.coming_pieces);

    // Not yet implemented the priority yet

    return true;
}

#endif // defined(USE_TXT_CONSOLE_RENDERER)

#if defined(USE_TXT_CONSOLE_RENDERER)
bool Renderer::StdTxtRenderer::try_update(Tetris::GameModule const& mod,
                                          RenderData& overriding_data) {
    return try_initialize(mod, overriding_data);
}
#endif // defined(USE_TXT_CONSOLE_RENDERER)

bool Renderer::StdTxtRenderer::try_display(const RenderData& data) {
    std::cout << data << std::endl;
    return true;
}

void Renderer::StdTxtRenderer::assign_empty_render_string(Tetris::GameModule const& mod,
                                                          const bool& shadow, std::string& str) {
    auto r_d = tetris_array_renderstrings(mod);
    // Merge r_d into str
    for (int i = 0; i < QUEUE_LENGTH; i++) {
        ITERATE(r_d[i], str);
    }
}

void Renderer::StdTxtRenderer::override_render_string(Tetris::GameModule const& mod,
                                                      const bool& shadow, std::string& str) {
    // Impossible to implement with higher efficiency than
    // assign_empty_render_string.
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_field_string(
    Tetris::GameModule const& mod) {
    const int n_lines = Tetris::TetrisField::HEIGHT;
    const int str_length = Tetris::TetrisField::WIDTH;
    int controlling_x0 = RoundingExt::position_round<int>(mod.controlling_piece.current_position.x);
    int controlling_y0 = RoundingExt::position_round<int>(mod.controlling_piece.current_position.y);
    auto shadow_y0_d = mod.down_cast(mod.controlling_piece, mod.game_field) +
                       mod.controlling_piece.current_position.y;
    int shadow_y0 = RoundingExt::position_round<int>(shadow_y0_d);
    RenderStrings rs;
    rs.reserve(n_lines);
    for (int i = 0; i <
#ifdef RELEASE_DEFINED
                    n_lines - 1
#else // DEBUG_DEFINED //The last one will be all '#' if it's worked correctly, micro optimization
                    n_lines
#endif
         ;
         i++) {
        std::string str;
        for (int j = 0; j < str_length; j++) {
            // str += body_type_2_char(game_field.collider[Tetris::TetrisField::xy2i(i, j)]);
            unsigned char index = Tetris::TetrisField::xy2i(j, i);
            Tetris::BodyType this_type = mod.game_field.collider[index];
            // Shadow casting first
            bool ctr_piece_in_x_range =
                j >= controlling_x0 && j < controlling_x0 + Tetris::T_COLLIDER_WID;

            // check for controlling piece
            if (this_type == Tetris::BodyType::BLANK && i >= controlling_y0 &&
                i < controlling_y0 + Tetris::T_COLLIDER_HEIGHT && // within y range
                ctr_piece_in_x_range) {
                if (mod.controlling_piece.collider[Tetris::TetrisBody::xy2i(j - controlling_x0,
                                                                            i - controlling_y0)]) {
                    this_type = mod.controlling_piece.type;
                }
            } else if (i >= shadow_y0 && i < shadow_y0 + Tetris::T_COLLIDER_HEIGHT &&
                       ctr_piece_in_x_range) {
                if (mod.controlling_piece
                        .collider[Tetris::TetrisBody::xy2i(j - controlling_x0, i - shadow_y0)]) {
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

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_field_string(
    Tetris::TetrisField const& game_field, Tetris::TetrisBody const& controlling_piece) {
    const int n_lines = Tetris::TetrisField::HEIGHT;
    const int str_length = Tetris::TetrisField::WIDTH;
    int controlling_x0 = RoundingExt::position_round<int>(controlling_piece.current_position.x);
    int controlling_y0 = RoundingExt::position_round<int>(controlling_piece.current_position.y);
    RenderStrings rs;
    rs.reserve(n_lines);
    for (int i = 0; i <
#ifdef RELEASE_DEFINED
                    n_lines - 1
#else // DEBUG_DEFINED //The last one will be all '#' if it's worked correctly, micro optimization
                    n_lines
#endif
         ;
         i++) {
        std::string str;
        for (int j = 0; j < str_length; j++) {
            // str += body_type_2_char(game_field.collider[Tetris::TetrisField::xy2i(i, j)]);
            unsigned char index = Tetris::TetrisField::xy2i(j, i);
            Tetris::BodyType this_type = game_field.collider[index];
            // check for controlling piece
            if (this_type == Tetris::BodyType::BLANK && i >= controlling_y0 &&
                i < controlling_y0 + Tetris::T_COLLIDER_HEIGHT &&                  // within y range
                j >= controlling_x0 && j < controlling_x0 + Tetris::T_COLLIDER_WID // within x range
            ) {
                if (controlling_piece.collider[Tetris::TetrisBody::xy2i(j - controlling_x0,
                                                                        i - controlling_y0)]) {
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

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_upcoming_pieces(
    Tetris::GameModule::ComingPieces coming_pieces) {
    RenderStrings rs;
    if (RendererExt::UPCOMING_PIECES::MODE == RendererExt::RenderMode::VERTICAL) {
        rs.reserve(RendererExt::UPCOMING_PIECES::MOD_Y); // the number of lines
        rs.push_back("UPCOMING");
        rs.push_back(""); // For new line.
        for (int i = 0; i < Tetris::GameModule::N_PIECE_AHEAD; i++) {
            char c = body_type_2_char(coming_pieces.front());
            int cols_ind = Tetris::TetrisBody::get_min_index(coming_pieces.front());
            // coming_pieces.pop_front();
            coming_pieces.pop();
            for (int j = 0; j < Tetris::T_COLLIDER_HEIGHT; j++) {
                std::string str(2, ' '); // offset: 2
                for (int k = 0; k < Tetris::T_COLLIDER_WID; k++) {
                    if ((Tetris::TetrisBody::colliders[cols_ind])[Tetris::TetrisBody::xy2i(k, j)]) {
                        str += c;
                    } else {
                        str += ' ';
                    }
                }
                rs.push_back(str);
            }
            if (i != Tetris::GameModule::N_PIECE_AHEAD - 1) {
                rs.push_back(""); // one line that separates the pieces
            } else {
                rs.push_back("########");
            }
        }
    }
    return rs;
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_upcoming_pieces(
    Tetris::GameModule const& mod) {
    return tetris_upcoming_pieces(mod.coming_pieces);
}

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_scoreboard(
    Tetris::GameModule const& mod) {
    RenderStrings rs;
    if (RendererExt::SCORE_BOARD::MODE == RendererExt::RenderMode::VERTICAL) {
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

Renderer::StdTxtRenderer::RenderStrings Renderer::StdTxtRenderer::tetris_game_info(
    Tetris::GameModule const& mod) {
    RenderStrings rs;
    if (RendererExt::GAME_INFO::MODE == RendererExt::RenderMode::VERTICAL) {
#ifdef DEBUG_DEFINED
        rs.push_back("Seed");
        // Seed display
        std::string seed = std::to_string(mod.current_seed.seed);
        seed += " (";
        seed += std::to_string(Common::ZMath::UInt64RNG::to_double(mod.current_seed.seed));
        seed += ")";
        rs.push_back(seed);
#endif
        rs.push_back("Fall Velocity");
        rs.push_back(std::to_string(Tetris::v_fall_at(mod.current_level)));
        rs.push_back("Controlling Prop.");

        // Controlling piece properties
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
Renderer::StdTxtRenderer::RenderStrsArray& Renderer::StdTxtRenderer::tetris_array_renderstrings(
    Tetris::GameModule const& mod) {
    // TODO: Positioning hasn't been implemented.
    // Use threads to generate each string
    std::thread th[QUEUE_LENGTH];
    RenderStrsArray r_d;
    std::fill(r_d.begin(), r_d.end(), RenderStrings());
    // Fill r_d parallelly
    for (int i = 0; i < QUEUE_LENGTH; i++) {
        auto func = func_from_render_order(i);
        th[i] = std::thread([&r_d, func, &mod, i]() { r_d[i] = func(mod); });
    }
    // Wait for everything to be done
    int remain = QUEUE_LENGTH;
    while (remain > 0) {
        for (int i = 0; i < QUEUE_LENGTH; i++) {
            if (th[i].joinable()) {
                th[i].join();
                remain--;
            }
        }
    }
    return r_d;
}
Renderer::StdTxtRenderer::StringGetFunction Renderer::StdTxtRenderer::func_from_render_order(
    const int& render_order) {
    // Initializes a function mapping from
    // Priority to function
    static const get_func_map func_map{
        {RendererExt::GAME_FIELD::PRIORITY, tetris_field_string},
        {RendererExt::GAME_INFO::PRIORITY, tetris_game_info},
        {RendererExt::SCORE_BOARD::PRIORITY, tetris_scoreboard},
        {RendererExt::UPCOMING_PIECES::PRIORITY, tetris_upcoming_pieces}};
    static RendererExt::Priority priority_queue[] = {
        RendererExt::GAME_FIELD::PRIORITY, RendererExt::GAME_INFO::PRIORITY,
        RendererExt::SCORE_BOARD::PRIORITY, RendererExt::UPCOMING_PIECES::PRIORITY};
    // Sort the priority queue only once (static)
    static bool sorted = false;
    if (!sorted) {
        // bubblesort is nlogn or 3+2+1 = 6 cycles
        // mergesort is 4 cycles
        // UNSAFE_SWAP works because there is nothing much on priority
#define UNSAFE_SWAP(x, y)                                                                          \
    x = x + y;                                                                                     \
    y = x - y;                                                                                     \
    x = x - y;
#define UNSAFE_CONDITIONAL_SWAP(smaller, larger)                                                   \
    if (smaller > larger)                                                                          \
        UNSAFE_SWAP(smaller, larger);
        // inline mergesort
        UNSAFE_CONDITIONAL_SWAP(priority_queue[0], priority_queue[1]);
        UNSAFE_CONDITIONAL_SWAP(priority_queue[2], priority_queue[3]);
        UNSAFE_CONDITIONAL_SWAP(priority_queue[0], priority_queue[2]);
        UNSAFE_CONDITIONAL_SWAP(priority_queue[1], priority_queue[3]);
        UNSAFE_CONDITIONAL_SWAP(priority_queue[1], priority_queue[2]);
#undef UNSAFE_CONDITIONAL_SWAP
#undef UNSAFE_SWAP
        sorted = true;
    }
    // Done ensuring static initialization
    return func_map.at(priority_queue[render_order]);
}
#undef ITERATE
#endif

#ifdef USE_REVISED_TXT_CONSOLE_RENDERER
void Renderer::StdTxtRenderer::clear_screen() {
#ifdef __cplusplus
    // Do not flush. This improves visual update performance
    // by not flushing.
    std::cout << std::string(100, '\n');
#else
    int n;
    for (n = 0; n < 10; n++) {
        printf("\n\n\n\n\n\n\n\n\n\n");
    }
#endif
}

#define LAZY_INITIALIZE static bool _function_initialized = false;
#define LAZY_DO_ONCE(_1)                                                                           \
    if (!_function_initialized) {                                                                  \
        _1;                                                                                        \
    }
#define LAZY_FUNCTION_END _function_initialized = true;

bool Renderer::StdTxtRenderer::try_initialize(Tetris::GameModule const& mod, RenderData& new_data) {
    // TODO: Make out contain constant fields.
    new_data = RenderData();
    return true;
}

void Renderer::StdTxtRenderer::assign_tetris_field(Tetris::GameModule const& mod, RenderData& out) {
    const int N_LINES = Tetris::TetrisField::HEIGHT;
    const int WIDTH = Tetris::TetrisField::WIDTH;
    // Making shadow
    int controlling_x0 = RoundingExt::position_round<int>(mod.controlling_piece.current_position.x);
    int controlling_y0 = RoundingExt::position_round<int>(mod.controlling_piece.current_position.y);
    auto shadow_y0_d = mod.down_cast(mod.controlling_piece, mod.game_field) +
                       mod.controlling_piece.current_position.y;
    int shadow_y0 = RoundingExt::position_round<int>(shadow_y0_d);

    int render_x, render_y;
    RendererExt::Math<int>::general_get(RendererExt::GAME_FIELD::POSITION, render_x, render_y,
                                        RendererExt::GAME_FIELD::MOD_X,
                                        RendererExt::GAME_FIELD::MOD_Y);
    for (int i = 0; i < N_LINES; i++) {   // y position of TetrisField
        for (int j = 0; j < WIDTH; j++) { // x position of TetrisField
            int render_i = RendererExt::xy2i<int>(render_x + j, render_y + i);
            int field_i = Tetris::TetrisField::xy2i(j, i);

            Tetris::BodyType block_type = mod.game_field.collider[field_i];
            // Shadow casting and controlling piece appearance
            bool ctr_piece_in_x_range =
                j >= controlling_x0 && j < controlling_x0 + Tetris::T_COLLIDER_WID;

            if (ctr_piece_in_x_range) {
                if (block_type == Tetris::BodyType::BLANK &&
                    // i and j is within y of controlling piece
                    i >= controlling_y0 && i < controlling_y0 + Tetris::T_COLLIDER_HEIGHT &&
                    // There is collider at this position of controlling piece
                    mod.controlling_piece.collider[Tetris::TetrisBody::xy2i(j - controlling_x0,
                                                                            i - controlling_y0)]) {

                    // Prioritize the controlling piece before shadow.
                    block_type = mod.controlling_piece.type;
                } else if (i >= shadow_y0 && i < shadow_y0 + Tetris::T_COLLIDER_HEIGHT &&
                           // There is shadow collider at this position of controlling piece
                           mod.controlling_piece.collider[Tetris::TetrisBody::xy2i(
                               j - controlling_x0, i - shadow_y0)]) {

                    // Place a shadow character and move on.
                    out.render_data[render_i] = RendererExt::SHADOW_CHAR;
                    continue;
                }
            }
            out.render_data[render_i] = body_type_2_char(block_type);
        }
    }
}

void Renderer::StdTxtRenderer::assign_upcoming(std::queue<Tetris::BodyType> coming_pieces,
                                               RenderData& out) {

    LAZY_INITIALIZE
    if (RendererExt::UPCOMING_PIECES::MODE == RendererExt::RenderMode::VERTICAL) {
        int render_x, render_y;
        RendererExt::Math<int>::general_get(RendererExt::UPCOMING_PIECES::POSITION, render_x,
                                            render_y, RendererExt::UPCOMING_PIECES::MOD_X,
                                            RendererExt::UPCOMING_PIECES::MOD_Y);
        // TODO: Constant field. Can be optimized by doing once in try_initialize.
        // Make first line to display UPCOMING
        int render_i = RendererExt::xy2i<int>(render_x, render_y);
        // Get the string length of "UPCOMING" without null terminator.
        static constexpr int RENDER_WIDTH = (sizeof("UPCOMING") / sizeof(char)) - 1;

        LAZY_DO_ONCE(out.render_data.replace(render_i, RENDER_WIDTH, "UPCOMING");)
        //// Make next line to be all ' ' filled
        // render_i = RendererExt::xy2i<int>(render_x, render_y + 1);
        // out.render_data.replace(render_i, RENDER_WIDTH, RENDER_WIDTH, ' ');
        // END Constant field.

        // Assign n_piece_ahead
        for (int i = 0; i < Tetris::GameModule::N_PIECE_AHEAD; i++) {
            // space between two pieces
            render_y += 1;

            Tetris::BodyType front;
            if (coming_pieces.empty()) {
                front = Tetris::BodyType::BLANK;
            } else {
                front = coming_pieces.front();
                coming_pieces.pop();
            }
            char c = body_type_2_char(front);
            // Index of default rotation/state of coming_pieces type.
            int cols_default_i = Tetris::TetrisBody::get_min_index(front);
            for (int j = 0; j < Tetris::T_COLLIDER_HEIGHT; j++) { // j : y-pos of coming_piece
                render_i = RendererExt::xy2i<int>(render_x, render_y);
                // Offset by 2 space characters
                constexpr int offset = 2;
                // Ensures that the first two tiles are space
                // out.render_data.replace(render_i, offset, offset, ' ');
                for (int k = 0; k < Tetris::T_COLLIDER_WID; k++) { // k: x-pos of coming_piece
                    render_i = RendererExt::xy2i<int>(render_x + k + offset, render_y);
                    if ((Tetris::TetrisBody::colliders[cols_default_i])[Tetris::TetrisBody::xy2i(
                            k, j)]) {
                        // If there is collider at (j,k) in default state
                        out.render_data[render_i] = c;
                    } else {
                        out.render_data[render_i] = ' ';
                    }
                }
                // Done with this line
                render_y++;
            }
        }

        // Make the last line '#'*RENDER_WIDTH
        // Recorrect render_x position on render_i
        LAZY_DO_ONCE(render_i = RendererExt::xy2i<int>(render_x, render_y);
                     out.render_data.replace(render_i, RENDER_WIDTH, RENDER_WIDTH, '#');)
        // End Constant field.
    }
    LAZY_FUNCTION_END
}

void Renderer::StdTxtRenderer::assign_upcoming(Tetris::GameModule const& mod, RenderData& out) {
    // TODO: It threw a read access violation once on copying
    // mod.coming_pieces. It was referring to 0xFFFF... on BodyType.
    // Exception thrown: read access violation.
    // xmemory::construct
    // list::_Append_range_unchecked at construct shenanigan
    // list::_Construct_range_unchecked at _attach_head
    // list::constructor(&right)
    return assign_upcoming(mod.coming_pieces, out);
}

void Renderer::StdTxtRenderer::assign_scoreboard(Tetris::GameModule const& mod, RenderData& out) {
    int render_x, render_y;
    RendererExt::Math<int>::general_get(RendererExt::SCORE_BOARD::POSITION, render_x, render_y,
                                        RendererExt::SCORE_BOARD::MOD_X,
                                        RendererExt::SCORE_BOARD::MOD_Y);
    // TODO: Might need to go easy on the length of the string.
    LAZY_INITIALIZE
    const int RENDER_WIDTH = 5;
#ifndef ASSIGN
#define ASSIGN(off_x, off_y, str)                                                                  \
    out.render_data.replace(RendererExt::xy2i<int>(render_x + off_x, render_y + off_y),            \
                            str.size(), str);
    const int TITLE_OFFSET = 0;
    const int NUM_OFFSET = 0;
    const int space = 1;
    LAZY_DO_ONCE(ASSIGN(TITLE_OFFSET, 0 * space, std::string("Level"));)
    ASSIGN(NUM_OFFSET, 1 * space, std::to_string(mod.current_level));
    LAZY_DO_ONCE(ASSIGN(TITLE_OFFSET, 2 * space, std::string("Score"));)
    ASSIGN(NUM_OFFSET, 3 * space, std::to_string(mod.score));
    LAZY_DO_ONCE(ASSIGN(TITLE_OFFSET, 4 * space, std::string("Rows Remaining"));)
    ASSIGN(NUM_OFFSET, 5 * space, std::to_string(mod.n_rows));
    LAZY_DO_ONCE(ASSIGN(TITLE_OFFSET, 6 * space, std::string("Highest"));)
    ASSIGN(NUM_OFFSET, 7 * space, std::to_string(mod.highest_score));
    LAZY_DO_ONCE(ASSIGN(TITLE_OFFSET, 8 * space, std::string("Tetris Count"));)
    ASSIGN(NUM_OFFSET, 9 * space, std::to_string(mod.tetris_scored));
    LAZY_DO_ONCE(ASSIGN(TITLE_OFFSET, 10 * space, std::string("Rows Cleared"));)
    ASSIGN(NUM_OFFSET, 11 * space, std::to_string(mod.n_rows_burned));
#undef ASSIGN
#else
#error(ASSIGN Macro is already defined! Find another name.)
#endif
    LAZY_FUNCTION_END
}

void Renderer::StdTxtRenderer::assign_game_info(Tetris::GameModule const& mod, RenderData& out) {
    static char conversion_buffer[Common::RYU_BUFFER_SIZE];
    // static char string_buffer[64];
    // string_buffer[63] = '\0';
    if (RendererExt::GAME_INFO::MODE == RendererExt::RenderMode::VERTICAL) {
        static const std::string s1 = std::string("Seed");
        static const std::string s2 = std::string("Fall Velocity");
        static const std::string s3 = std::string("Controlling Prop.");
        static constexpr size_t MAX_DOUBLE_CHAR_BUFFER =
            (16 + 2 + 17); // FFFFFFFFFFFFFFFF(largest_double_str)
        static const int RENDER_WIDTH = std::max(s3.length(), MAX_DOUBLE_CHAR_BUFFER);
        int y_offset = 0;
        int render_x, render_y;
        RendererExt::Math<int>::general_get(RendererExt::GAME_INFO::POSITION, render_x, render_y,
                                            RendererExt::GAME_INFO::MOD_X,
                                            RendererExt::GAME_INFO::MOD_Y);

#ifndef PUSH_BACK
#define PUSH_BACK(str)                                                                             \
    out.render_data.replace(RendererExt::xy2i<int>(render_x, render_y + (y_offset++)), str.size(), \
                            (str));
#define PUSH_BACK_C(cstr)                                                                          \
    out.render_data.replace(RendererExt::xy2i<int>(render_x, render_y + (y_offset++)),             \
                            strlen(cstr), cstr)
#ifdef DEBUG_DEFINED
        PUSH_BACK(s1);
        std::string result = std::move(Common::decimal2hex_str(mod.current_seed.seed));
        size_t blanks = (sizeof(mod.current_seed.seed) * 8 / 4) - result.size();
        result.append(blanks, ' ');
        result.append("(").append(Common::ryu_d2s_buffered(
            Common::ZMath::UInt64RNG::to_double(mod.current_seed.seed), conversion_buffer));
        result.append(")");
        PUSH_BACK(result);
        // PUSH_BACK_C(string_buffer);
        // PUSH_BACK((Common::decimal2hex_str(mod.current_seed.seed) + "(" +
        //          std::to_string(Common::ZMath::UInt64RNG::to_double(mod.current_seed.seed)) +
        //          ")"));
#endif
        PUSH_BACK(s2);
        PUSH_BACK_C(Common::ryu_d2s_buffered(Tetris::v_fall_at(mod.current_level), conversion_buffer));
        PUSH_BACK(s3);
        PUSH_BACK(("{" + std::to_string(mod.controlling_piece.current_position.x) + ", " +
                   std::to_string(mod.controlling_piece.current_position.y) + ", " +
                   std::to_string(mod.controlling_piece.current_rot) + "}"));
#undef PUSH_BACK

#else
#error(PUSH_BACK already defined. Find another name.)
#endif
    }
}

void Renderer::StdTxtRenderer::assign_all(Tetris::GameModule const& mod, RenderData& out) {
    static const AssignFunction funcs[] = {assign_tetris_field, assign_upcoming, assign_scoreboard,
                                           assign_game_info};
    constexpr int n_threads = sizeof(funcs) / sizeof(AssignFunction);
    // Use n_threads to assign different sections of
    // the game's engine to the render buffer.
    std::thread th[n_threads];
    for (int i = 0; i < n_threads; i++) {
        auto func = funcs[i];
        th[i] = std::thread([&out, func, &mod]() { func(mod, out); });
        //// Open this out to make rendering synchronized
        // th[i].join();
    }
    // True parallel rendering (4 threads on CPU)
    // Wait for threads to be done
    int remain = n_threads;
    while (remain > 0) {
        for (int i = 0; i < n_threads; i++) {
            if (th[i].joinable()) {
                th[i].join();
                remain--;
            }
        }
    }
#ifdef DEBUG_DEFINED
    // VERBOSITY_LOG("\nassign_all() output: ");
    // VERBOSITY_LOG(out.render_data);
#endif
}

bool Renderer::StdTxtRenderer::try_update(Tetris::GameModule const& mod,
                                          RenderData& overriding_data) {
    assign_all(mod, overriding_data);
    return true;
}

bool Renderer::StdTxtRenderer::try_display(RenderData const& data) {
    std::cout << data.render_data << std::flush;
    return true;
}
#undef LAZY_INITIALIZE
#undef LAZY_DO_ONCE
#undef LAZY_FUNCTION_END
#endif