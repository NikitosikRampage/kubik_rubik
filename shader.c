#include <glad/glad.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shader.h"

char* Shader_LoadFile(char* FileName)
{
	FILE* f = fopen(FileName, "rb");
	if (!f)
	{
		printf("Error %s Not Found", FileName);
	}
	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	char* text = malloc(len + 1);
	memset(text, 0, len + 1);

	fseek(f, 0, SEEK_SET);
	fread(text, 1, len, f);
	fclose(f);

	return text;
}


GLuint Shader_CreateShader(char* FileName, GLuint type)
{
	char* txt = Shader_LoadFile(FileName);
	if (!txt) {
		return 0;
	}

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char* const*)&txt, NULL);
	glCompileShader(shader);

	GLint ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		GLchar log[2000];
		glGetShaderInfoLog(shader, sizeof(log), NULL, log);
		printf("Error compiling shader %s: %s\n", FileName, log);
		glDeleteShader(shader);
		free(txt);
		return 0;
	}

	free(txt);
	return shader;
}

GLuint Shader_CreateProgramm(char* VertexFileName, char* FragmentFileName)
{
	GLuint vert = Shader_CreateShader(VertexFileName, GL_VERTEX_SHADER); 
	GLuint frag = Shader_CreateShader(FragmentFileName, GL_FRAGMENT_SHADER);

	if (!vert || !frag) {
		return 0; 
	}

	GLuint prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog);

	GLint ok;
	glGetProgramiv(prog, GL_LINK_STATUS, &ok);
	if (!ok) {
		GLchar log[2000];
		glGetProgramInfoLog(prog, sizeof(log), NULL, log);
		printf("Error linking program: %s\n", log);
		glDeleteProgram(prog);
		glDeleteShader(vert);
		glDeleteShader(frag);
		return 0;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	return prog;
}