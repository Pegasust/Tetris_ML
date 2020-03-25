# Tetris Engine Core Library

message(STATUS "Accessing tetris dir")
add_library(tetris STATIC 
game_specs.h
tetris_pieces.h
tetris_pieces.cpp
game_module.h
game_module.cpp
game_input.h
game_input.cpp
game_input.ipp
)

create_target_directory_groups(tetris)

target_link_libraries(tetris common)