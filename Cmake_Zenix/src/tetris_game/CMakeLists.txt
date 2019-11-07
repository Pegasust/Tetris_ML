# Playable implementation of Tetris Core Engine.

add_library(tetris_game STATIC
tetris_game.h
tetris_game.ipp
tetris_game.cpp
)

create_target_directory_groups(tetris_game)
target_link_libraries(tetris_game common renderer tetris video_core verbosity)