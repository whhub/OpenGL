#pragma once
#include <glew.h>
#include <stdlib.h>
#include <glut.h>  
#include <string>

GLuint LoadShader(const char *vertice_path, const char *framgment_path);
bool compileShader(GLuint &shader_id, const char *path);

static const GLchar *getFileData(const char * path);