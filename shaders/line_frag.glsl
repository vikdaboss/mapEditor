#version 460 core

in float vAttrib;
in vec2 worldPos;
out vec4 frag_color;

void main()
{
    frag_color = vec4(vAttrib/2.0,1.0,1.0,1.0);
}