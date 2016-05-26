# This is the main library
add_executable(bphash_test test.cpp)

# Include the main source directory (my parent) as an include directory
target_include_directories(bphash_test PRIVATE ${CMAKE_SOURCE_DIR})

# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-w3")

    #   383  : value copied to temporary, reference to temporary used (generally ok in c++)
    #   981  : operands are evaluated in unspecified order (generally ok in c++)
    #  3291  : invalid narrowing conversion (done on purpose in testing sources)
    #  1418  : external function definition with no prior declaration (generally ok)
    #  2259  : non-pointer conversion from "double" to "float" may lose significant bits
    #   280  : selector expression is constant (ok)
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd383")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd981")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd3291")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd1418")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd2259")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd280")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-Wall;-Wextra;-pedantic")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-Wno-narrowing")  # done on purpose in the testing sources
endif()


target_compile_options(bphash_test PRIVATE ${BPHASH_TEST_CXX_FLAGS})
target_link_libraries(bphash_test PRIVATE bphash)
