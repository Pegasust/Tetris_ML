add_executable(renderer
renderer.cpp
renderer_specs.h
game_renderer.h
game_renderer.cpp
)

create_target_directory_groups(renderer)

target_link_libraries(renderer common tetris)