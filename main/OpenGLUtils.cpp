#include <OpenGLUtils.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <myMath.h>

static std::string readFile(const char* filePath) {
    
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint LoadShader(const char* vertexPath, const char* fragmentPath) {

    std::string vertexCodeStr = readFile(vertexPath);
    std::string fragmentCodeStr = readFile(fragmentPath);

    //log if shaders fail to load or don't exist
    if (vertexCodeStr.empty()) {
        std::cerr << "Vertex shader is empty or failed to load!\n";
    }
    
    if (fragmentCodeStr.empty()) {
        std::cerr << "Fragment shader is empty or failed to load!\n";
    }

    const char* vertexShaderCode = vertexCodeStr.c_str();
    const char* fragmentShaderCode = fragmentCodeStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderCode,nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Linking Error:\n" << infoLog << std::endl;
    }
    return shaderProgram;
}

GLuint DrawLine(float* pointA, float* pointB, float thickness) //draws a quad between 2 points
{
    Vector2 vec = Vector2(pointB[0] - pointA[0], pointB[1] - pointB[1]);
    Vector2 ortho = Vector2(vec.y, -vec.x);

    float points[] = { // 4 points of our quad line
        pointA[0] + (ortho * (thickness/2)).x, pointA[1] + (ortho * (thickness/2)).y,
        pointA[0] - (ortho * (thickness/2)).x, pointA[1] + (ortho * (thickness/2)).y,
        pointB[0] + (ortho * (thickness/2)).x, pointB[1] + (ortho * (thickness/2)).y,
        pointB[0] - (ortho * (thickness/2)).x, pointB[1] + (ortho * (thickness/2)).y,
    };
    
}