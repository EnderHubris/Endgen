file(GLOB SRC cpp/*.cpp)
file(GLOB HPP hpp/*.hpp)

file(GLOB TYPE_SRC cpp/type/*.cpp)
file(GLOB TYPE_HPP hpp/type/*.hpp)

add_executable(endgen ${SRC} ${HPP} ${TYPE_SRC} ${TYPE_HPP})
target_include_directories(endgen PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(endgen PRIVATE ${SDL2_LIBRARIES})