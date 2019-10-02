#pragma once
///



//This is only for temporary as this would only work on windows
#ifdef WINDOWS_DEFINED
#include <conio.h>
#else
#error Error: Linux keyboard support is not yet implemented
#endif
#include <thread>
#include "zmath.h"
#include <queue>

namespace Common
{
	using KeyID = int;
	using InputBufferQueue = std::queue<Common::KeyID>;

	template<bool threaded, uint64_t time_out, typename ChronoType>
	class KeyboardListener
	{
	public:
		static inline int get_key()
		{
			return get_char();
		}
	protected:
		static int get_char();
	};
	//template<uint64_t time_out_ns>
	//class KeyboardListener<false, time_out_ns>
	//{
	//public:
	//	static inline int get_key()
	//	{
	//		return KeyboardListener<false, time_out_ns>::get_char();
	//	}
	//};
	//template<bool threaded>
	//class KeyboardListener<threaded, 0>
	//{
	//protected:
	//	static inline int get_char()
	//	{
	//		return _getch();
	//	}
	//};
}
#include "keyboard_input.ipp"