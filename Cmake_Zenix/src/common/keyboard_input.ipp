#pragma once
template<bool threaded, uint64_t time_out, typename ChronoType>
int Common::KeyboardListener<threaded, time_out, ChronoType>::get_char()
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
	int actual_char = _getch();
	while (_kbhit())
	{
		actual_char = _getch();
	}
	return actual_char;
}

template<uint64_t time_out, typename ChronoType>
class Common::KeyboardListener<true, time_out, ChronoType>
{
private:
	static bool exit;
	static bool initialized;
	static std::thread input_thr;
public:
	static void exit_synchronously();
	static inline int get_key()
	{
		return get_char();
	}
protected:
	static int get_char();
};

template<uint64_t time_out, typename ChronoType>
bool Common::KeyboardListener<true, time_out, ChronoType>::exit = false;

template<uint64_t time_out, typename ChronoType>
bool Common::KeyboardListener<true, time_out, ChronoType>::initialized = false;

template<uint64_t time_out, typename ChronoType>
std::thread Common::KeyboardListener<true, time_out, ChronoType>::input_thr = std::thread();

template<uint64_t time_out, typename ChronoType>
void Common::KeyboardListener<true, time_out, ChronoType>::exit_synchronously()
{
	exit = true;
	initialized = false;
	if (input_thr.joinable())
	{
		input_thr.join();
	}
}

template<uint64_t time_out, typename ChronoType>
int Common::KeyboardListener<true, time_out, ChronoType>::get_char()
{
	static InputBufferQueue input_buffer;
	if (!initialized)
	{
		input_thr = std::thread([]()
			{
				while (!exit)
				{
					if (time_out == 0)
					{
						input_buffer.push(_getch());
					}
					else
					{
						std::this_thread::sleep_for(ChronoType(time_out));
						while (_kbhit())
						{
							input_buffer.push(_getch());
						}
					}
				}
			});
		initialized = true;
	}
	if (input_buffer.empty())
	{
		return -1;
	}
	int ret_val = input_buffer.front();
	input_buffer.pop();
	return ret_val;
}