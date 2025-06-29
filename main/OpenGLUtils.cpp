#include "OpenGLUtils.h"
#include "myMath.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

Shader loadedShaders[256];
int lastLoadedShader = 0;

static std::string readFile(const char* filePath) {
    
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Shader* LoadShader(const char* vertexPath, const char* fragmentPath) {

    int shaderIndex = -1;
    //check if shader isn't alread loaded
    for(int i=0; i< lastLoadedShader; i++){
        if(loadedShaders[i].vertexPath == vertexPath && loadedShaders[i].fragmentPath == fragmentPath)
        {
            shaderIndex = i;
            break;
        }
    }
    if(shaderIndex == -1)
    { // if shader isnt already in the list we add it to the next available space
        shaderIndex = lastLoadedShader;
        lastLoadedShader +=1 ; 
    } 
    loadedShaders[shaderIndex].vertexPath = vertexPath;
    loadedShaders[shaderIndex].fragmentPath = fragmentPath;

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

    loadedShaders[shaderIndex].shaderProgramID = shaderProgram; //store the created shader program ID

    //delete vertex and frag shader now that program has both
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);

    GLint success; //if failed print some shit
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Linking Error:\n" << infoLog << std::endl;
    }

    return &loadedShaders[shaderIndex];
}

void ReloadAllShaders() 
{
    for(int i = 0;i < lastLoadedShader;i++)
    {
        //delete old shader program
        if(loadedShaders[i].shaderProgramID != 0) { glDeleteProgram(loadedShaders[i].shaderProgramID); }
        //load new shader program
        LoadShader(loadedShaders[i].vertexPath.c_str(),loadedShaders[i].fragmentPath.c_str());

        if(loadedShaders[i].uniforms.size() >0){ //update uniforms if they exist
            for(auto it = loadedShaders[i].uniforms.begin(); it!=loadedShaders[i].uniforms.end();it++){
                *it->second = glGetUniformLocation(loadedShaders[i].shaderProgramID, it->first.c_str());
                std::cout << it->first.c_str() << std::endl;
            }
        }
    }
}
//TODO : "add to total vertex array" function that can be called to add / draw new lines in callbacks,
// and that returns a pointer to the line in the combined array for deletion

void DrawLine(float* pointA, float* pointB, float thickness,int lineType, float* outPoints) //draws a quad between 2 points
{
    Vector2 vec = Vector2(pointB[0] - pointA[0], pointB[1] - pointA[1]);
    Vector2 ortho = Vector2(-vec.normalized().x,vec.normalized().y);

    // 6 vertices of our quad line
    outPoints[0] = pointA[0] + (ortho * (thickness/2)).x;
    outPoints[1] = pointA[1] + (ortho * (thickness/2)).y;
    outPoints[2] = (float)lineType;
    outPoints[3] = pointA[0] - (ortho * (thickness/2)).x;
    outPoints[4] = pointA[1] - (ortho * (thickness/2)).y;
    outPoints[5] = (float)lineType;
    outPoints[6] = pointB[0] + (ortho * (thickness/2)).x;
    outPoints[7] = pointB[1] + (ortho * (thickness/2)).y;
    outPoints[8] = (float)lineType;
    
    outPoints[9] = pointB[0] + (ortho * (thickness/2)).x;
    outPoints[10] = pointB[1] + (ortho * (thickness/2)).y;
    outPoints[11] = (float)lineType;
    outPoints[12] = pointB[0] - (ortho * (thickness/2)).x;
    outPoints[13] = pointB[1] - (ortho * (thickness/2)).y;
    outPoints[14] = (float)lineType;
    outPoints[15] = pointA[0] - (ortho * (thickness/2)).x;
    outPoints[16] = pointA[1] - (ortho * (thickness/2)).y;
    outPoints[17] = (float)lineType;
}