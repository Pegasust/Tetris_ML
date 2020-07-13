#pragma once
///

// This is only for temporary as this would only work on windows
#ifdef WINDOWS_DEFINED
#include <conio.h>
#else
#error Error: Linux keyboard support is not yet implemented
#endif
#include <thread>
#include <queue>
#include "zmath.h"
#include "game_clock.h"
#include <string>
#include <iostream>
#include "assertion.hpp"
namespace Common {
using KeyID = int;
using InputBufferQueue = std::queue<Common::KeyID>;

template <bool threaded, uint64_t time_out, typename ChronoType = std::chrono::seconds>
class KeyboardListener {
public:
    /*
    Returns the keystroke recorded by the OS. If the user made
    no keystroke since the last update, returns -1.
     */
    static inline int get_key() {
        return get_char();
    }
    /* This should wait for ChronoType(time_out) amount of time. If nothing is entered,
     * return default_value. Currently doesn't support waiting & timeout.
     * Method is broken, there is no standard way for async std::cin I/O.
     */
    static std::string get_cin_line(const std::string& default_value);

protected:
    static int get_char();
};
// template<uint64_t time_out_ns>
// class KeyboardListener<false, time_out_ns>
//{
// public:
//	static inline int get_key()
//	{
//		return KeyboardListener<false, time_out_ns>::get_char();
//	}
//};
// template<bool threaded>
// class KeyboardListener<threaded, 0>
//{
// protected:
//	static inline int get_char()
//	{
//		return _getch();
//	}
//};
} // namespace Common
#include "keyboard_input.ipp"