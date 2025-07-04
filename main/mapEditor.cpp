#include <glad/glad.h>
#include <mapEditor.h>
#include <GLFW/glfw3.h>
#include "OpenGLUtils.h"
#include<iostream>

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

void MapEditor::drawLines(float* outPoints, int start){ // adds the array of points necessary for drawing all lines to outpoint starting at start


    for(int i = 0; i<lastLine; i++){
        float pointA[2] = {lines[i].startPoint.x,lines[i].startPoint.y};
        float pointB[2] = {lines[i].endPoint.x,lines[i].endPoint.y};

        DrawLine(pointA,pointB,lines[i].thickness,lines[i].type,&outPoints[start+18*i]);
    }
}