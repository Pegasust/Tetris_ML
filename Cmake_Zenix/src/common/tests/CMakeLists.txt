add_executable(string_manip_test
string_manip_test.cpp
)
add_executable(preprocessor_path_join_test
preprocessor_path_join_test.cpp
)

add_executable(zmath_test
zmath_test.cpp
)

add_executable(bi_map_test
bi_map_test.cpp
)

add_executable(sandbox
sandbox.cpp
)

create_target_directory_groups(string_manip_test)
create_target_directory_groups(preprocessor_path_join_test)
create_target_directory_groups(zmath_test)
create_target_directory_groups(bi_map_test)
create_target_directory_groups(sandbox)

target_link_libraries(string_manip_test common test_utils)
target_link_libraries(preprocessor_path_join_test common)
target_link_libraries(zmath_test common test_utils)
target_link_libraries(bi_map_test common test_utils)
target_link_libraries(sandbox common test_utils)