add_library(StructuresLib STATIC
        Binary.cpp
        Binary.hpp
        Collection.hpp
        Element.hpp
        Collection.cpp
        Utils.cpp
        Utils.hpp
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