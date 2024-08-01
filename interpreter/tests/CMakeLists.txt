enable_testing()

include(CTest)
include(GoogleTest)

find_package(GTest CONFIG REQUIRED)

function(add_test_executable name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE fmt::fmt)
    target_link_libraries(${name} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

    target_link_libraries(${name} PRIVATE liblox)

    target_include_directories(${name} PRIVATE ../include)
    target_include_directories(${name} PRIVATE include)

    gtest_discover_tests(${name})

    # add_test(NAME ${name} COMMAND ${name})
endfunction()

add_test_executable(test_scanner)