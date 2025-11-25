enable_testing()

# Add test executables
add_executable(test_array suites/api/test_array.c suites/api/util.h)
target_link_libraries(test_array jansson)
add_test(NAME test_array COMMAND test_array)

add_executable(test_chaos suites/api/test_chaos.c suites/api/util.h)
target_link_libraries(test_chaos jansson)
add_test(NAME test_chaos COMMAND test_chaos)

add_executable(test_copy suites/api/test_copy.c suites/api/util.h)
target_link_libraries(test_copy jansson)
add_test(NAME test_copy COMMAND test_copy)

add_executable(test_dump suites/api/test_dump.c suites/api/util.h)
target_link_libraries(test_dump jansson)
add_test(NAME test_dump COMMAND test_dump)

add_executable(test_dump_callback suites/api/test_dump_callback.c suites/api/util.h)
target_link_libraries(test_dump_callback jansson)
add_test(NAME test_dump_callback COMMAND test_dump_callback)

add_executable(test_equal suites/api/test_equal.c suites/api/util.h)
target_link_libraries(test_equal jansson)
add_test(NAME test_equal COMMAND test_equal)

add_executable(test_load suites/api/test_load.c suites/api/util.h)
target_link_libraries(test_load jansson)
add_test(NAME test_load COMMAND test_load)

add_executable(test_loadb suites/api/test_loadb.c suites/api/util.h)
target_link_libraries(test_loadb jansson)
add_test(NAME test_loadb COMMAND test_loadb)

add_executable(test_load_callback suites/api/test_load_callback.c suites/api/util.h)
target_link_libraries(test_load_callback jansson)
add_test(NAME test_load_callback COMMAND test_load_callback)

add_executable(test_number suites/api/test_number.c suites/api/util.h)
target_link_libraries(test_number jansson)
add_test(NAME test_number COMMAND test_number)

add_executable(test_object suites/api/test_object.c suites/api/util.h)
target_link_libraries(test_object jansson)
add_test(NAME test_object COMMAND test_object)

add_executable(test_pack suites/api/test_pack.c suites/api/util.h)
target_link_libraries(test_pack jansson)
add_test(NAME test_pack COMMAND test_pack)

add_executable(test_simple suites/api/test_simple.c suites/api/util.h)
target_link_libraries(test_simple jansson)
add_test(NAME test_simple COMMAND test_simple)

add_executable(test_unpack suites/api/test_unpack.c suites/api/util.h)
target_link_libraries(test_unpack jansson)
add_test(NAME test_unpack COMMAND test_unpack)
