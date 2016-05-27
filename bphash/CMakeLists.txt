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
    list(APPEND BPHASH_CXX_FLAGS "-wd383")
    list(APPEND BPHASH_CXX_FLAGS "-wd981")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPHASH_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_CXX_FLAGS "-Wall;-Wextra;-pedantic")
endif()


target_compile_options(bphash PRIVATE ${BPHASH_CXX_FLAGS})


# Where to install the library
install(TARGETS bphash
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Install the headers headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")
