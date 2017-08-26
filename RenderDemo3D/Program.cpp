#include "Program.h"


Program::Program(void)
{
	status = 3;
}

void Program::InitRender()
{ 
	if(status==0)
	{ 
		triangle.vertStr="triangle.vert";
		triangle.flagStr="triangle.frag";
		triangle.Init();
	}
	
	if(status==1)
	{ 
		cone.vertStr="cone.vert";
		cone.flagStr="triangle.frag";
		cone.Init();
	}

    if(status==2)
    {
		ball.vertStr="ball.vert";
		ball.flagStr="ball.frag";
		ball.Init();
    }

	if(status==3)
	{
		mObj.vertStr="ball.vert";
		mObj.flagStr="ball.frag";
		mObj.Init();
	}
}

void Program::RenderDisplay()
{ 
	if(status==0)
	{ 
		triangle.Display();
	}

	if(status==1)
	{ 
		cone.Display();
	}
	
    if(status ==2)
    {
        ball.Display();
    }

	if(status ==3)
	{
		mObj.Display();
	}
}

void Program::MouseRender(float x,float y,float lastX, float lastY, int type)
{ 
	if(status==0)
	{ 
		triangle.rotateAngle++;
		if(triangle.rotateAngle>360) triangle.rotateAngle=0;
	}
	
	if(status==1)
	{ 
		cone.rotateAngle++;
		if(cone.rotateAngle>360) cone.rotateAngle=0;
	}

    if(status==2)
    {
		ball.rotateAngle++;
		if(ball.rotateAngle>360) ball.rotateAngle=0;
    }

	if(status==3)
	{
		//mObj.rotateAngle++;
		//mObj.triangleRotateAngle++;
		//if(mObj.rotateAngle>360) mObj.rotateAngle=0;
		//if(mObj.triangleRotateAngle>360) mObj.triangleRotateAngle=0;
		mObj.ChangeObj( x, y, lastX,  lastY,  type);
	}
	RenderDisplay();
}

void Program::KeyDown(unsigned char key)
{
    if(status==2)
    {
        ball.ChangeLight(key);
    }
	if(status==3)
	{
		mObj.ChangeLight(key);
	}
}

Program::~Program(void)
{
}
