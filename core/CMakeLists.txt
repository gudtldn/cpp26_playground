add_library(core_lib INTERFACE)

target_include_directories(core_lib INTERFACE include)
target_link_libraries(core_lib INTERFACE DefaultOptions)
