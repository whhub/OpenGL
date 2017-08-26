#pragma once
#include "renderobjectbase.h"

class RenderTriangle 
{
public:	
	RenderTriangle(void);
	~RenderTriangle(void);

	float rotateAngle;
	char* vertStr;
	char* flagStr;

	GLuint VAO,VBO,EBO;
	GLuint shader_program;

    void Init(void);
	void Display(void);
};

