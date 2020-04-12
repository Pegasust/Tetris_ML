message(STATUS "Accessing neural_network_core")

# This folder is for core structures of a neural network
# which uses edges & vertices in its implementation.

add_library(neural_network_core STATIC
neural_graph.cpp
neural_network_core.hpp
neural_graph.ipp
neural_network_core.cpp
)

create_target_directory_groups(neural_network_core)

target_link_libraries(neural_network_core common)

add_subdirectory(tests)