find_package(fmt CONFIG REQUIRED)

set(GRAVLAX_GENERATED_INCLUDE_PATH ${CMAKE_BINARY_DIR}/include/gravlax/generated)

add_library(libgravlax STATIC src/main.cpp src/scanner.cpp src/token.cpp src/ast_printer.cpp)
target_link_libraries(libgravlax PRIVATE fmt::fmt)
target_include_directories(libgravlax PUBLIC include)
target_include_directories(libgravlax PUBLIC ${CMAKE_BINARY_DIR}/include)

# target_compile_options(libgravlax PUBLIC -fprofile-arcs -ftest-coverage)
# target_link_options(libgravlax PUBLIC -fprofile-arcs -ftest-coverage)

file(MAKE_DIRECTORY ${GRAVLAX_GENERATED_INCLUDE_PATH})
add_custom_target(generate_ast COMMAND ast_generator ${GRAVLAX_GENERATED_INCLUDE_PATH})
add_dependencies(libgravlax generate_ast)

add_subdirectory(tests)
add_subdirectory(tools)