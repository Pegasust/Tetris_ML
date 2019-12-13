#pragma once

#include <array>
#include <unordered_map>
#include <vector>
#include <stdlib.h>
#include "verbosity/verbosity_core.h"
#include "renderer_specs.h"
#ifdef __cplusplus
#include <iostream>
#include <string>
#endif
namespace Renderer {
/**Renderers must be implemented with the following contents:
- Encapsulated within ifdef
- public typedef: RenderData
- using MainRenderer = <RendererName>
- static void clear_screen() //As means to clear the current display
 -static bool try_initialize(const ::Tetris::GameModule&, RenderData&)
- static bool try_update(const ::Tetris::GameModule&, RenderData&)
- static bool try_display(const RenderData&)
*/
#ifdef USE_TXT_CONSOLE_RENDERER
class StdTxtRenderer {
public:
    // strings are separated from lines
    typedef std::vector<std::string> RenderStrings;
    typedef std::string RenderData;
    static void clear_screen();
    static bool try_initialize(Tetris::GameModule const& mod, RenderData& new_data);
    static bool try_update(Tetris::GameModule const& mod, RenderData& overriding_data);
    /**
     * Renders data to the display.
     * In this case, dump everything to std::cout and std::endl it.
     */
    static bool try_display(const RenderData& data);

    /**
     * Returns a character that represents bodytype.
     * Uses local static std::unordered_map to lookup.
     */
    static char body_type_2_char(Tetris::BodyType const& body);

    /**
     * Parallelly assign tetris_field_string(), tetris_upcoming_pieces(), ...
     * to an array of RenderStrings, then merge all elements in that array
     * to str using ITERATE macro, which ADDS each RenderStrings to
     * str.
     *
     * Pre-condition:
     *  - str is newly created std::string (empty string).
     */
    static void assign_empty_render_string(Tetris::GameModule const& mod, const bool& shadow,
                                           std::string& str);
    static void override_render_string(Tetris::GameModule const& mod, const bool& shadow,
                                       std::string& str);

private:
    typedef int CharPosition;
    using RendererMath = RendererExt::Math<CharPosition>;
    // static std::vector
    // This supports shadowing
    static RenderStrings tetris_field_string(Tetris::GameModule const& mod);
    // Does not support shadowing
    static RenderStrings tetris_field_string(Tetris::TetrisField const& game_field,
                                             Tetris::TetrisBody const& controlling_piece);
    static RenderStrings tetris_upcoming_pieces(std::list<Tetris::BodyType> coming_pieces);
    static RenderStrings tetris_upcoming_pieces(Tetris::GameModule const& mod);
    static RenderStrings tetris_scoreboard(Tetris::GameModule const& mod);
    static RenderStrings tetris_game_info(Tetris::GameModule const& mod);
    /// The number of elements in priority_queue in func_from_render_order
    static constexpr int QUEUE_LENGTH = 4;
    typedef std::array<Renderer::StdTxtRenderer::RenderStrings, QUEUE_LENGTH> RenderStrsArray;
    static RenderStrsArray& tetris_array_renderstrings(Tetris::GameModule const& mod);

    typedef RenderStrings (*StringGetFunction)(Tetris::GameModule const&);
    typedef std::unordered_map<RendererExt::Priority, StringGetFunction> get_func_map;

    /**
     * Get execution function from integer that represents the render order.
     * This is used (at least) in assign_empty_render_string.
     * Uses static unordered_map, creates and sorts local priority_queue
     * by the first time this function is called.
     */
    static StringGetFunction func_from_render_order(const int& render_order);
};
using MainRenderer = StdTxtRenderer;
#endif

#ifdef USE_REVISED_TXT_CONSOLE_RENDERER
class StdTxtRenderer {
public:
    struct RenderData {
        static const int RENDER_DATA_SIZE =
            // The extra +1 is \n element.
            (RendererExt::TXT_RENDERER_WIDTH + 1) * RendererExt::TXT_RENDERER_HEIGHT;

        RenderData() : render_data(RENDER_DATA_SIZE, ' ') {
            int line_end;
            for (int i = 0; i < RendererExt::TXT_RENDERER_HEIGHT; i++) {
                // Assign \n at the end of each line
                line_end =
                    RendererExt::xy2i<int>(static_cast<int>(RendererExt::TXT_RENDERER_WIDTH), i);
                render_data[line_end] = '\n';
            }
        }
        std::string render_data;
    };
    /**
     * Technically clears the screen. Does not guarantee that std::cout
     * will be flushed (most likely not).
     */
    static void clear_screen();
    /**
     * Initialize new_data so that it is ready to be passed
     * to try_display to display the NULL state of
     * game module (mod).
     *
     * Pre-condition:
     *  - new_data is constructed by default constructor (string of lots of newlines).
     */
    static bool try_initialize(Tetris::GameModule const& mod, RenderData& new_data);
    /**
     * Update overriding_data so that it reflects the current state
     * of game module. try_display() will use this exact output
     * to render the current state of game module.
     */
    static bool try_update(Tetris::GameModule const& mod, RenderData& overriding_data);
    static bool try_display(RenderData const& data);

private:
    static char body_type_2_char(Tetris::BodyType const& body);
    typedef std::vector<std::string> RenderStrings;
    typedef RenderStrings (*StringGetFunction)(Tetris::GameModule const&);
    typedef std::unordered_map<RendererExt::Priority, StringGetFunction> get_func_map;

    static void assign_tetris_field(Tetris::GameModule const& mod, RenderData& out);
    static void assign_upcoming(std::queue<Tetris::BodyType> coming_pieces, RenderData& out);
    static void assign_upcoming(Tetris::GameModule const& mod, RenderData& out);
    static void assign_scoreboard(Tetris::GameModule const& mod, RenderData& out);
    static void assign_game_info(Tetris::GameModule const& mod, RenderData& out);
    /// There are 4 render fields with 4 funcs: assign(tetris_field, upcoming, scoreboard, game_info)
    static constexpr int N_RENDER_FIELDS = 4;
    /**
     * Parallelly assign all render fields into RenderData& out.
     */
    static void assign_all(Tetris::GameModule const& mod, RenderData& out);
    typedef void (*AssignFunction)(Tetris::GameModule const&, RenderData&);
};
using MainRenderer = StdTxtRenderer;
#endif
} // namespace Renderer