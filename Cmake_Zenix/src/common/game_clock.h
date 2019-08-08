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
		Instance();
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


		constexpr long long numeric_diff_nano(std::chrono::time_point<Clock> const& now) const
		{
			auto d = nano_time_diff(now);
			return d.count();
		}
		constexpr long long numeric_diff_milli(std::chrono::time_point<Clock> const& now) const
		{
			auto d = std::chrono::duration_cast<std::chrono::milliseconds>(nano_time_diff(now));
			return d.count();
		}
	};
}
}