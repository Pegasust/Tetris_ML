# The NEAT implementation with procedural (C-like) approach.

add_library(NEAT_procedural STATIC
neat.cpp

)

create_target_directory_groups(NEAT_procedural)
target_link_libraries(NEAT_procedural neural_network_core)

add_subdirectory(tests)