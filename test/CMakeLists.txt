find_package(GTest REQUIRED)

add_executable(Tests
        HeapTests.cpp
        ElementTests.cpp
)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests
        PRIVATE
        StructuresLib
        GTest::gtest
        GTest::gtest_main
        ${CURSES_LIBRARIES}
)

add_test(
        NAME Tests
        COMMAND Tests
)