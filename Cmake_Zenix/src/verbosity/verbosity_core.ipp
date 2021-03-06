template <>
Verbosity::VerbosityWriter<true>::VerbosityWriter(const std::string& file_path) :
    dying_log([]() { return std::string("Program exiting"); }),
    output_stream(file_path, std::ios::in | std::ios::out | std::ios::app)
{
    std_filesystem::create_directory(RELATIVE_LOG_ROOT);
}

template <>
Verbosity::VerbosityWriter<true>::VerbosityWriter(
    const std::string& file_path, Log_On_Destruct dying_log) :
    dying_log(dying_log),
    output_stream(file_path, std::ios::in | std::ios::out | std::ios::trunc)
{
    std_filesystem::create_directory(RELATIVE_LOG_ROOT);
}

template <bool try_append_to_file>
Verbosity::VerbosityWriter<try_append_to_file>::~VerbosityWriter()
{
    output_stream << dying_log() << std::endl;
    output_stream.close();
    POTENTIAL_BUG_POINT_MSG("Do we need to flush format_out_str?");
    // delete[] format_out_str;
}

template <>
Verbosity::VerbosityWriter<false>::VerbosityWriter(const std::string& file_path) :
    dying_log([]() { return std::string(""); })
{
    std_filesystem::create_directory(RELATIVE_LOG_ROOT);
    std::string new_path;
    next_available_filename(file_path, new_path);
    output_stream = std::fstream(new_path, std::ios::in | std::ios::out | std::ios::trunc);
}

template <>
Verbosity::VerbosityWriter<false>::VerbosityWriter(
    const std::string& file_path, const Log_On_Destruct f_log) :
    dying_log(f_log)
{
    std_filesystem::create_directory(RELATIVE_LOG_ROOT);
    std::string new_path;
    next_available_filename(file_path, new_path);
    output_stream = std::fstream(new_path, std::ios::in | std::ios::out | std::ios::trunc);
}

Verbosity::VerbosityWriter<false>& Verbosity::VerbosityCore::get_rt_logger()
{
    static Verbosity::VerbosityWriter<false> logger
        = Verbosity::VerbosityWriter<false>(LOG_ROOT("log"));
    return logger;
}
