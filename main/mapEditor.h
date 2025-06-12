#include <GLFW/glfw3.h>

class MapEditor {

public:
    MapEditor(GLFWwindow* window);
    void zoomIn();
    void zoomOut();
    int get_zoom(); 
private:
    int zoomLevel;
    GLFWwindow* GLwindow;
};


