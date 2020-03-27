#include <string>
#include "../../common/common.hpp"
#include "../../tetris_api/tetris_extended_engine.h"
#include "../../video_core/video_core.h"
#include "../serialize_format.h"

#define RENDER_RECREATION

void parse_ignore_test() {
    std::string test_strs[] = {
        "test",
        "012345",
        R"(Here is a multiline example
You should not parse any weird stuff beyond the last line.
You hear me?)",
        "Please ignore the rest of the line, alright?## So the other day \
I dreamed about me getting the Corona Virus so I had to manually hide my \
face in public. Everywhere I go, as long as I said \"I got it\", every\
one splits in half in front of me.\n",
        "## Nothing should be parsed. Got it?",
        R"(This is a test for bound_ignore.//Make sure you ignore this message because it is within the bound.
Even after the newline, you should still be looking and parse the important message right after I close this bound.// Ohayo Sekai.)",

        R"(Ignore the following lines.// So the bound ignore comes priority before the
ignore line <##> because it appeared before ignore line.
Make sure the following message is not dropped.// Unrequited love X-Y.)",
        R"(This one is quite tricky, because you face the limit. See, aa## Do not parse anything beyond this
Also ignore this too.)"};
    std::string result[] = {"test",
                            "012345",
                            "Here is a multiline example",
                            "Please ignore the rest of the line, alright?",
                            "",
                            "This is a test for bound_ignore. Ohayo Sekai.",
                            "Ignore the following lines. Unrequited love X-Y.",
                            "This one is quite tricky, because you face the limit. See, aa"};
    for (int i = 0; i < Common::array_size(test_strs); i++) {
        std::cout << "Parsing: " << test_strs[i] << std::endl;
        bool success;
        char buffer[TetrisReplay::DESERIALIZE_BUFFER];
        int strlength;
        TetrisReplay::__private__::parse_ignore(success, buffer, strlength,
                                                std::istringstream(test_strs[i]));
        std::cout << "result: " << result[i] << "  \tparsed: " << buffer;
        std::cout << "\nsuccess: " << success << "\tstring_length: " << strlength;
        std::cout << std::endl;
        std::cout << std::endl;
        ASSERT(std::string(buffer) == result[i], "parsed string does not meet expectation.");
    }
}

constexpr ::Tetris::Input key2input(const Common::KeyID& key) {
    switch (key) {
    case -1:
        return ::Tetris::Input::NONE;
    case 80:
        return ::Tetris::Input::DOWN;
    case 72:
        return ::Tetris::Input::UP;
    case 75:
        return ::Tetris::Input::LEFT;
    case 77:
        return ::Tetris::Input::RIGHT;
    case 32:
        return ::Tetris::Input::CAST_DOWN;
    case 122:
        return ::Tetris::Input::ROTATE_BACK;
    case 120:
        return ::Tetris::Input::ROTATE_FORTH;
    case 62:
        return ::Tetris::Input::PROGRAM_EXIT;
    }
}
int simulate_game(TetrisAPI::TetrisExtendedEngine& out_extended_engine) {
    // initialize modules
    int random = rand();
    int initial_seed = random;
    TetrisAPI::TetrisExtendedEngine extended_engine(random);
    VideoCore::VideoHandler handler;
    handler.start_async_display(extended_engine.engine);
    handler.pause_displaying();
    Renderer::MainRenderer::try_initialize(extended_engine.engine, handler.display_data);
    // saved meta-data
    unsigned long int keys_registered = 0;
    double time_elapsed = 0.0;
    std::vector<double> staticized_seconds;
    std::vector<::Tetris::Position2D> staticized_positions;
    ::Tetris::TetrisBody controlling_body(::Tetris::BLANK);
    std::cout << "Complete initialization. Seed: " << initial_seed << std::endl;
    std::cout << "Press any key to start." << std::endl;
    Common::SynchronousKeyboard::get_key();
    handler.resume_displaying(); // Race condition :D
    // start physics update (human player)
    while (true) {
        Common::KeyID key = Common::BufferedKeyboard::get_key();
        ::Tetris::Input input = key2input(key);
        if (input == ::Tetris::Input::PROGRAM_EXIT) {
            handler.stop_displaying();
            extended_engine.engine.lost = true;
            Common::BufferedKeyboard::exit_synchronously();
            return 0;
        }
        unsigned char n_burned;
        unsigned char y_coord_burned[4];
        double update_took_seconds;
        bool piece_staticized;
        ::Tetris::TetrisField field =
            extended_engine.update(input, piece_staticized, controlling_body, y_coord_burned,
                                   n_burned, update_took_seconds);
        time_elapsed += update_took_seconds;
        if (piece_staticized) {
            TetrisAPI::reassign_piece(extended_engine.engine);
            staticized_seconds.push_back(time_elapsed);
            staticized_positions.push_back(controlling_body.current_position);
        }
        Renderer::MainRenderer::try_update(extended_engine.engine, handler.display_data);
        handler.keep_displaying_data = true;
        if (extended_engine.engine.lost) {
            break;
        }
        // Not lost.
    }
    handler.stop_displaying();
    unsigned long long rows_burned_human = extended_engine.engine.n_rows_burned;
    std::cout << "time_elapsed: " << time_elapsed << std::endl;
    std::cout << "staticized_times: " << staticized_seconds.size() << std::endl;
    std::cout << "rows_burned: " << extended_engine.engine.n_rows_burned << std::endl;
    keys_registered = extended_engine.input_collection.collection.size();
    std::cout << "calculated_rows_burned: " << rows_burned_human << std::endl;
    std::cout << "Keystrokes registered: " << keys_registered << std::endl;
    std::cout << "Press any key to continue." << std::endl;
    unsigned long int rows_burned = extended_engine.engine.n_rows_burned;
    Common::SynchronousKeyboard::get_key();
    Common::BufferedKeyboard::exit_synchronously();
    out_extended_engine = std::move(extended_engine);
    return initial_seed;
}
int recreate(const std::string& col_str, TetrisAPI::TetrisExtendedEngine& engine) {
    auto original_size = engine.input_collection.collection.size();
    engine.input_collection.clear();
    engine.input_collection.add_entries(col_str);
    std::cout << "original size: " << original_size
              << "\tadded_size: " << engine.input_collection.collection.size() << std::endl;
    ASSERT(original_size == engine.input_collection.collection.size(), "size mismatch!");
    VideoCore::VideoHandler handler;
    handler.start_async_display(engine.engine);
    // Renderer::MainRenderer::try_initialize(extended_engine.engine, handler.display_data);
    // saved meta-data
    double time_elapsed_re = 0.0;
    std::vector<double> staticized_seconds_re;
    std::vector<::Tetris::Position2D> staticized_positions_re;
    ::Tetris::TetrisBody controlling_body_re(::Tetris::BLANK);
    std::cout << "Complete initialization" << std::endl;
    // start recreation
    while (true) {
        Common::KeyID key = Common::BufferedKeyboard::get_key();
        ::Tetris::Input input = key2input(key);
        if (input == ::Tetris::Input::PROGRAM_EXIT) {
            handler.stop_displaying();
            engine.engine.lost = true;
            Common::BufferedKeyboard::exit_synchronously();
            return 0;
        }
        unsigned char n_burned;
        unsigned char y_coord_burned[4];
        double update_took_seconds;
        bool piece_staticized;
        ::Tetris::TetrisField field = engine.recreate_once(
            piece_staticized, controlling_body_re, y_coord_burned, n_burned, update_took_seconds);
        time_elapsed_re += update_took_seconds;
        if (piece_staticized) {
            TetrisAPI::reassign_piece(engine.engine);
            staticized_seconds_re.push_back(time_elapsed_re);
            staticized_positions_re.push_back(controlling_body_re.current_position);
        }
        if (engine.engine.lost) {
            break;
        }
// Not lost.
#ifdef RENDER_RECREATION
        Renderer::MainRenderer::try_update(engine.engine, handler.display_data);
        handler.keep_displaying_data = true;
#endif
    }
    handler.stop_displaying();
}
int replay_parse_test() {
    TetrisAPI::TetrisExtendedEngine engine;
    int initial_seed = simulate_game(engine);
    Renderer::StdTxtRenderer::clear_screen();
    std::cout << std::flush;
    std::cout << "file output: " << std::endl;
    std::string replay_str;
    TetrisReplay::RandomSeed seed = engine.reset_for_recreation();
    TetrisReplay::serialize_replay(replay_str, seed, engine.input_collection);
    std::cout << replay_str << std::endl;
    TetrisReplay::RandomSeed parsed_seed;
    std::string collection;
    TetrisReplay::deserialize_replay(parsed_seed, collection, std::istringstream(replay_str));
    std::cout << "collection str: \n" << collection << std::endl;
    std::cout << "Initial seed: " << initial_seed << "\tseed: " << seed
              << "\tparsed_seed: " << parsed_seed << std::endl;
    ASSERT(parsed_seed == seed, "Parsed seed is different from seed");

    std::cout << "Press any key to perform recreation." << std::endl;
    Common::SynchronousKeyboard::get_key();
    recreate(collection, engine);
    Common::BufferedKeyboard::exit_synchronously();
    return 0;
}

int main() {
    parse_ignore_test();
    replay_parse_test();
}