# Additional compilation flags
set(BPHASH_EXTRA_CXX_FLAGS "" CACHE BOOL "Additional compilations flags for the BPHash library (semicolon-separated)")
set(BPHASH_BUILD_SHARED FALSE CACHE BOOL "Build a shared library, rather than a static library")

# Set up the library type. This variable is passed to add_library
set(BPHASH_BUILD_TYPE "STATIC")
if(BPHASH_BUILD_SHARED)
  set(BPHASH_BUILD_TYPE "SHARED")
endif()


# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPHASH_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_CXX_FLAGS "-w3")

    #   383  : value copied to temporary, reference to temporary used
    #   981  : operands are evaluated in unspecified order
    #  1418  : external function definition with no prior declaration
    list(APPEND BPHASH_CXX_FLAGS "-wd383")
    list(APPEND BPHASH_CXX_FLAGS "-wd981")
    list(APPEND BPHASH_CXX_FLAGS "-wd1418")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPHASH_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_CXX_FLAGS "-Wall;-Wextra;-pedantic")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    list(APPEND BPHASH_CXX_FLAGS "-std=c++11")
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
  

# This is the main library
add_library(bphash ${BPHASH_BUILD_TYPE}
                   Hasher.cpp
                   Hash.cpp
                   MurmurHash3_128_x64.cpp
                   MurmurHash3_64_x64.cpp
                   MurmurHash3_32_x64.cpp
                   MurmurHash3_32_x32.cpp
           )

# Include the main source directory (my parent) as an include directory
target_include_directories(bphash PRIVATE ${CMAKE_SOURCE_DIR})

# Add the extra flags to the actual build flags
list(APPEND BPHASH_CXX_FLAGS ${BPHASH_EXTRA_CXX_FLAGS})

target_compile_options(bphash PRIVATE ${BPHASH_CXX_FLAGS})


# Where to install the library
install(TARGETS bphash
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Install the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")

