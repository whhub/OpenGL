#pragma once
#include "renderobjectbase.h"

class RenderBall
{
public:
	RenderBall(void);
	~RenderBall(void);
	
	float* verts;
	int vertNum;
	int slice ;
	int statck ;
	float r ;

	float rotateAngle;
	char* vertStr;
	char* flagStr;

	GLuint VAO,VBO;
	GLuint shader_program;

    GLfloat vDiffuseColor[3];
    GLfloat vAmbientColor[3];
    GLfloat vSpecularColor[3];
    GLfloat vLightPosition[3];
    GLfloat sh;

    void Init(void);
	void Display(void);
    void ChangeLight(unsigned char);
};