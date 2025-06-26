#version 460 core

uniform vec2 windowSize;
uniform vec2 centerOffset;
uniform int zoomLevel;
out vec4 frag_color;

void main()
{
    vec2 offset = centerOffset * windowSize /2 ;
    if(zoomLevel>1)
        if( mod(gl_FragCoord.x- (offset.x + windowSize.x/2),10.0 * zoomLevel)<1.0 || mod(gl_FragCoord.y- (offset.y + windowSize.y/2),10.0 * zoomLevel)<1.0)
            frag_color = vec4(1.0,1.0,1.0,1.0);

    if (mod(gl_FragCoord.x- (offset.x + windowSize.x/2),50.0 * zoomLevel)<3.0 || mod(gl_FragCoord.y- (offset.y + windowSize.y/2),50.0 * zoomLevel)<3.0 )
        frag_color = vec4(1.0,1.0,1.0,1.0);

    if( abs(gl_FragCoord.x - (offset.x + windowSize.x/2)) <3.0)
        frag_color = vec4(0.0,1.0,0.0,1.0);
    else if( abs(gl_FragCoord.y - (offset.y + windowSize.y/2)) <3.0)
        frag_color = vec4(1.0,0.0,0.0,1.0);
    //frag_color = vec4(gl_FragCoord.x / windowSize.x,gl_FragCoord.y / windowSize.y,0.0,1.0);
}