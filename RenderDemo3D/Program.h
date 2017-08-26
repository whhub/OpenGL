#pragma once
#include "RenderTriangle.h"
#include "RenderCone.h"
#include "RenderBall.h"
#include "MultiObj.h"


class Program
{
public:
	Program(void);
	~Program(void);
	
    RenderTriangle triangle; 
	RenderCone cone;
    RenderBall ball;
	MultiObj mObj;

	int status;

	void InitRender();
	void RenderDisplay();
	void MouseRender(float x,float y, float lastX, float lastY, int type = -1);
	void KeyDown(unsigned char key);

};

