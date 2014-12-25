cmake_minimum_required(VERSION 2.8)

if(COMMAND cmake_policy)  
	cmake_policy(SET CMP0015 NEW)  
endif(COMMAND cmake_policy)  


set(CMAKE_ALLOW_LOSSE_LOOP_CONSTRUCTS ON)

message(STATUS "#####################################################################")
message(STATUS "# DuiLib Build System")
message(STATUS "#####################################################################")
message(STATUS "Configuring build system...")
message(STATUS "CMake version: ${CMAKE_VERSION}")

#option(STATIC "Static build" FALSE)
#option(UNICODE "Using Unicode charset" FALSE)

PROJECT (DuiLib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Control)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Layout)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Utils)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


file(GLOB_RECURSE HEADER_FILES *.h *hpp *.inl)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.cc *.c)

set(ALL_FILES ${HEADER_FILES} ${SOURCE_FILES})

macro(GROUP_FILES src_files)
        set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(group_files ${${src_files}})
            string(REGEX REPLACE ${cur_dir}/\(.*\) \\1 sgbd_fpath ${group_files})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "\\" "/" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "")
            endif(sgbd_nogroup)
			
            source_group(${sgbd_group_name} FILES ${group_files})
        endforeach(group_files)
endmacro(GROUP_FILES)

GROUP_FILES(ALL_FILES)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(duilib SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_definitions(-DUILIB_EXPORTS)
target_link_libraries(duilib riched20 comctl32)
set_target_properties(duilib PROPERTIES OUTPUT_NAME "duilib")
add_custom_command(TARGET duilib POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${PROJECT_BINARY_DIR}/lib/duilib.dll ${PROJECT_SOURCE_DIR}/bin/duilib.dll)