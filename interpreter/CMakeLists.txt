find_package(fmt CONFIG REQUIRED)

add_library(liblox STATIC src/main.cpp src/scanner.cpp)
target_link_libraries(liblox PRIVATE fmt::fmt)
target_include_directories(liblox PUBLIC include)

add_subdirectory(tests)