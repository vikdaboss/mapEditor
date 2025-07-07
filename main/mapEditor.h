#include <GLFW/glfw3.h>
#include <vector>
#include "myMath.h"

struct Line{
    Vector2 startPoint;
    Vector2 endPoint;
    int type;
    float thickness;
};

#define VIEWING 0
#define PLACING_LINE 1
#define DELETING_LINE 2
#define EDITING_LINE 3

#define LINE_START 0
#define LINE_END 1

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
    void editLine(int line, int startEnd, float* newPoint, int type, float thickness);
    void drawLines(std::vector<float>& outPoints, int start);
    int get_numLines();

    void set_AppState(int newState);
    void revert_AppState();
    int get_AppState();

    void handleMouseInputs();

private:
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;
    int appState = VIEWING;
    int previousAppState = VIEWING;
    int lastLine = 0;
    int editedLines[512];
    int zoomLevel;
    GLFWwindow* GLwindow;
};


