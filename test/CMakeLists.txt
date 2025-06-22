find_package(GTest REQUIRED)

add_executable(Tests
        BinaryTests.cpp
        ElementTests.cpp
        BinomialTests.cpp
        PairingTests.cpp
        TwoThreeTests.cpp
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