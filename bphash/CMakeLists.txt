# Additional compilation flags
set(BPHASH_EXTRA_CXX_FLAGS "" CACHE BOOL "Additional compilations flags for the BPHash library (semicolon-separated)")

# This is the main library
add_library(bphash STATIC Hasher.cpp
                          Hash.cpp
                          MurmurHash3.cpp)

# Include the main source directory (my parent) as an include directory
target_include_directories(bphash PRIVATE ${CMAKE_SOURCE_DIR})

# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPHASH_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_CXX_FLAGS "-w3")

    #   383  : value copied to temporary, reference to temporary used (generally ok in c++)
    #   981  : operands are evaluated in unspecified order (generally ok in c++)
    #   444  : destructor for base class .. not virtual (even warns for classes without virtual functions)
    #  1418  : external function definition with no prior declaration
    list(APPEND BPHASH_CXX_FLAGS "-wd383")
    list(APPEND BPHASH_CXX_FLAGS "-wd981")
    list(APPEND BPHASH_CXX_FLAGS "-wd444")
    list(APPEND BPHASH_CXX_FLAGS "-wd1418")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPHASH_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_CXX_FLAGS "-Wall;-Wextra;-pedantic")
endif()

list(APPEND BPHASH_CXX_FLAGS ${BPHASH_EXTRA_CXX_FLAGS})

target_compile_options(bphash PRIVATE ${BPHASH_CXX_FLAGS})


# Where to install the library
install(TARGETS bphash
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Install the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")
