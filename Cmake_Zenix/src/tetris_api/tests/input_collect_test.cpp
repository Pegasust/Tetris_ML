#include "../../common/common.hpp"
#include "../../video_core/video_core.h"
#include "../tetris_extended_engine.h"

#define RENDER_RECREATION
//#define STEP_RECREATION
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
int main() {
    // initialize modules
    int random = rand();
    int initial_seed = random;
    TetrisAPI::TetrisExtendedEngine extended_engine(random);
    VideoCore::VideoHandler handler;
    handler.start_async_display(extended_engine.engine);
    Renderer::MainRenderer::try_initialize(extended_engine.engine, handler.display_data);
    // saved meta-data
    unsigned long int keys_registered = 0;
    double time_elapsed = 0.0;
    std::vector<double> staticized_seconds;
    std::vector<::Tetris::Position2D> staticized_positions;
    ::Tetris::TetrisBody controlling_body(::Tetris::BLANK);
    std::cout << "Complete initialization. Seed: " << initial_seed << std::endl;
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
    std::cout << "Press any key to perform recreation." << std::endl;
    unsigned long int rows_burned = extended_engine.engine.n_rows_burned;
    Common::SynchronousKeyboard::get_key();
    // Serialize test
    std::string replay_str;
    TetrisReplay::serialize_replay(replay_str, extended_engine.initial_seed,
                                   extended_engine.input_collection);

    // extended_engine.reset(initial_seed);
    // auto seed = extended_engine.reset_for_recreation();
    auto seed = extended_engine.initial_seed;
    extended_engine.reset(replay_str);
        // VideoCore::VideoHandler handler;
        handler.start_async_display(extended_engine.engine);
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
            extended_engine.engine.lost = true;
            Common::BufferedKeyboard::exit_synchronously();
            return 0;
        }
        unsigned char n_burned;
        unsigned char y_coord_burned[4];
        double update_took_seconds;
        bool piece_staticized;
        ::Tetris::TetrisField field = extended_engine.recreate_once(
            piece_staticized, controlling_body_re, y_coord_burned, n_burned, update_took_seconds);
        time_elapsed_re += update_took_seconds;
        if (piece_staticized) {
            TetrisAPI::reassign_piece(extended_engine.engine);
            staticized_seconds_re.push_back(time_elapsed_re);
            staticized_positions_re.push_back(controlling_body_re.current_position);
        }
        if (extended_engine.engine.lost) {
            break;
        }
// Not lost.
#ifdef RENDER_RECREATION
        Renderer::MainRenderer::try_update(extended_engine.engine, handler.display_data);
        handler.keep_displaying_data = true;
#endif
    }
    handler.stop_displaying();
    std::cout << "=== Recreation result:" << std::endl;
    std::cout << "time_elapsed: " << time_elapsed_re << std::endl;
    std::cout << "staticized_times: " << staticized_seconds_re.size() << std::endl;
    std::cout << "rows_burned: " << extended_engine.engine.n_rows_burned << std::endl;
    std::cout << "Keystrokes registered: " << keys_registered << std::endl;
    std::cout << std::endl;
    std::cout << "=== Human player result:" << std::endl;
    std::cout << "time_elapsed: " << time_elapsed << std::endl;
    std::cout << "staticized_times: " << staticized_seconds.size() << std::endl;
    std::cout << "rows_burned: " << rows_burned_human << std::endl;
    ASSERT(time_elapsed == time_elapsed_re, "time elapsed mismatch");
    ASSERT(staticized_positions == staticized_positions_re, "staticized positions mismatch");
    ASSERT(staticized_seconds_re.size() == staticized_seconds.size(),
           "staticized seconds mismatch");
    if (staticized_seconds != staticized_seconds_re) {
        for (int i = 0; i < staticized_seconds.size(); i++) {
            std::cout << "staticized_sec_human: " << staticized_seconds.at(i);
            std::cout << "\tstaticized_sec_rec: " << staticized_seconds_re.at(i) << std::endl;
        }
    }
    Common::BufferedKeyboard::exit_synchronously();
    ASSERT(initial_seed == seed, "seed mismatch.");
    std::cout << "seed was: " << seed << std::endl;

    return 0;
}