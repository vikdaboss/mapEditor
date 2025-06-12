#version 460 core

uniform vec2 windowSize;
uniform vec2 centerOffset;
uniform int zoomLevel;
out vec4 frag_color;

void main()
{
    
    if( mod(gl_FragCoord.x- (centerOffset.x + windowSize.x/2),10.0 * zoomLevel)<1.1 || mod(gl_FragCoord.y- (centerOffset.y + windowSize.y/2),10.0 * zoomLevel)<1.1)
        frag_color = vec4(1.0,1.0,1.0,1.0);
    if (mod(gl_FragCoord.x- (centerOffset.x + windowSize.x/2),50.0 * zoomLevel)<3.0 || mod(gl_FragCoord.y- (centerOffset.y + windowSize.y/2),50.0 * zoomLevel)<3.0 )
        frag_color = vec4(1.0,1.0,1.0,1.0);
    if( abs(gl_FragCoord.x - (centerOffset.x + windowSize.x/2)) <5.0)
        frag_color = vec4(0.0,1.0,0.0,1.0);
    else if( abs(gl_FragCoord.y - (centerOffset.y + windowSize.y/2)) <5.0)
        frag_color = vec4(1.0,0.0,0.0,1.0);
    //frag_color = vec4(gl_FragCoord.x / windowSize.x,gl_FragCoord.y / windowSize.y,0.0,1.0);
}