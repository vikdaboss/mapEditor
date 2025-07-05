#version 460 core

layout (location = 0) in vec2 aPos;        // x, y
layout (location = 1) in float aAttrib;    // your custom attribute

uniform mat4 u_MVP;

out float vAttrib;  // Pass to fragment shader
out vec2 worldPos; // Pass to fragment shader

void main() {
    gl_Position = u_MVP * vec4(aPos, 0.0, 1.0);
    vAttrib = aAttrib; // Pass it through
    worldPos = aPos;
}