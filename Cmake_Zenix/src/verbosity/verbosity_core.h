#pragma once

#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <iostream>

namespace Verbosity
{
	#define RELATIVE_LOG_ROOT  "./logs/"
#define LOG_ROOT(x) (RELATIVE_LOG_ROOT x)
	template<bool try_append_to_file = true> //If false, and name already exists, try to manipulate the file name.
	class VerbosityWriter
	{
	public:
		typedef std::string (*Log_On_Destruct)();
		VerbosityWriter(const std::string& log_rooted_relative_file_path);
		VerbosityWriter(const std::string& log_rooted_relative_file_path, const Log_On_Destruct dying_log);
		~VerbosityWriter();
		std::fstream output_stream;
		VerbosityWriter(const VerbosityWriter&) = delete;
		void operator=(const VerbosityWriter&) = delete;
	protected:
		const Log_On_Destruct dying_log;
	};
	struct VerbosityCore
	{
	public:
		static VerbosityWriter<false>& get_rt_logger();
		//static VerbosityWriter<false>& get_perf_logger()
		//{
		//	static VerbosityWriter<false> perf_log = VerbosityWriter<false>(LOG_ROOT("perf"));
		//	return perf_log;
		//}
	};

}
#define GET_VERBOSITY_LOGGER Verbosity::VerbosityCore::get_rt_logger()
#define VERBOSITY_LOG(string) Verbosity::VerbosityCore::get_rt_logger().output_stream<<string<<std::endl
//#define PERFORMANCE_LOG(string) Verbosity::VerbosityCore::get_perf_logger().output_stream<<string<<std::endl