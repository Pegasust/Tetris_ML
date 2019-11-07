# Encapsulates the Tetris Core Engine to provide a better
# view for machine learning. This library is intended to be
# written in a procedural/functional fashion with lots of inlines
# for optimization chance and with scriptablity in mind.

add_library(tetris_api STATIC
tetris_api.h
tetris_api_renderer.h
tetris_api_renderer.cpp
tetris_core.h
tetris_core.cpp
tetris_extended_engine.h
)

create_target_directory_groups(tetris_api)