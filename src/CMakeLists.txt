add_library(StructuresLib STATIC
        Binary.cpp
        Binary.hpp
        Heap.hpp
        Element.hpp
        Heap.cpp
        Utils.cpp
        Utils.hpp
        Binomial.cpp
        Binomial.hpp
        Pairing.cpp
        Pairing.hpp
        TwoThreeHeap.cpp
        TwoThreeHeap.hpp
        Fibonacci.cpp
        Fibonacci.hpp
)
target_include_directories(StructuresLib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Structures
        main.cpp
        Analysis.cpp
        Analysis.hpp
)
target_include_directories(Structures
        PRIVATE
        StructuresLib
)
target_link_libraries(Structures
        PRIVATE
        StructuresLib
        ${CURSES_LIBRARIES}
)