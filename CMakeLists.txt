cmake_minimum_required(VERSION 2.8)
set(EXECUTABLE_NAME "waterfall")
project(${EXECUTABLE_NAME})

set(CMAKE_CXX_COMPILER "gcc-4.9")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror")

find_package(OpenGL REQUIRED)

include_directories(libs src)

set(SOURCE_FILES src/main.cpp src/model.cpp src/particlesystem.cpp src/shaders.cpp src/texture.cpp src/utils.cpp src/waterfallprogram.cpp)
set(LIBS AntTweakBar glfw3 GLEW FreeImage)
set(LIBS ${LIBS} Xext X11 Xrandr Xi Xxf86vm pthread)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARY} ${LIBS})