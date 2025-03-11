#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include <glad/glad.h>

char* Shader_LoadFile(char* FileName);
GLuint Shader_CreateShader(char* FileName, GLuint type);
GLuint Shader_CreateProgramm(char* VertexFileName, char* FragmentFileName);

#endif 