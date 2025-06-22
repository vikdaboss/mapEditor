#include <string>
#include <glad/glad.h>

GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
void DrawLine(float* pointA, float* pointB, float thickness,int lineType, float* outPoints);