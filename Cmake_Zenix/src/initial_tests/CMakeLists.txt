message(STATUS "Accessing initial_tests dir")
add_library(initial_tests STATIC
processor_count.cc
)

create_target_directory_groups(initial_tests)