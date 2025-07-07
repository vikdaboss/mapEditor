#include <glad/glad.h>
#include <mapEditor.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <vector>
#include "OpenGLUtils.h"
#include "callbacks.h"

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

int MapEditor::get_numLines(){
    return lastLine;
}

void MapEditor::addLine(float* startPoint, float* endPoint, int type, float thickness){

    lines[lastLine] = {Vector2(startPoint[0],startPoint[1]),Vector2(endPoint[0],endPoint[1]),type,thickness};
    lastLine += 1;
}

void MapEditor::editLine(int line, int startEnd, float* newPoint, int type, float thickness){
    if(startEnd == LINE_START){
        lines[line].startPoint.x = newPoint[0];
        lines[line].startPoint.y = newPoint[1];
    }
    if(startEnd == LINE_END){
        lines[line].endPoint.x = newPoint[0];
        lines[line].endPoint.y = newPoint[1];
    }
    lines[line].type = type;
    lines[line].thickness = thickness;
    editedLines[line] = 1;
}

void MapEditor::drawLines(std::vector<float>& outPoints, int start){ // adds the array of points necessary for drawing all lines to outpoint starting at start

    for(int i = 0; i < lastLine; i++){
        if(editedLines[i] == 1){
            float pointA[2] = {lines[i].startPoint.x,lines[i].startPoint.y};
            float pointB[2] = {lines[i].endPoint.x,lines[i].endPoint.y};

            DrawLine(pointA,pointB,lines[i].thickness,lines[i].type,outPoints,start + 18*i);
        }
    }

}

void MapEditor::set_AppState(int newState){
    previousAppState = appState;
    appState = newState;
}
void MapEditor::revert_AppState(){
    appState = previousAppState;
}
int MapEditor::get_AppState(){
    return appState;
}

void MapEditor::handleMouseInputs(){
    int zoom = get_zoom();
    float aspect = (float)InputState::window_width / (float)InputState::window_height;
    float scale = 12.5f / zoom;

    Vector2 deltaWorld;
    deltaWorld.x =  InputState::mouse_delta[0] / InputState::window_width  * 2.0f * scale;
    deltaWorld.y = -InputState::mouse_delta[1] / InputState::window_height * 2.0f * scale;
    
    if(InputState::mouseX == lastMouseX && InputState::mouseY == lastMouseY)
    { // if the mouse hasn't moved, reset delta
        
        InputState::mouse_delta[0] = 0.0;
        InputState::mouse_delta[1] = 0.0;
        InputState::mouse_delta_normalized[0] = 0.0;
        InputState::mouse_delta_normalized[1] = 0.0;
    }
    
    if(InputState::mouse_action == 1 && InputState::mouse_button == 2) //middle click held
    {
        offset.x += deltaWorld.x;
        offset.y += deltaWorld.y;
    }

    //placing a line
    if(InputState::mouse_action == 0 && InputState::last_mouse_action==1 && InputState::mouse_button == 0){//left click released
        if(get_AppState() == VIEWING){
            
            float linePos[] = {roundToNearest(InputState::mousePosX,0.5f),roundToNearest(InputState::mousePosY,0.5f)};
            float linePos2[] = {InputState::mousePosX + 10.0f,InputState::mousePosY + 10.0f};
            addLine(linePos,linePos2,1,0.1f);
            set_AppState(PLACING_LINE);
        }
        else if(get_AppState() == PLACING_LINE){
            revert_AppState();
        }
    }
    
    if(get_AppState() == PLACING_LINE){
        float linePos[] = {roundToNearest(InputState::mousePosX,0.5f),roundToNearest(InputState::mousePosY,0.5f)};
        editLine(get_numLines()-1,LINE_END,linePos,1,0.1f);
    }

    //scrolling
    if(InputState::scrollY > 0){
        zoomIn();
    }
    if(InputState::scrollY < 0){
        zoomOut();
    }
    InputState::scrollX = 0;
    InputState::scrollY = 0;

    lastMouseX = InputState::mouseX;
    lastMouseY = InputState::mouseY;
    InputState::last_mouse_action = InputState::mouse_action;

}