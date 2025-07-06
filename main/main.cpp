#include<glad/glad.h>
#include<iostream>
#include<GLFW/glfw3.h>
#include<chrono>
#include<vector>
#include"windowManager.h"
#include"callbacks.h"
#include"OpenGLUtils.h"
#include"mapEditor.h"

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

    //view transform matrix
    float view[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    float proj[16];
    ortho(-12.5f, 12.5f, -12.5f, 12.5f, -0.5f, 0.5f, proj);

    float mvp[16];
    multMatrix(proj,view,mvp);

    float invMvp[16];
    invertMatrix(mvp,invMvp);

    float   grid_points[] = {
        -1.0f,  1.0f, 0.0f,  // Top-left
         1.0f,  1.0f, 0.0f,  // Top-right
        -1.0f, -1.0f, 0.0f,  // Bottom-left

        -1.0f, -1.0f, 0.0f,  // Bottom-left
         1.0f,  1.0f, 0.0f,  // Top-right
         1.0f, -1.0f, 0.0f   // Bottom-right
    };

    std::vector<float> points;
    points.insert(points.end(),grid_points,grid_points+sizeof(grid_points));
    
    //create vertex buffer object
    GLuint vbo = 0;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_DYNAMIC_DRAW );

    //create vertex array object
    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 );
    glEnableVertexAttribArray( 0 );
    //vertex type attrib
    glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(2*sizeof (float)));
    glEnableVertexAttribArray( 1 );

    //set black clear color and alpha blending
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //load shader programs
    Shader* shaderProgramGrid = LoadShader("shaders/gui_vertex.glsl","shaders/grid_frag.glsl");
    Shader* shaderProgramLine = LoadShader("shaders/vertex.glsl","shaders/line_frag.glsl");

    //get uniform locations to pass to the shader in program loop
    GLint shader_windowSize = glGetUniformLocation(shaderProgramGrid->shaderProgramID, "windowSize");
    loadedShaders[0].uniforms["windowSize"] = &shader_windowSize;
    GLint shader_centerOffset = glGetUniformLocation(shaderProgramGrid->shaderProgramID, "centerOffset");
    loadedShaders[0].uniforms["centerOffset"] = &shader_centerOffset;
    GLint shader_zoom = glGetUniformLocation(shaderProgramGrid->shaderProgramID, "zoomLevel");
    loadedShaders[0].uniforms["zoomLevel"] = &shader_zoom;

    GLint shader_MVP = glGetUniformLocation(shaderProgramLine->shaderProgramID, "u_MVP");
    loadedShaders[1].uniforms["u_MVP"] = &shader_MVP;

    //basic profiling
    std::chrono::_V2::system_clock::time_point frame_start;
    std::chrono::_V2::system_clock::time_point frame_end;
    std::chrono::duration<double> frame_time;
    
    int lastMouseX = 0;
    int lastMouseY = 0;
    int lastMouseAction = 0;
    float mouseWorld[4]; // vec4 containing the world coords of the mouse cursor

    //mainLoop
    while ( !glfwWindowShouldClose( window ) )
    {
        frame_start = std::chrono::high_resolution_clock::now();
        glfwPollEvents();

/////////////////////////////////////////INPUT AND VIEW MATRICES///////////////////////////////////////////////
        mapEditor.handleMouseInputs();

        int zoom = mapEditor.get_zoom();
        float aspect = (float)InputState::window_width / (float)InputState::window_height;
        float scale = 10.0f / zoom;
    
        // To prevent distortion:
        if (aspect >= 1.0f) {
            // Wide window — expand X range
            ortho(-scale * aspect, scale * aspect, -scale, scale, -1.0f, 1.0f, proj);
        } else {
            // Tall window — expand Y range
            ortho(-scale, scale, -scale / aspect, scale / aspect, -1.0f, 1.0f, proj);
        }

        translate(mapEditor.offset/scale,view); //updates view matrix
        multMatrix(proj,view,mvp); // updates mvp
        invertMatrix(mvp,invMvp); // updates inverse of mvp

        float ndcVec[4] = {(float)InputState::mouseX_normalized,(float)InputState::mouseY_normalized,0.0f,1.0f};
        multMatrixVec4(invMvp, ndcVec, mouseWorld); //transform our NDC mouse coords to world coords
        mouseWorld[0] = mouseWorld[0] - mapEditor.offset.x * aspect;
        mouseWorld[1] = mouseWorld[1] - mapEditor.offset.y;
        InputState::mousePosX = mouseWorld[0];
        InputState::mousePosY = mouseWorld[1];

/////////////////////////////////////////////DRAWING GEOMETRY///////////////////////////////////////////////////////
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        mapEditor.drawLines(points,18);
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_DYNAMIC_DRAW );
        //drawing background grid
        glUseProgram( shaderProgramGrid->shaderProgramID );
        glUniform2f(shader_windowSize,InputState::window_width,InputState::window_height);
        glUniform1i(shader_zoom,zoom);
        glUniform2f(shader_centerOffset,mapEditor.offset.x/scale ,mapEditor.offset.y/scale);

        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLES, 0, 6 ); //this is the background quad it'll never really change

        //drawing lines
        glUseProgram(shaderProgramLine->shaderProgramID);
        glUniformMatrix4fv(shader_MVP,1,GL_FALSE,mvp);
        glDrawArrays( GL_TRIANGLES, 6, mapEditor.get_numLines()*6); //this will be bound by the amount of lines we're drawing * 6 (2 tris per line)
    
        //end of frame
        glfwSwapBuffers( window );

        frame_end = std::chrono::high_resolution_clock::now();
        frame_time = frame_end - frame_start;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}