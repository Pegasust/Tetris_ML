add_library (replay STATIC
serialize_format.cpp
serialize_format.h
)
create_target_directory_groups(replay)

target_link_libraries(replay common tetris)
add_subdirectory(tests)