#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace InputState{
    extern double mouseX;
    extern double mouseY;
    extern double mouseX_normalized;
    extern double mouseY_normalized;
    extern double mousePosX;
    extern double mousePosY;
    extern double mouse_delta[2];
    extern double mouse_delta_normalized[2];

    extern double scrollX;
    extern double scrollY;

    extern int window_width;
    extern int window_height;

    extern int mouse_button;
    extern int mouse_action;
}

void mouse_button_callback(GLFWwindow* window,int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window,double xoffset, double yoffset);