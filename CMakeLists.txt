cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(shabalaga)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")

# Our Project
include(src/src.cmake)
add_executable(${PROJECT_NAME} ${game_source})
if (UNIX)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/linux/libraylib.a)
endif()

if (WINDOWS)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/windows/libraylib.a)
endif()
