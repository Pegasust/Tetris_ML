message(STATUS "Accessing neat folder")
add_library(neat STATIC
neat.cpp
neat.hpp
neat.ipp
genotype.cpp
genotype.hpp
genotype.ipp
innovation.cpp
innovation.hpp
phenotype.cpp
phenotype.hpp
organism.cpp
organism.hpp
neural_info.hpp
innovation_database.cpp
innovation_database.hpp
population.cpp
population.hpp
population.ipp
)


create_target_directory_groups(neat)
target_link_libraries(neat neural_network_core)

add_subdirectory(tests)