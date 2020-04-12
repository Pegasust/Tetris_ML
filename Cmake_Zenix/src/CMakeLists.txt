message(STATUS "Accessing src folder")
include_directories(.)

add_subdirectory(test_utils)

add_subdirectory(initial_tests)
add_subdirectory(common)
add_subdirectory(verbosity)
add_subdirectory(tetris)
add_subdirectory(tetris_api)
add_subdirectory(renderer)
add_subdirectory(replay)
add_subdirectory(video_core)
add_subdirectory(tetris_game)

add_subdirectory(neural_network_core)
add_subdirectory(tetris_ml)
add_subdirectory(NEAT)