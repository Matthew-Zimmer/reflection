cmake_minimum_required(VERSION 3.13)

project(reflection)

add_library(reflection INTERFACE)

add_subdirectory(include)
add_subdirectory(src)

target_compile_features(reflection INTERFACE cxx_std_17)

#Set(CMAKE_CXX_FLAGS "-Wall" "-Wextra")

install(TARGETS reflection EXPORT reflection DESTINATION lib/reflection)
install(EXPORT reflection DESTINATION lib/reflection)
install(FILES reflection-config.cmake DESTINATION lib/reflection)

find_Package(concept REQUIRED)
find_Package(meta REQUIRED)
target_link_libraries(reflection INTERFACE concept meta)