cmake_minimum_required(VERSION 3.16.3)

add_executable(KLMPropagator
        language.yy.cpp
        language.tab.cpp
        KLMPropagator.cpp
        node.cpp
        Program.cpp
        random_formula.cpp)

#target_link_libraries(KLMPropagator PRIVATE dep)

if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

#include_directories("./dep")
target_link_libraries(KLMPropagator PRIVATE ${Z3_LIBRARIES})
