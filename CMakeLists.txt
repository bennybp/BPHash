cmake_minimum_required(VERSION 3.1.3)
project(BPHash CXX)
set(bphash_VERSION 1.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS False)

# For standard install paths (CMAKE_INSTALL_*DIR)
include(GNUInstallDirs)

# CMake doesn't support Intel CXX standard until cmake 3.6
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      add_compile_options(-std=c++${CMAKE_CXX_STANDARD})
  endif()
endif()

# Enable testing?
enable_testing()

# BPHash-specific options
option(BPHASH_USE_TYPEID "Hash the typeid name of the object along with the data" OFF)

if(BPHASH_USE_TYPEID)
    add_compile_options(-DBPHASH_USE_TYPEID)
endif()

# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    add_compile_options(-w3)

    #   383  : value copied to temporary, reference to temporary used
    #   981  : operands are evaluated in unspecified order
    #  1418  : external function definition with no prior declaration
    add_compile_options(-wd383)
    add_compile_options(-wd981)
    add_compile_options(-wd1418)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    add_compile_options(-Wall;-Wextra;-pedantic)
    add_compile_options(-Wno-implicit-fallthrough)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_compile_options(-Wall;-Wextra;-pedantic)

    # comment out for heavy-duty testing
    add_compile_options(-Weverything)
    add_compile_options(-Wno-c++98-compat)
    add_compile_options(-Wno-c++98-compat-pedantic)
    add_compile_options(-Wno-padded)
    add_compile_options(-Wno-weak-vtables)
    add_compile_options(-Wno-covered-switch-default)
    add_compile_options(-Wno-implicit-fallthrough)
endif()

# The main subdirectory with the bphash library
add_subdirectory(bphash)

# Directory with some tests
add_subdirectory(test)

####################################
# Exporting the CMake configuration
####################################
include(CMakePackageConfigHelpers)

configure_package_config_file(bphashConfig.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/bphashConfig.cmake
                              INSTALL_DESTINATION share/cmake/bphash
)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/bphashConfigVersion.cmake
                                 VERSION ${bphash_VERSION}
                                 COMPATIBILITY AnyNewerVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bphashConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/bphashConfigVersion.cmake
        DESTINATION share/cmake/bphash
)

install(EXPORT bphashTargets
        NAMESPACE bphash::
        DESTINATION share/cmake/bphash
)
