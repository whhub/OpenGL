#include <iostream>
#include <cstring>
#include "Program.h"
#include "renderobjectbase.h"

bool mouseisdown=false;
bool midmouseisdown = false;
bool rightmouseisdown = false;
bool loopr=false;

int lastX = 0;	
int lastY = 0;

int deltaY = 0;
int deltaX = 0;

Program program;
void Render();


void mouse(int button, int state, int x, int y)
{
	 if(button == GLUT_LEFT_BUTTON)
	 {
		 if(state == GLUT_DOWN)
		 {
			 mouseisdown=true;
			 program.MouseRender(x,y,0.0,0.0,0);//Ñ¡Ôñ	 
			 lastX=x;
			 lastY=y;
			 loopr=false;
		 }
		  else
		  {
			  mouseisdown=false;
		  }		  
	 }
	 if(button == GLUT_MIDDLE_BUTTON)
	 {
		 if(state == GLUT_DOWN)
		 {
			 midmouseisdown=true;			 
			 lastX=x;
			 lastY=y;			 
		 }
		 else
		 {
			 midmouseisdown=false;
		 }		  
	 }
	 if (button== GLUT_RIGHT_BUTTON)
	 {
		 if(state == GLUT_DOWN)
		 {
			 rightmouseisdown=true;			 
			 lastX=x;
			 lastY=y;			 
		 }
		 else
		 {
			 rightmouseisdown=false;
		 }		  
	 }	  
}

void motion(int x, int y)
{
	if (program.status != 3)
	{
		if(mouseisdown==true)
		{
			deltaY+=x-lastX;
			deltaX+=y-lastY;
			lastX=x;
			lastY=y;

			program.MouseRender(x,y,0.0,0.0,-1);
		}
	}
	else		//program.status == 3
	{
		if(mouseisdown==true)
		{
		}
		if(midmouseisdown==true)
		{
			program.MouseRender(x,y,lastX,lastY,1);//Æ½ÒÆ
		}
		if(rightmouseisdown==true)
		{
			program.MouseRender(x,y,lastX,lastY,2);//Ðý×ª
		}
	}	
}

void processNormalKeys(unsigned char key,int x,int y)
{
    program.KeyDown(key);
}

void Render()
{
	program.RenderDisplay();
}

int main(int argc, char *argv[])  
{  
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  
	glutInitWindowPosition(100, 100);  
	glutInitWindowSize(800,800);  
	glutCreateWindow("First OpenGl");

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		return 0;
	}
	
	program.InitRender();
	glutDisplayFunc(Render); 
	
	glutIdleFunc(&Render); 
	glutMouseFunc(mouse);
    glutKeyboardFunc(processNormalKeys);
	glutMotionFunc(motion);
    glutMainLoop();  
	return 0;  
}  
