add_executable(neat_test
neat_test.cpp
)

create_target_directory_groups(neat_test)

target_link_libraries(neat_test neat test_utils)