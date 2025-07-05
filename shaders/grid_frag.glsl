#version 460 core

uniform vec2 windowSize;
uniform vec2 centerOffset;
uniform int zoomLevel;
in vec2 worldPos;

out vec4 frag_color;

void main()
{
    vec3 uBackgroundColor = vec3(0.0, 0.0, 0.0);
    vec3 uGridColor = vec3(0.5, 0.5, 0.5);
    float uGridSize = 0.1;
    float uAxisWidth = 10.0;
    vec3 uXAxisColor = vec3(1.0, 0.0, 0.0);
    vec3 uYAxisColor = vec3(0.0, 1.0, 0.0);
    vec2 aspectRatio = vec2(windowSize.x / windowSize.y, 1.0);
    vec2 pos = worldPos - centerOffset ;

    vec2 grid = abs(fract(pos * aspectRatio/ (uGridSize * zoomLevel) - 0.5) - 0.5) / fwidth(pos *aspectRatio/ (uGridSize * zoomLevel));   
    float line = min(grid.x, grid.y);
    
    // Create grid lines
    float gridMask = 1.0 - min(line, 1.0);
    
    // Calculate axis lines
    float xAxisMask = 1.0 - smoothstep(0.0, uAxisWidth / windowSize.y, abs(pos.y)); // Horizontal line at Y=0
    float yAxisMask = 1.0 - smoothstep(0.0, uAxisWidth / windowSize.x, abs(pos.x)); // Vertical line at X=0
    
    // Start with background color
    vec3 finalColor = uBackgroundColor;
    
    // Add grid
    finalColor = mix(finalColor, uGridColor, gridMask);
    
    // Add axes (they override grid)
    finalColor = mix(finalColor, uXAxisColor, xAxisMask); // Red X-axis
    finalColor = mix(finalColor, uYAxisColor, yAxisMask); // Green Y-axis
    
    frag_color = vec4(finalColor, 1.0);

    
}