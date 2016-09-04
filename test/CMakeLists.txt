# Testing of BPHash

# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-w3")

    #   383  : value copied to temporary, reference to temporary used
    #   981  : operands are evaluated in unspecified order
    #  1418  : external function definition with no prior declaration
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd383")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd981")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-wd1418")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-Wall;-Wextra;-pedantic")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPHASH_TEST_CXX_FLAGS "-Wall;-Wextra;-pedantic")

    # for heavy-duty testing
    #list(APPEND BPHASH_TEST_CXX_FLAGS "-Weverything")
    #list(APPEND BPHASH_TEST_CXX_FLAGS "-Wno-c++98-compat")
    #list(APPEND BPHASH_TEST_CXX_FLAGS "-Wno-c++98-compat-pedantic")
    #list(APPEND BPHASH_TEST_CXX_FLAGS "-Wno-implicit-fallthrough")
    #list(APPEND BPHASH_TEST_CXX_FLAGS "-Wno-padded")
    #list(APPEND BPHASH_TEST_CXX_FLAGS "-Wno-weak-vtables")
    #list(APPEND BPHASH_TEST_CXX_FLAGS "-Wno-covered-switch-default")
endif()


#add_executable(test_unique test_integers.cpp
#                           test_floating.cpp
#                           test_string.cpp
#                           test_tuple.cpp
#                           test_unique.cpp
#              )
#
#target_include_directories(test_unique PRIVATE ${CMAKE_SOURCE_DIR})
#target_compile_options(test_unique PRIVATE ${BPHASH_TEST_CXX_FLAGS} ${BPHASH_EXTRA_CXX_FLAGS})
#target_link_libraries(test_unique PRIVATE bphash)


add_executable(test_reference test_reference.cpp MurmurHash3_reference.cpp)
target_include_directories(test_reference PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(test_reference PRIVATE ${BPHASH_TEST_CXX_FLAGS} ${BPHASH_EXTRA_CXX_FLAGS})
target_link_libraries(test_reference PRIVATE bphash)
         

add_executable(test_benchmark test_benchmark.cpp MurmurHash3_reference.cpp)
target_include_directories(test_benchmark PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(test_benchmark PRIVATE ${BPHASH_TEST_CXX_FLAGS} ${BPHASH_EXTRA_CXX_FLAGS})
target_link_libraries(test_benchmark PRIVATE bphash)

add_executable(test_detect test_detect.cpp)
target_include_directories(test_detect PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(test_detect PRIVATE ${BPHASH_TEST_CXX_FLAGS} ${BPHASH_EXTRA_CXX_FLAGS})
target_link_libraries(test_detect PRIVATE bphash)

add_executable(test_stl test_stl.cpp)
target_include_directories(test_stl PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(test_stl PRIVATE ${BPHASH_TEST_CXX_FLAGS} ${BPHASH_EXTRA_CXX_FLAGS})
target_link_libraries(test_stl PRIVATE bphash)

add_test(NAME run_test_reference COMMAND test_reference)
add_test(NAME run_test_benchmark COMMAND test_benchmark 1048576)
add_test(NAME run_test_detect COMMAND test_detect)
add_test(NAME run_test_stl COMMAND test_stl)
