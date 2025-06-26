#version 460 core

layout (location = 0) in vec2 aPos;        // x, y
layout (location = 1) in float aAttrib;    // your custom attribute


out float vAttrib;  // Pass to fragment shader

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}