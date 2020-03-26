#pragma once
#include <chrono>
namespace Common
{
namespace GameClock
{
	typedef std::chrono::high_resolution_clock Clock;
	class Instance
	{
	public:
		std::chrono::time_point<Clock> then;
		Instance(bool initialize_time = true);
		constexpr std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds>
			nano_time_diff(std::chrono::time_point<Clock> const& now) const
		{
			return now - then;
		}
		constexpr std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds>
			nano_time_diff() const
		{
			return Clock::now() - then;
		}
		void reset_then();
        /*Resets the timer, retrieve the last "then"*/
        std::chrono::time_point<Clock> reset();

		constexpr long long numeric_diff_nano(std::chrono::time_point<Clock> const& now = Clock::now()) const
		{
			auto d = nano_time_diff(now);
			return d.count();
		}
		constexpr long long numeric_diff_milli(std::chrono::time_point<Clock> const& now = Clock::now()) const
		{
			auto d = std::chrono::duration_cast<std::chrono::milliseconds>(nano_time_diff(now));
			return d.count();
		}
		/*get time difference from nano in milliseconds*/
		constexpr double fp_diff_milli(std::chrono::time_point<Clock> const& now = Clock::now()) const
		{
			long long x = numeric_diff_nano(now);
			return static_cast<double>(x) / 1e6;
		}
		/*get time difference from nano in seconds*/
		constexpr double fp_diff_seconds(std::chrono::time_point<Clock> const& now = Clock::now()) const
		{
			long long x = numeric_diff_nano(now);
			return static_cast<double>(x) / 1e9;
		}
		/*get time difference from milliseconds to seconds*/
		constexpr double fp_diff_seconds_milli(std::chrono::time_point<Clock> const& now = Clock::now()) const
		{
			long long x = numeric_diff_milli(now);
			return static_cast<double>(x) / 1e3;
		}
		/*get time difference from microseconds to seconds*/
		constexpr double fp_diff_seconds_micro(std::chrono::time_point<Clock> const& now = Clock::now()) const
		{
			long long x = (std::chrono::duration_cast<std::chrono::microseconds>(now - then)).count();
			return static_cast<double>(x) / 1e3;
		}
	};
}
}