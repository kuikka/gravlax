find_package(fmt CONFIG REQUIRED)

add_library(libgravlax STATIC src/main.cpp src/scanner.cpp)
target_link_libraries(libgravlax PRIVATE fmt::fmt)
target_include_directories(libgravlax PUBLIC include)

add_subdirectory(tests)