#include <GLFW/glfw3.h>
#include <myMath.h>
class MapEditor {

public:
    MapEditor(GLFWwindow* window);
    Vector2 offset;
    int max_zoom;
    void zoomIn();
    void zoomOut();
    int get_zoom(); 
private:
    int zoomLevel;
    GLFWwindow* GLwindow;
};


