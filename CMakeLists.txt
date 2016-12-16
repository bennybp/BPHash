cmake_minimum_required(VERSION 3.1.3)
project(BPHash CXX)

# Enable testing?
enable_testing()

# Options to set
# By default, use C++14 compilation
set(CMAKE_CXX_EXTENSIONS False CACHE BOOL "Enable/Disable compiler-specific C++ extensions")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "Which C++ standard to use")
set(BPHASH_EXTRA_CXX_FLAGS "" CACHE BOOL "Additional compilations flags for the BPHash library (semicolon-separated)")
set(BPHASH_BUILD_SHARED FALSE CACHE BOOL "Build a shared library, rather than a static library")

# CMake doesn't support Intel CXX standard until cmake 3.6
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD}")
  endif()
endif()


# Set up the library type. This variable is passed to add_library
set(BPHASH_BUILD_TYPE "STATIC")
if(BPHASH_BUILD_SHARED)
  set(BPHASH_BUILD_TYPE "SHARED")
endif()


# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPHASH_CXX_FLAGS "-w3")

    #   383  : value copied to temporary, reference to temporary used
    #   981  : operands are evaluated in unspecified order
    #  1418  : external function definition with no prior declaration
    list(APPEND BPHASH_CXX_FLAGS "-wd383")
    list(APPEND BPHASH_CXX_FLAGS "-wd981")
    list(APPEND BPHASH_CXX_FLAGS "-wd1418")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPHASH_CXX_FLAGS "-Wall;-Wextra;-pedantic")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    list(APPEND BPHASH_CXX_FLAGS "-Wall;-Wextra;-pedantic")

    # for heavy-duty testing
    list(APPEND BPHASH_CXX_FLAGS "-Weverything")
    list(APPEND BPHASH_CXX_FLAGS "-Wno-c++98-compat")
    list(APPEND BPHASH_CXX_FLAGS "-Wno-c++98-compat-pedantic")
    list(APPEND BPHASH_CXX_FLAGS "-Wno-implicit-fallthrough")
    list(APPEND BPHASH_CXX_FLAGS "-Wno-padded")
    list(APPEND BPHASH_CXX_FLAGS "-Wno-weak-vtables")
    list(APPEND BPHASH_CXX_FLAGS "-Wno-covered-switch-default")
endif()

# The main subdirectory with the bphash library
add_subdirectory(bphash)

# Directory with some tests
add_subdirectory(test)


# Create and install the config file
configure_file("bphashConfig.cmake.in" "bphashConfig.cmake" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/bphashConfig.cmake DESTINATION lib/cmake/bphash) 
