#include <GLFW/glfw3.h>
#include "myMath.h"

struct Line{
    Vector2 startPoint;
    Vector2 endPoint;
    int type;
    float thickness;
};

class MapEditor {

public:
    MapEditor(GLFWwindow* window);
    Vector2 offset;

    int max_zoom;
    void zoomIn();
    void zoomOut();
    int get_zoom();
    

    Line lines[512]; //512 should be plenty for now
    void addLine(float* startPoint, float* endPoint, int type, float thickness);
    void drawLines(float* outPoints, int start);
    int get_numLines();

private:
    int lastLine = 0;
    int zoomLevel;
    GLFWwindow* GLwindow;
};


