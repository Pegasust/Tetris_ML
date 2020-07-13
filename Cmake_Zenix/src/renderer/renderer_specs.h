#pragma once

#include <type_traits>
#include "..//tetris/game_module.h"
// Here is the list of macros that can be defined.
// If multiple flags are set, the program will *HOPEFULLY* run every flag set
// by the program



constexpr int FRAMETIME_MILLIS =
#ifdef DEBUG_DEFINED
    static_cast<int>(1000.0 / 60.0);
#else
static_cast <int>(1000.0 / 240.0);
#endif
// #define USE_TXT_CONSOLE_RENDERER
#define USE_REVISED_TXT_CONSOLE_RENDERER
//#define USE_OPENCL_RENDERER //NOT IMPLEMENTED YET

// Here is the region which the design is set.

#if defined(USE_TXT_CONSOLE_RENDERER) || defined(USE_REVISED_TXT_CONSOLE_RENDERER)

// Top left is {0, 0}, bot right is {x, y} (unsigned)
namespace RendererExt {
constexpr float TXT_RENDERER_WIDTH = (Tetris::TetrisField::WIDTH * 8);
constexpr float TXT_RENDERER_HEIGHT = (Tetris::TetrisField::HEIGHT * 2);
constexpr float TXT_RENDERER_MID_WID = TXT_RENDERER_WIDTH / 2.0f;
constexpr float TXT_RENDERER_MID_HEIGHT = TXT_RENDERER_HEIGHT / 2.0f;

template <class T, typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
constexpr T xy2i(const T& x, const T& y) {
    // + 1 because of trailing '\n'
    return y * static_cast<T>(TXT_RENDERER_WIDTH+1) + x;
}


template <class T, typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
constexpr void i2xy(T& x, T& y, const T& i) {
    const T WIDTH = static_cast<T>(TXT_RENDERER_WIDTH+1);
    y = static_cast<T>(i) / WIDTH;
    x = static_cast<T>(i) - (y * WIDTH);
}
const int cx = Common::ZMath::round_nearest(TXT_RENDERER_WIDTH / 2.0f);
const int cy = Common::ZMath::round_nearest(TXT_RENDERER_HEIGHT / 2.0f);
// constexpr int CENTER_X = Common::ZMath::round_nearest(TXT_RENDERER_MID_WID);
const char SHADOW_CHAR = '-';
enum RenderPosition {
    TOP_LEFT,
    TOP_MID,
    TOP_RIGHT,
    MID_LEFT,
    CENTER,
    MID_RIGHT,
    BOT_LEFT,
    BOT_MID,
    BOT_RIGHT
};
template <typename T>
class Math {
public:
    /**
     * Get the x and y position of the according
     * r_pos (RenderPosition) at mod_x and mod_y.
     * The output values are assigned to ox and oy.
     * ox and oy is the top-left of the designated r_pos.

     * Note: mod_x and mod_y can be offset
     * or the rendering object's width or height.
     * Generally, passing in STRUCT_NAME.MOD_X and
     * STRUCT_NAME.MOD_Y should be okay.
     */
    static inline void general_get(RenderPosition const& r_pos, T& ox, T& oy, int const& mod_x,
                                   int const& mod_y) {
        switch (r_pos) { // should have used macros, silly me
        case TOP_LEFT:
            get_top_left(ox, oy, mod_x, mod_y);
            break;
        case TOP_MID:
            get_top_mid(ox, oy, mod_x, mod_y);
            break;
        case TOP_RIGHT:
            get_top_right(ox, oy, mod_x, mod_y);
            break;
        case MID_LEFT:
            get_mid_left(ox, oy, mod_x, mod_y);
            break;
        case CENTER:
            get_center(ox, oy, mod_x, mod_y);
            break;
        case MID_RIGHT:
            get_mid_right(ox, oy, mod_x, mod_y);
            break;
        case BOT_LEFT:
            get_bot_left(ox, oy, mod_x, mod_y);
            break;
        case BOT_MID:
            get_bot_mid(ox, oy, mod_x, mod_y);
            break;
        case BOT_RIGHT:
            get_bot_right(ox, oy, mod_x, mod_y);
            break;
        }
    }

    static inline void get_center(T& center_x, T& center_y, int const& obj_width,
                                  int const& obj_height) {
        center_x = static_cast<T>(cx - obj_width / 2);
        center_y = static_cast<T>(cy - obj_height / 2);
    }
    static inline void get_top_left(T& out_x, T& out_y, int const& offset_x, int const& offset_y) {
        out_x = static_cast<T>(offset_x);
        out_y = static_cast<T>(offset_y);
    }
    static inline void get_top_right(T& out_x, T& out_y, int const& offset_x, int const& offset_y) {
        out_x = static_cast<T>(static_cast<int>(TXT_RENDERER_WIDTH) - offset_x);
        out_y = static_cast<T>(offset_y);
    }
    static inline void get_top_mid(T& out_x, T& out_y, int const& obj_width, int const& offset_y) {
        out_x = static_cast<T>(cx - obj_width / 2);
        out_y = static_cast<T>(offset_y);
    }
    static inline void get_mid_left(T& out_x, T& out_y, int const& offset_x,
                                    int const& obj_height) {
        out_x = static_cast<T>(offset_x);
        out_y = static_cast<T>(cy - obj_height / 2);
    }
    static inline void get_mid_right(T& out_x, T& out_y, int const& offset_x,
                                     int const& obj_height) {
        out_x = static_cast<T>(TXT_RENDERER_WIDTH - offset_x);
        out_y = static_cast<T>(cy - obj_height / 2);
    }
    static inline void get_bot_left(T& out_x, T& out_y, int const& offset_x, int const& offset_y) {
        out_x = static_cast<T>(offset_x);
        out_y = static_cast<T>(TXT_RENDERER_HEIGHT - offset_y);
    }
    static inline void get_bot_mid(T& out_x, T& out_y, int const& obj_width, int const& offset_y) {
        out_x = static_cast<T>(cx - obj_width / 2);
        out_y = static_cast<T>(TXT_RENDERER_HEIGHT - offset_y);
    }
    static inline void get_bot_right(T& out_x, T& out_y, int const& offset_x, int const& offset_y) {
        out_x = static_cast<T>(TXT_RENDERER_WIDTH - offset_x);
        out_y = static_cast<T>(TXT_RENDERER_HEIGHT - offset_y);
    }
};

enum RenderMode { HORIZONTAL, VERTICAL };
// The higher it is, the more in front this is. (Which means it will be rendered later)
typedef unsigned int Priority;

#define DECLARE_RENDER_ENTRY(name, render_mode, render_position, mod_x, mod_y, priority)           \
    struct name {                                                                                  \
                                                                                                   \
        static const RenderMode MODE = render_mode;                                                \
        static const RenderPosition POSITION = render_position;                                    \
        static const int MOD_X = mod_x;                                                            \
        static const int MOD_Y = mod_y;                                                            \
        static const Priority PRIORITY = priority;                                                 \
    };

DECLARE_RENDER_ENTRY(GAME_FIELD, RenderMode::VERTICAL, RenderPosition::CENTER,
                     Tetris::TetrisField::WIDTH, Tetris::TetrisField::HEIGHT, 4);
// const RenderMode GAME_FIELD_MODE = VERTICAL; //It would be weird if otherwise
// const RenderPosition GAME_FIELD_POSITION = CENTER;
// constexpr int GAME_FIELD_MOD_X = Tetris::TetrisField::WIDTH;
// constexpr int GAME_FIELD_MOD_Y = Tetris::TetrisField::HEIGHT;

DECLARE_RENDER_ENTRY(UPCOMING_PIECES, RenderMode::VERTICAL, RenderPosition::MID_RIGHT, sizeof("# UPCOMING #")+3,
                     (2 + 5 * Tetris::GameModule::N_PIECE_AHEAD), 0);
/*
12345678910
#UPCOMING#	1
#        #		2
#    XX  #		3
#	 XX  #		4
#        #		5
#        #		6
#        #		7
#    X   #		8
#    XX	 #		9
#     X  #		10
#					 #		11
#					 #		12
#	  X			 #		13
#	  X			 #		14
#	  X			 #		15
#	  X			 #		16
########		17
*/

DECLARE_RENDER_ENTRY(SCORE_BOARD, RenderMode::VERTICAL, RenderPosition::TOP_LEFT, 5, 2,
                     3); // Includes current score, high score, tetris scored, rows burned, level

DECLARE_RENDER_ENTRY(GAME_INFO, RenderMode::VERTICAL, RenderPosition::BOT_LEFT, 5, 8,
                     2); // Includes current_seed, fall_velocity, current position, etc.

#undef DECLARE_RENDER_ENTRY

//===============Priority=================
// class Prioritizing
//{
// public:
//	template <typename ReturnType,typename Arg, typename... Types, typename
//std::enable_if<std::is_integral<Arg>::value>::type * = nullptr> 	inline ReturnType*
//get_order_of(int order, Arg obj_ptr, uint)
//};
} // namespace RendererExt
#endif