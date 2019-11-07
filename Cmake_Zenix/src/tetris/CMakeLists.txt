# Tetris Engine Core Library

message(STATUS "Accessing tetris dir")
add_library(tetris STATIC 
game_specs.h
tetris_pieces.h
tetris_pieces.cpp
game_module.h
game_module.cpp
)

create_target_directory_groups(tetris)

target_link_libraries(tetris common)