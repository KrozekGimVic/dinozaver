project(dinosaur)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -Wall")

add_executable(dinosaur src/dinosaur.cpp)
target_link_libraries(dinosaur sfml-graphics sfml-window sfml-system)
