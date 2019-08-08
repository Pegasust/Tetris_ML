#Basically stores base codes that is commonly used in the project
#It should be cross platform too!
message(STATUS "Accessing common")
add_library(common STATIC
zmath.h
zmath.cpp
rng.h
rng.cpp
game_clock.h
game_clock.cpp
)

create_target_directory_groups(common)
find_library(M_LIB m)

if (UNIX)
	target_link_libraries(Cmake_Zenix m)
endif()