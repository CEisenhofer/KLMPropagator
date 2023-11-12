cmake_minimum_required(VERSION 3.16.3)

add_executable(KLM
        KLMPropagator.cpp
        LanguageBaseVisitor.cpp
        LanguageLexer.cpp
        LanguageParser.cpp
        LanguageVisitor.cpp
        node.cpp
        Program.cpp
        random_formula.cpp)

#target_link_libraries(KLM PRIVATE dep)

if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

#include_directories("./dep")
target_link_libraries(KLM PRIVATE ${Z3_LIBRARIES})
target_link_libraries(KLM PRIVATE ${ANTLR_LIBRARIES})
target_link_libraries(KLM PRIVATE ${ANTLR_LIBRARIES})
target_link_libraries(KLM PRIVATE  ${OGDF_LIBRARIES})
target_link_libraries(KLM PRIVATE  ${COIN_LIBRARIES})
