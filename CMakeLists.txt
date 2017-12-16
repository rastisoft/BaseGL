cmake_minimum_required (VERSION 3.0)

project(basegl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -fPIC")
set(CMAKE_CXX_COMPILER g++)


find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OpenGL_INCLUDE_DIRS})
    link_directories(${OpenGL_LIBRARY_DIRS})
    add_definitions(${OpenGL_DEFINITIONS})
else(OPENGL_FOUND)
    message(FATAL_ERROR "OPENGL not found!")
endif(OPENGL_FOUND)


find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_directories(${GLEW_LIBRARY_DIRS})
    add_definitions(${GLEW_DEFINITIONS})
else(GLEW_FOUND)
    message(FATAL_ERROR " GLEW not found!")
endif(GLEW_FOUND)

#Finding GLFW--------------------------------
find_path(GLFW_INCLUDE_DIR  NAMES  GLFW/glfw3.h
PATHS
    /usr/include
    /usr/local/include
    NO_DEFAULT_PATH
)

find_library(GLFW_LIBRARY NAMES glfw3 glfw
PATHS
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /usr/lib/x86_64-linux-gnu
    NO_DEFAULT_PATH
)

if(GLFW_LIBRARY)
    set(GLFW_LIBRARIES ${GLFW_LIBRARY})
else(GLFW_LIBRARY)
    message(FATAL_ERROR " GLEW not found!")
endif(GLFW_LIBRARY)
#------------------------------------------

add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")
add_subdirectory(${CMAKE_SOURCE_DIR}/include/RS/Graphics/BaseGL/3rdparty/glm)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE SRC_FILES 
                ${CMAKE_SOURCE_DIR}/src/RS/*.cpp 
                ${CMAKE_SOURCE_DIR}/include/RS/*.c
    )

set(LIBS ${LIBS} ${OPENGL_LIBRARY} ${GLEW_LIBRARY} ${GLFW_LIBRARIES} pthread stdc++ m GLU GL dl Xinerama Xrandr Xi Xcursor X11 Xxf86vm Xext rt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/examples/bin)
add_library(baseGL STATIC ${SRC_FILES})
target_link_libraries(baseGL ${LIBS})

add_executable(simple ${CMAKE_SOURCE_DIR}/examples/simple/simple.cpp ${CMAKE_SOURCE_DIR}/examples/simple/SimpleApp.cpp ${CMAKE_SOURCE_DIR}/examples/simple/SimpleApp.h)
target_link_libraries(simple baseGL)