# Basically stores base codes that is commonly used in the project
# All functions should be cross platform too!
message(STATUS "Accessing common")

add_subdirectory(data_structs)
add_library(common STATIC
zmath.h
zmath.cpp
rng.h
rng.cpp
game_clock.h
game_clock.cpp
keyboard_input.h
keyboard_input.cpp
keyboard_input.ipp
file_managing.hpp
assertion.hpp
string_manip.cpp
string_manip.h
common.hpp
)

add_executable(keyboard_test
keyboard_test.cpp
)

create_target_directory_groups(common)
create_target_directory_groups(keyboard_test)

target_link_libraries(common ryu)

target_link_libraries(keyboard_test common)
#if (UNIX)
#	find_library(M_LIB m)
#	target_link_libraries(Cmake_Zenix m)
#endif()

add_subdirectory(tests)