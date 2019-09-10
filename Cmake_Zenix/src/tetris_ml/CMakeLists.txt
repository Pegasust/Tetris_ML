add_executable(tetris_game_test
tetris_game_test.cpp
)
create_target_directory_groups(tetris_game_test)

target_link_libraries(tetris_game_test tetris_game)
add_executable(tetris_ml
tetris_ml.cpp
)

create_target_directory_groups(tetris_ml)
target_link_libraries(tetris_ml common renderer tetris video_core)
