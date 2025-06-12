#include<glad/glad.h>
#include<windowManager.h>
#include<callbacks.h>
#include<iostream>

GLFWwindow* WindowManager::createWindow(int width, int height, const char* title)
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialized OpenGL context" << std::endl;
        return nullptr;
    }
    
    return window;
}

void WindowManager::setupCallbacks(GLFWwindow* window)
{
    glfwGetWindowSize(window,&InputState::window_width,&InputState::window_height);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
}