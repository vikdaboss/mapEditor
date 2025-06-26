#include <string>
#include <glad/glad.h>

struct shaderProgram{
    std::string vertex;
    std::string fragment;
};

shaderProgram loadedShaders[256];
int lastLoadedShader = 0;
GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
void DrawLine(float* pointA, float* pointB, float thickness,int lineType, float* outPoints);