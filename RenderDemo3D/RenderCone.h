#pragma once
#include "renderobjectbase.h"

class RenderCone
{
public:
	RenderCone(void);
	~RenderCone(void);

	float rotateAngle;
	char* vertStr;
	char* flagStr;

	GLuint VAO[2],VBO[2];
	GLuint shader_program;


    void Init(void);
	void Display(void);

	void Cone();
	
};

