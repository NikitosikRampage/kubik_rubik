#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int ID;
} Shader;

// Прототипы функций
Shader Shader_create(const char* vertexPath, const char* fragmentPath);
void Shader_use(Shader* shader);
void Shader_setBool(Shader* shader, const char* name, int value);
void Shader_setInt(Shader* shader, const char* name, int value);
void Shader_setFloat(Shader* shader, const char* name, float value);

// Вспомогательные функции
static void checkCompileErrors(unsigned int shader, const char* type);
static char* readFile(const char* path);

#endif