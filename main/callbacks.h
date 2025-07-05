#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace InputState{
    extern float mouseX;
    extern float mouseY;
    extern float mouseX_normalized;
    extern float mouseY_normalized;
    extern float mousePosX;
    extern float mousePosY;
    extern float mouse_delta[2];
    extern float mouse_delta_normalized[2];

    extern float scrollX;
    extern float scrollY;

    extern int window_width;
    extern int window_height;

    extern int mouse_button;
    extern int mouse_action;
    extern int last_mouse_action;
}

void mouse_button_callback(GLFWwindow* window,int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window,double xoffset, double yoffset);