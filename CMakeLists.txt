cmake_minimum_required(VERSION 3.13)

project(reflection)

add_library(reflection INTERFACE)
add_executable(reflection_test)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

target_compile_features(reflection INTERFACE cxx_std_17)
target_compile_features(reflection_test PUBLIC cxx_std_17)

#Set(CMAKE_CXX_FLAGS "-Wall" "-Wextra")
set(CMAKE_CXX_FLAGS -fmax-errors=1)

install(TARGETS reflection EXPORT reflection DESTINATION lib/reflection)
install(EXPORT reflection DESTINATION lib/reflection)
install(FILES reflection-config.cmake DESTINATION lib/reflection)

find_Package(concept REQUIRED)
find_Package(meta REQUIRED)