# CMakeList.txt : CMake project for Cmake_Zenix, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
project (Cmake_Zenix)


#=====Source code inclusion=====
function(create_target_directory_groups target_name)
    source_group("Other Files" REGULAR_EXPRESSION ".")

    get_target_property(target_sources "${target_name}" SOURCES)

    foreach(file_name IN LISTS target_sources)
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()



#======= Compile flags =======
#C++ standard
set(CMAKE_CXX_STANDARD 17) #C++ std-17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
add_compile_options(-DDEBUG_DEFINED)
message(STATUS "DEBUG_DEFINED is defined")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
add_compile_options(-DRELEASE_DEFINED)
message(STATUS "RELEASE_DEFINED is defined")
endif()

if(WIN32)
add_compile_options(-DWINDOWS_DEFINED)
message(STATUS "WINDOWS_DEFINED is defined")
elseif(UNIX)
add_compile_options(-DUNIX_DEFINED)
message(STATUC "UNIX_DEFINED is defined")
endif()

#processor count
include(ProcessorCount)
ProcessorCount(N_PROCESSOR)
if(NOT N_PROCESSOR EQUAL 0) #ProcessorCount returns 0 if it can't determine core count'
#add_compile_definitions(CMAKE_N_PROCESSOR_DETECTED=${N_PROCESSOR})
add_compile_options(-DCMAKE_N_PROCESSOR_DETECTED=${N_PROCESSOR})
message(STATUS "Cmake detected # of processor is ${N_PROCESSOR}")
endif()

#===== Packaging/libraries =====
find_package (OpenCL REQUIRED)
find_package (Threads REQUIRED) #for multithreading/pthread
if(WIN32)
option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
endif()

#===== Source code =====
add_subdirectory(third_party)
add_subdirectory(src)

# Add source to this project's executable.
add_executable (Cmake_Zenix "Cmake_Zenix.cpp" "Cmake_Zenix.h")

#=====Linker=====
find_library(M_LIB m)
target_link_libraries (Cmake_Zenix OpenCL::OpenCL common)
if (UNIX)
	target_link_libraries(Cmake_Zenix m)
endif()


#Tests & install targets
#install(TARGETS )


#copy_cl_source()