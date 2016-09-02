cmake_minimum_required(VERSION 3.0.2)
project(BPHash CXX)

# Enable testing?
enable_testing()

# The main subdirectory with the bphash library
add_subdirectory(bphash)

# Directory with some tests
add_subdirectory(test)


# Create and install the config file
configure_file("bphashConfig.cmake.in" "bphashConfig.cmake" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/bphashConfig.cmake DESTINATION lib/cmake/bphash) 
