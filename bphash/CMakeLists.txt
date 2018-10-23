# This is the main library
add_library(bphash Hasher.cpp
                   Hash.cpp
                   MurmurHash3_128_x64.cpp
                   MurmurHash3_64_x64.cpp
                   MurmurHash3_32_x64.cpp
                   MurmurHash3_32_x32.cpp
           )

# Include the main source directory (my parent) as an include directory
target_include_directories(bphash 
                           PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
                                  $<INSTALL_INTERFACE:include>
                          )

# Where to install the library
install(TARGETS bphash
        EXPORT bphashTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

