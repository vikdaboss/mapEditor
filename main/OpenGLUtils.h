#include <string>
#include <glad/glad.h>

GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
GLuint DrawLine(float* pointA, float* pointB);