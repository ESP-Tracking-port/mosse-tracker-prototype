file(GLOB_RECURSE MOSSE_SOURCES "./*.cpp")
add_library(lib_mosse_tracking ${MOSSE_SOURCES})
target_link_libraries(lib_mosse_tracking PRIVATE -g)
