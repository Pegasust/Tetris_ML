# Implementation of visual from renderer

add_library(video_core STATIC
video_core.h
video_core.cpp
)

add_executable(video_core_exe
tester.cpp)

create_target_directory_groups(video_core)
create_target_directory_groups(video_core_exe)
target_link_libraries(video_core common renderer verbosity)
target_link_libraries(video_core_exe video_core)