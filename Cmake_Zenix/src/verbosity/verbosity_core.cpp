#include "verbosity_core.h"

template<>
Verbosity::VerbosityWriter<true>::VerbosityWriter(const std::string& file_path):dying_log([]()
{
	return std::string("");
}),
output_stream(file_path, std::ios::in|std::ios::out|std::ios::app)
{
	std_filesystem::create_directory(RELATIVE_LOG_ROOT);
}

template<>
Verbosity::VerbosityWriter<true>::VerbosityWriter(const std::string& file_path, Log_On_Destruct dying_log)
	:
	dying_log(dying_log),
	output_stream(file_path, std::ios::in | std::ios::out | std::ios::trunc)
{
	std_filesystem::create_directory(RELATIVE_LOG_ROOT);
}

template<bool try_append_to_file>
Verbosity::VerbosityWriter<try_append_to_file>::~VerbosityWriter()
{
	output_stream << dying_log() << std::endl;
	output_stream.close();
}

template<>
Verbosity::VerbosityWriter<false>::VerbosityWriter(const std::string& file_path) :dying_log([]()
	{
		return std::string("");
	})	
{
	std_filesystem::create_directory(RELATIVE_LOG_ROOT);
	if (std_filesystem::exists(file_path))
	{
		unsigned int i = 0;
		std::string new_path;
		while (std_filesystem::exists(new_path = file_path + std::to_string(i++)))
		{

		}
		output_stream = std::fstream(new_path, std::ios::in | std::ios::out | std::ios::trunc);
	}
	else
	{
		output_stream = std::fstream(file_path, std::ios::in | std::ios::out| std::ios::trunc);
	}
}

template<>
Verbosity::VerbosityWriter<false>::VerbosityWriter(const std::string& file_path, const Log_On_Destruct f_log) :
	dying_log(f_log)
{
	std_filesystem::create_directory(RELATIVE_LOG_ROOT);
	if (std_filesystem::exists(file_path))
	{
		unsigned int i = 0;
		std::string new_path;
		while (std_filesystem::exists(new_path = file_path + std::to_string(i++)))
		{
		}
		output_stream = std::fstream(new_path, std::ios::in | std::ios::out| std::ios::trunc);
	}
	else
	{
		output_stream = std::fstream(file_path, std::ios::in | std::ios::out|std::ios::trunc);
	}
}

Verbosity::VerbosityWriter<false>& Verbosity::VerbosityCore::get_rt_logger()
{
	static Verbosity::VerbosityWriter<false> logger = Verbosity::VerbosityWriter<false>(LOG_ROOT("log"));
	return logger;
}
