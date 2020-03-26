# Tests for Tetris Engine Core library

add_executable(input_serialize_test
input_serialize_test.cpp
)

create_target_directory_groups(input_serialize_test)

target_link_libraries(input_serialize_test tetris)