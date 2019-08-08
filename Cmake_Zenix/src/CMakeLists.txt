message(STATUS "Accessing src folder")
include_directories(.)

add_subdirectory(initial_tests)
add_subdirectory(common)
add_subdirectory(tetris)
add_subdirectory(renderer)
add_subdirectory(video_core)