# Initial test of communicating between CMake to C++ by using
# define.
message(STATUS "Accessing initial_tests dir")
add_executable(initial_tests
processor_count.cc
)

create_target_directory_groups(initial_tests)
target_link_libraries(initial_tests test_utils common)