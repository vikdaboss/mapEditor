#include <mapEditor.h>
#include <GLFW/glfw3.h>

MapEditor::MapEditor(GLFWwindow* window){
    zoomLevel = 1;
    GLwindow = window;
}

void MapEditor::zoomIn() {
    if(zoomLevel <6){
        zoomLevel++;
    }
}

void MapEditor::zoomOut() {
    if (zoomLevel > 2) {
        zoomLevel--;
    }
}

int MapEditor::get_zoom() {
    return zoomLevel;
}