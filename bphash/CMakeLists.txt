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

# Where to install the library
install(TARGETS bphash
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Install the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")

