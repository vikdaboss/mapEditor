#include<callbacks.h>
#include<iostream>
#include<mapEditor.h>
#include<cmath>

namespace InputState{
    double mouseX = 0.0;
    double mouseY = 0.0;
    double mouseX_normalized = 0.0;
    double mouseY_normalized = 0.0;
    double mousePosX = 0.0;
    double mousePosY = 0.0;
    double mouse_delta[] = {0.0,0.0};
    double mouse_delta_normalized[] = {0.0,0.0};

    double scrollX = 0.0;
    double scrollY = 0.0;

    int window_width;
    int window_height;
    int mouse_button = 0;
    int mouse_action = 0;
}

void mouse_button_callback(GLFWwindow* window,int button, int action, int mods)
{
    InputState::mouse_button = button;
    InputState::mouse_action = action;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    InputState::mouse_delta[0] = xpos - InputState::mouseX;
    InputState::mouse_delta[1] = ypos - InputState::mouseY;
    InputState::mouse_delta_normalized[0] = InputState::mouse_delta[0] / InputState::window_width;
    InputState::mouse_delta_normalized[1] = InputState::mouse_delta[1] / InputState::window_height;

    InputState::mouseX = xpos;
    InputState::mouseY = ypos;
    float mag = sqrt(InputState::window_width*InputState::window_width + InputState::window_height * InputState::window_height); 
    InputState::mouseX_normalized = (xpos / InputState::window_width) * 2.0f - 1.0f;
    InputState::mouseY_normalized = 1.0f-(ypos / InputState::window_height) * 2.0f ;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    InputState::scrollX = xoffset;
    InputState::scrollY = yoffset;
}