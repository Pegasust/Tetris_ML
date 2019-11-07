# Library that helps with logging performance 

add_library(verbosity STATIC
framerate.cpp
framerate.h
verbosity_core.cpp
verbosity_core.h
)

create_target_directory_groups(verbosity)
target_link_libraries(verbosity common)