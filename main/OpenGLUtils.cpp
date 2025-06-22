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
//TODO : add to total vertex array function that can be called to add / draw new lines in callbacks,
// and that returns a pointer to the line in the combined array for deletion

void DrawLine(float* pointA, float* pointB, float thickness,int lineType, float* outPoints) //draws a quad between 2 points
{
    Vector2 vec = Vector2(pointB[0] - pointA[0], pointB[1] - pointA[1]);
    Vector2 ortho = Vector2(vec.y, -vec.x);

    if(sizeof(outPoints) != 18*sizeof(float)){
        std::cout << "outpoints was declared with wrong size for drawline" << std::endl;
    }
     // 6 vertices of our quad line
    outPoints[0] = pointA[0] + (ortho * (thickness/2)).x;
    outPoints[1] = pointA[1] + (ortho * (thickness/2)).y;
    outPoints[2] = (float)lineType;
    outPoints[3] = pointA[0] - (ortho * (thickness/2)).x;
    outPoints[4] = pointA[1] + (ortho * (thickness/2)).y;
    outPoints[5] = (float)lineType;
    outPoints[6] = pointB[0] + (ortho * (thickness/2)).x;
    outPoints[7] = pointB[1] + (ortho * (thickness/2)).y;
    outPoints[8] = (float)lineType;
    
    outPoints[9] = pointB[0] + (ortho * (thickness/2)).x;
    outPoints[10] = pointB[1] + (ortho * (thickness/2)).y;
    outPoints[11] = (float)lineType;
    outPoints[12] = pointB[0] - (ortho * (thickness/2)).x;
    outPoints[13] = pointB[1] + (ortho * (thickness/2)).y;
    outPoints[14] = (float)lineType;
    outPoints[15] = pointA[0] - (ortho * (thickness/2)).x;
    outPoints[16] = pointA[1] + (ortho * (thickness/2)).y;
    outPoints[17] = (float)lineType;
}