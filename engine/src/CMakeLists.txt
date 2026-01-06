file(GLOB SRC cpp/*.cpp)
file(GLOB HPP hpp/*.hpp)

add_executable(endgen ${SRC} ${HPP})
target_include_directories(endgen PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(endgen PRIVATE ${SDL2_LIBRARIES})