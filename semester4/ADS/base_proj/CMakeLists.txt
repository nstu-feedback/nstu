cmake_minimum_required(VERSION 3.10)
project(test)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXEC test)
set(SRC test.cpp)

add_executable(${EXEC} ${SRC})
target_link_libraries(${EXEC} ${CONAN_LIBS})

