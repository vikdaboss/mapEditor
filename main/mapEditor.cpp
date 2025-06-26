#include <mapEditor.h>
#include <GLFW/glfw3.h>



MapEditor::MapEditor(GLFWwindow* window){
    offset = {0.0,0.0};
    zoomLevel = 1;
    max_zoom = 6;
    GLwindow = window;
}

void MapEditor::zoomIn() {
    if(zoomLevel < max_zoom){
        zoomLevel++;
    }
}

void MapEditor::zoomOut() {
    if (zoomLevel >= 2) {
        zoomLevel--;
    }
}

int MapEditor::get_zoom() {
    return zoomLevel;
}