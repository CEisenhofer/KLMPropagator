cmake_minimum_required(VERSION 3.16.3)
project (KLM_UP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Z3 4.12.0 REQUIRED)
# find_package(ANTLR REQUIRED)

if ((NOT Z3_INCLUDE_DIR) OR (NOT Z3_LIBRARIES))
    message(FATAL_ERROR "Did not find Z3. Pass path directly to CMake via -D")
    fail()
else()
    include_directories(${Z3_INCLUDE_DIR})
endif()

if ((NOT ANTLR_INCLUDE_DIR) OR (NOT ANTLR_LIBRARIES))
    message(FATAL_ERROR "Did not find ANTLR. Pass path directly to CMake via -D")
    fail()
else()
    include_directories(${ANTLR_INCLUDE_DIR})
endif()

if ((NOT OGDF_INCLUDE_DIR) OR (NOT OGDF_INCLUDE2_DIR) OR (NOT OGDF_LIBRARIES) OR (NOT COIN_LIBRARIES))
    message(FATAL_ERROR "Did not find ogdf. Pass path directly to CMake via -D")
    fail()
else()
	include_directories(${OGDF_INCLUDE_DIR})
	include_directories(${OGDF_INCLUDE2_DIR})
endif()

add_subdirectory(src)
