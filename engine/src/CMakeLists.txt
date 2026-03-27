file(GLOB SRC
    cpp/*.cpp
    cpp/type/*.cpp
    cpp/objects/*.cpp
)

# move obj files where the build can freely access
file(COPY ${CMAKE_SOURCE_DIR}/engine/objects DESTINATION ${CMAKE_BINARY_DIR})

add_executable(endgen ${SRC})

# manually feed the paths
set(SDL2_TTF_LIBRARIES /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so)
set(SDL2_TTF_INCLUDE_DIRS /usr/include/SDL2)

target_include_directories(endgen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hpp/type
    ${CMAKE_CURRENT_SOURCE_DIR}/hpp/objects
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries(endgen PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)