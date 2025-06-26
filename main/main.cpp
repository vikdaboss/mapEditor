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
    float pointA[] = {5,5};
    float pointB[] = {-5,-5};
    float pointC[] = {-5,5};
    float pointD[] = {5,-5};
    
    float testLine1[18];
    DrawLine(pointA,pointB,0.1,1,testLine1);

    float testLine2[18];
    DrawLine(pointC,pointD,0.1,2,testLine2);

    float points[54];
    memcpy(points,grid_points, sizeof(grid_points));
    memcpy(points+18, testLine1, sizeof(testLine1));
    memcpy(points+36, testLine2, sizeof(testLine2));

    //create vertex buffer object
    GLuint vbo = 0;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

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
    GLuint shaderProgramGrid = LoadShader("shaders/gui_vertex.glsl","shaders/grid_frag.glsl");
    GLuint shaderProgramLine = LoadShader("shaders/vertex.glsl","shaders/line_frag.glsl");

    //get uniform location to pass to the shader in program loop
    GLint shader_windowSize = glGetUniformLocation(shaderProgramGrid, "windowSize");
    GLint shader_centerOffset = glGetUniformLocation(shaderProgramGrid, "centerOffset");
    GLint shader_zoom = glGetUniformLocation(shaderProgramGrid, "zoomLevel");

    GLint shader_MVP = glGetUniformLocation(shaderProgramLine, "u_MVP");

    //basic profiling
    std::chrono::_V2::system_clock::time_point frame_start;
    std::chrono::_V2::system_clock::time_point frame_end;
    std::chrono::duration<double> frame_time;
    
    int lastMouseX = 0;
    int lastMouseY = 0;
    float mouseWorld[4];

    //mainLoop
    while ( !glfwWindowShouldClose( window ) )
    {
        
        
        frame_start = std::chrono::high_resolution_clock::now();
        glfwPollEvents();

/////////////////////////////////////////INPUT AND VIEW MATRICES///////////////////////////////////////////////
        
        int zoom = mapEditor.get_zoom();
        float aspect = (float)InputState::window_width / (float)InputState::window_height;
        float scale = 12.5f / zoom;
    
        Vector2 deltaWorld;
        deltaWorld.x =  InputState::mouse_delta[0] / InputState::window_width  * 2.0f * scale;
        deltaWorld.y = -InputState::mouse_delta[1] / InputState::window_height * 2.0f * scale;
        
        if(InputState::mouseX == lastMouseX && InputState::mouseY == lastMouseY)
        { // if the mouse hasn't moved, reset delta
            
            InputState::mouse_delta[0] = 0.0;
            InputState::mouse_delta[1] = 0.0;
            InputState::mouse_delta_normalized[0] = 0.0;
            InputState::mouse_delta_normalized[1] = 0.0;
        }
        
        if(InputState::mouse_action == 1 && InputState::mouse_button == 2)
        { //moving canvas with middle mouse held
            mapEditor.offset.x += deltaWorld.x;
            mapEditor.offset.y += deltaWorld.y;
        }

        if(InputState::scrollY > 0){
            mapEditor.zoomIn();
        }
        if(InputState::scrollY < 0){
            mapEditor.zoomOut();
        }
        InputState::scrollX = 0;
        InputState::scrollY = 0;

        lastMouseX = InputState::mouseX;
        lastMouseY = InputState::mouseY;
        
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
        multMatrixVec4(invMvp, ndcVec, mouseWorld);
        InputState::mousePosX = mouseWorld[0];
        InputState::mousePosY = mouseWorld[1];

/////////////////////////////////////////////DRAWING GEOMETRY///////////////////////////////////////////////////////
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //drawing background grid
        glUseProgram( shaderProgramGrid );
        glUniform2f(shader_windowSize,InputState::window_width,InputState::window_height);
        glUniform1i(shader_zoom,zoom);
        glUniform2f(shader_centerOffset,mapEditor.offset.x/scale ,mapEditor.offset.y/scale);

        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLES, 0, 6 );

        //drawing lines
        glUseProgram(shaderProgramLine);
        glUniformMatrix4fv(shader_MVP,1,GL_FALSE,mvp);
        glDrawArrays( GL_TRIANGLES, 6, 12);
    
        //end of frame
        glfwSwapBuffers( window );

        frame_end = std::chrono::high_resolution_clock::now();
        frame_time = frame_end - frame_start;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}