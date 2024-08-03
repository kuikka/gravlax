find_package(fmt CONFIG REQUIRED)

add_library(libgravlax STATIC src/main.cpp src/scanner.cpp src/token.cpp)
target_link_libraries(libgravlax PRIVATE fmt::fmt)
target_include_directories(libgravlax PUBLIC include)
target_compile_options(libgravlax PUBLIC -fprofile-arcs -ftest-coverage)
target_link_options(libgravlax PUBLIC -fprofile-arcs -ftest-coverage)

add_subdirectory(tests)