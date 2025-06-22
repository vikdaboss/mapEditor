cmake_minimum_required(VERSION 3.15)
project(MapEdit)

set(CMAKE_CXX_STANDARD 17)

# Add GLAD source
add_library(glad STATIC main/glad.c)
target_include_directories(glad PUBLIC libraries/include)

# Create your main executable
add_executable(mapedit
 main/main.cpp
 main/windowManager.cpp
 main/callbacks.cpp
 main/OpenGLUtils.cpp
 main/mapEditor.cpp
 main/myMath.cpp
 )

# Include paths
target_include_directories(mapedit
    PRIVATE
        libraries/include        # GLFW/glfw3.h
        main
)

# Link precompiled GLFW (static .a file)
target_link_libraries(mapedit
    PRIVATE
        glad
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libglfw3.a
)

#copy shaders over 
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Link OpenGL on Windows
if (WIN32)
    target_link_libraries(mapedit PRIVATE opengl32)
endif()