add_executable(ast_generator ast_generator.cpp)
target_link_libraries(ast_generator PRIVATE fmt::fmt)
target_include_directories(ast_generator PRIVATE ../include)