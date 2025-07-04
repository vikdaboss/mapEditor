
#include <glad/glad.h>
#include <string>
#include <unordered_map>


struct Shader{
    std::string vertexPath;
    std::string fragmentPath;
    GLuint shaderProgramID;
    std::unordered_map<std::string,GLint*> uniforms;
};

extern Shader loadedShaders[];
extern int lastLoadedShader;

Shader* LoadShader(const char* vertexPath, const char* fragmentPath);
void ReloadAllShaders();
void DrawLine(float* pointA, float* pointB, float thickness,int lineType, float* outPoints);
