# Contains the types of renderer for Tetris Module
add_library(renderer STATIC
renderer_specs.h
game_renderer.h
game_renderer.cpp)
create_target_directory_groups(renderer)


add_executable(renderer_exe
renderer.cpp
)
create_target_directory_groups(renderer_exe)

target_link_libraries(renderer common tetris)
target_link_libraries(renderer_exe renderer)