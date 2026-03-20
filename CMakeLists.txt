cmake_minimum_required(VERSION 3.28)

project(cpp26_playground
        VERSION 0.1.0
        LANGUAGES CXX
)

include(cmake/AddTestTarget.cmake)
include(cmake/CompilerFlags.cmake)

add_subdirectory(core)
add_subdirectory(tests)
