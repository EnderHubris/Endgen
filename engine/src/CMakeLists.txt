file(GLOB SRC cpp/*.cpp)

add_executable(endgen ${SRC} ${TYPE_SRC})
target_include_directories(endgen PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(endgen PRIVATE ${SDL2_LIBRARIES})