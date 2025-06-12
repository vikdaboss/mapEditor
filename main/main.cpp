#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<windowManager.h>
#include<callbacks.h>
#include<OpenGLUtils.h>
#include<mapEditor.h>
#include<chrono>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    InputState::window_width = width;
    InputState::window_height = height;
    glViewport(0,0,width,height);
}

int main()
{
    
    GLFWwindow* window = WindowManager::createWindow(500,500,"Floor Editor");
    MapEditor mapEditor = MapEditor(window);

    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    //glfwSwapInterval(0); // Disables V-Sync
    WindowManager::setupCallbacks(window);

    float points[] = {
        -1.0f,  1.0f,  // Top-left
         1.0f,  1.0f,  // Top-right
        -1.0f, -1.0f,  // Bottom-left

        -1.0f, -1.0f,  // Bottom-left
         1.0f,  1.0f,  // Top-right
         1.0f, -1.0f   // Bottom-right
    };
    float center_offset[] = {0.0,0.0};

    //create vertex buffer object
    GLuint vbo = 0;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    //create vertex array object
    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0 );

    //set black clear color and alpha blending
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //load shader program
    GLuint shaderProgram = LoadShader("shaders/vertex.glsl","shaders/fragment.glsl");

    //get uniform location to pass to the shader in program loop
    glUseProgram(shaderProgram);
    GLint shader_windowSize = glGetUniformLocation(shaderProgram, "windowSize");
    GLint shader_centerOffset = glGetUniformLocation(shaderProgram, "centerOffset");
    GLint shader_zoom = glGetUniformLocation(shaderProgram, "zoomLevel");

    //dumb profiling
    std::chrono::_V2::system_clock::time_point frame_start;
    std::chrono::_V2::system_clock::time_point frame_end;
    std::chrono::duration<double> frame_time;

    int lastMouseX = 0;
    int lastMouseY = 0;
    //mainLoop
    while ( !glfwWindowShouldClose( window ) )
    {
        
        frame_start = std::chrono::high_resolution_clock::now();
        glfwPollEvents();

        //Input stuff
        if(InputState::mouseX == lastMouseX && InputState::mouseY == lastMouseY)
        {
            InputState::mouse_delta[0] = 0.0;
            InputState::mouse_delta[1] = 0.0;
            InputState::mouse_delta_normalized[0] = 0.0;
            InputState::mouse_delta_normalized[1] = 0.0;
        }
        lastMouseX = InputState::mouseX;
        lastMouseY = InputState::mouseY;
        
        if(InputState::mouse_action == 1 && InputState::mouse_button == 2)
        {
            center_offset[0] += InputState::mouse_delta[0];
            center_offset[1] += InputState::mouse_delta[1];
        }

        if(InputState::scrollY > 0){
            mapEditor.zoomIn();
        }
        if(InputState::scrollY < 0){
            mapEditor.zoomOut();
        }
        InputState::scrollX = 0;
        InputState::scrollY = 0;

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glUseProgram( shaderProgram );

        //setting shader values
        glUniform2f(shader_windowSize,InputState::window_width,InputState::window_height);
        glUniform1i(shader_zoom,mapEditor.get_zoom());
        glUniform2f(shader_centerOffset,center_offset[0],-center_offset[1]);

        //drawing triangles
        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLES, 0, 6 );
    
        //end of frame
        glfwSwapBuffers( window );

        frame_end = std::chrono::high_resolution_clock::now();
        frame_time = frame_end - frame_start;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}