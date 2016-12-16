# Testing of BPHash

#add_executable(test_unique test_integers.cpp
#                           test_floating.cpp
#                           test_string.cpp
#                           test_tuple.cpp
#                           test_unique.cpp
#              )
#
#target_include_directories(test_unique PRIVATE ${CMAKE_SOURCE_DIR})
#target_link_libraries(test_unique PRIVATE bphash)


add_executable(test_reference test_reference.cpp MurmurHash3_reference.cpp)
target_include_directories(test_reference PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_reference PRIVATE bphash)
         

add_executable(test_benchmark test_benchmark.cpp MurmurHash3_reference.cpp)
target_include_directories(test_benchmark PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_benchmark PRIVATE bphash)

add_executable(test_detect test_detect.cpp)
target_include_directories(test_detect PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_detect PRIVATE bphash)

add_executable(test_stl test_stl.cpp)
target_include_directories(test_stl PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_stl PRIVATE bphash)

add_test(NAME run_test_reference COMMAND test_reference)
add_test(NAME run_test_benchmark COMMAND test_benchmark 1048576)
add_test(NAME run_test_detect COMMAND test_detect)
add_test(NAME run_test_stl COMMAND test_stl)
