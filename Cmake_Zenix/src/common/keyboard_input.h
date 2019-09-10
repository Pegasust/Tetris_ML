#pragma once
//This is only for temporary as this would only work on windows
#ifdef WINDOWS_DEFINED
#include <conio.h>
#endif
#include <thread>
#include "zmath.h"
namespace Common
{
	//TODO: Threading not implemented, sorry
	template<bool threaded, typename ChronoType, uint64_t time_out>
	class KeyboardListener
	{
	public:
		static constexpr int get_key()
		{
			return get_char();
		}
	protected:
		static constexpr int get_char()
		{
			if (time_out == 0)
			{
				//Block program
				return _getch();
			}
			auto invoke_tm = std::chrono::high_resolution_clock::now();
			std::chrono::high_resolution_clock::time_point tm_out = invoke_tm +
				ChronoType(time_out);

			while (!_kbhit())
			{
				if (std::chrono::high_resolution_clock::now() > tm_out)
				{
					return -1;
				}
			}
			return _getch();
		}
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