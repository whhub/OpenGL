#include "MultiObj.h"

#define PI 3.14159

MultiObj::MultiObj(void)
{
	r = 0.25;
	slice = 20;
	statck = 50;
	GLfloat ambientColor[3] = {0.2,0.2,0.2};
	GLfloat diffuseColor[3] = {0.5,0.5,0.5};
	GLfloat specularColor[3] = {0.5, 0.5,0.5};
	GLfloat lightPosition[3] = {2.0, 1.0, 2.0};
	memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
	memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
	memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
	memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));

	memcpy(trianglevAmbientColor,ambientColor,sizeof(trianglevAmbientColor));
	sh = 3.0;

	selectMode = 0;

	movevec = glm::vec3(-0.6f, 0.0f, -3.0f);
	trianglemovevec = glm::vec3(0.6f, 0.0f, -3.0f);

	rotateAngle = -100;
	axisvec = glm::vec3(1.0f, 0.0f, 1.0f);
	triangleRotateAngle = -100;
	triangleaxisvec = glm::vec3(1.0f, 0.0f, 1.0f);
}

void MultiObj::Init()
{
	vertNum=slice*statck*4;//顶点总数,由四边形组成
	verts=new float[vertNum*3];//每个顶点有xyz三个分量,因此*3

	float stepAngZ=PI/slice;//纵向角度每次增加的值(0°~180°)
	float stepAngXY=(2*PI)/statck;//横向角度每次增加的值(0°~360°)
	float angZ=0.0;//初始的纵向角度
	float angXY=0.0;//初始的横向角度

	int index=0;

	for(int i=0;i<slice;i++) 
	{
		for(int j=0;j<statck;j++) 
		{
			float x1=r*sin(angZ)*cos(angXY);
			float y1=r*sin(angZ)*sin(angXY);
			float z1=r*cos(angZ);
			verts[index]=x1; index++;
			verts[index]=y1; index++;
			verts[index]=z1; index++;

			float x2=r*sin(angZ+stepAngZ)*cos(angXY);
			float y2=r*sin(angZ+stepAngZ)*sin(angXY);
			float z2=r*cos(angZ+stepAngZ);
			verts[index]=x2; index++;
			verts[index]=y2; index++;
			verts[index]=z2; index++;

			float x3=r*sin(angZ+stepAngZ)*cos(angXY+stepAngXY);
			float y3=r*sin(angZ+stepAngZ)*sin(angXY+stepAngXY);
			float z3=r*cos(angZ+stepAngZ);
			verts[index]=x3; index++;
			verts[index]=y3; index++;
			verts[index]=z3; index++;

			float x4=r*sin(angZ)*cos(angXY+stepAngXY);
			float y4=r*sin(angZ)*sin(angXY+stepAngXY);
			float z4=r*cos(angZ);
			verts[index]=x4; index++;
			verts[index]=y4; index++;
			verts[index]=z4; index++;

			angXY+=stepAngXY;
		}
		angXY=0.0;//每次横向到达2PI角度则横向角度归0
		angZ+=stepAngZ;
	}

	glGenVertexArrays(2,VAO);	
	glBindVertexArray(VAO[0]);

	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertNum*3, verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); 

	shader_program = LoadShader(vertStr,flagStr);	
	glUseProgram(shader_program);


	triangleRotateAngle = rotateAngle = -100;	
	glBindVertexArray(VAO[1]);
	glGenBuffers(1, &triangleEBO);

	float vertices[]={ 
		0.5f/2,-sqrt(6.0f)/12/2,-sqrt(3.0f)/6/2,0.5,0,0,    // 顶点x,y,z，颜色r,g,b
		-0.5f/2,-sqrt(6.0f)/12/2,-sqrt(3.0f)/6/2,0,0.5,0,
		0.0f,-sqrt(6.0f)/12/2, sqrt(3.0f)/3/2,0,0,0.5,
		0.0f, sqrt(6.0f)/4/2,  0,0.5,0.5,0
	};	  

	unsigned int indices[] = {  // note that we start from 0! 按索引顺序把四个顶点组成一个三棱锥
		1, 0, 3,  // first Triangle
		0,1, 2, // second Triangle
		0,2, 3,
		2,1,3
	};

	//glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,6*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT,  GL_FALSE,6*sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);



}

void MultiObj::Display()
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);  
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );

	time_t tt = time(NULL);
	tm* t= localtime(&tt);
	float timeValue = t->tm_sec;	
	float delta = fabs(sin(timeValue) / 2.0f);	

	glm::mat4 transform;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	model = glm::rotate(model, rotateAngle, axisvec);
	view  = glm::translate(view, movevec);
	projection = glm::perspective(45.0f,1.0f, 0.1f, 100.0f);

	unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
	unsigned int viewLoc  = glGetUniformLocation(shader_program, "view");
	unsigned int porjectionLoc= glGetUniformLocation(shader_program, "projection");
	unsigned int vLightPositionLoc  = glGetUniformLocation(shader_program, "vLightPosition");

	unsigned int ambientColorLoc = glGetUniformLocation(shader_program, "ambientColor");
	unsigned int diffuseColorLoc = glGetUniformLocation(shader_program, "diffuseColor");
	unsigned int specularColorLoc = glGetUniformLocation(shader_program, "specularColor");
	unsigned int shLoc  = glGetUniformLocation(shader_program, "sh");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(porjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(vLightPositionLoc, 1, vLightPosition);

	glUniform3fv(ambientColorLoc, 1, vAmbientColor);
	glUniform3fv(diffuseColorLoc, 1, vDiffuseColor);
	glUniform3fv(specularColorLoc, 1, vSpecularColor);
	glUniform1f(shLoc, sh);

	//glPolygonMode(GL_FRONT, GL_LINE); 
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_QUADS, 0, vertNum);  
	glBindVertexArray(0);

	//glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );
	glm::mat4 trianglemodel;
	glm::mat4 triangleview;
	glm::mat4 triangleprojection;

	trianglemodel = glm::rotate(trianglemodel, triangleRotateAngle, triangleaxisvec);		
	triangleview  = glm::translate(triangleview, trianglemovevec);
	triangleprojection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	//transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5)); 
	// retrieve the matrix uniform locations
	unsigned int trianglemodelLoc = glGetUniformLocation(shader_program, "model");
	unsigned int triangleviewLoc  = glGetUniformLocation(shader_program, "view");
	unsigned int triangleporjectionLoc= glGetUniformLocation(shader_program, "projection");
	unsigned int trianglevLightPositionLoc  = glGetUniformLocation(shader_program, "vLightPosition");

	unsigned int triangleambientColorLoc = glGetUniformLocation(shader_program, "ambientColor");
	unsigned int trianglediffuseColorLoc = glGetUniformLocation(shader_program, "diffuseColor");
	unsigned int trianglespecularColorLoc = glGetUniformLocation(shader_program, "specularColor");
	unsigned int triangleshLoc  = glGetUniformLocation(shader_program, "sh");

	glUniformMatrix4fv(trianglemodelLoc, 1, GL_FALSE, glm::value_ptr(trianglemodel));
	glUniformMatrix4fv(triangleviewLoc, 1, GL_FALSE, glm::value_ptr(triangleview));
	glUniformMatrix4fv(triangleporjectionLoc, 1, GL_FALSE, glm::value_ptr(triangleprojection));
	glUniform3fv(trianglevLightPositionLoc, 1, vLightPosition);

	glUniform3fv(triangleambientColorLoc, 1, trianglevAmbientColor);
	glUniform3fv(trianglediffuseColorLoc, 1, vDiffuseColor);	
	glUniform3fv(trianglespecularColorLoc, 1, vSpecularColor);
	glUniform1f(triangleshLoc, sh);


	// unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
	//  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)); 


	//glEnable(GL_DEPTH_TEST);  
	glBindVertexArray(VAO[1]);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	////glFlush();
	glutSwapBuffers(); 
}

void MultiObj::ChangeLight(unsigned char key)
{
	if(key=='0')
	{
		GLfloat ambientColor[3] = {0.2,0.2,0.2};
		GLfloat diffuseColor[3] = {0.5,0.5,0.5};
		GLfloat specularColor[3] = {0.5, 0.5,0.5};
		GLfloat lightPosition[3] = {2.0, 1.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 3.0;
	}
	if(key=='1')
	{
		//环境光增强
		GLfloat ambientColor[3] = {0.5,0.5,0.5};
		GLfloat diffuseColor[3] = {0.5,0.5,0.5};
		GLfloat specularColor[3] = {0.5, 0.5,0.5};
		GLfloat lightPosition[3] = {2.0, 1.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 3.0;
	}
	if(key=='2')
	{
		//漫射光增强
		GLfloat ambientColor[3] = {0.2,0.2,0.2};
		GLfloat diffuseColor[3] = {0.8,0.8,0.8};
		GLfloat specularColor[3] = {0.1, 0.1,0.1};
		GLfloat lightPosition[3] = {2.0, 1.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 3.0;
	}
	if(key=='3')
	{
		//绝缘体
		GLfloat ambientColor[3] = {0.2,0.2,0.2};
		GLfloat diffuseColor[3] = {0.747,0.747,0.1695};
		GLfloat specularColor[3] = {0.0, 0.0,0.0};
		GLfloat lightPosition[3] = {2.0, 1.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 3.0;
	}
	if(key=='4')
	{
		//镜面反射光增强
		GLfloat ambientColor[3] = {0.2,0.2,0.2};
		GLfloat diffuseColor[3] = {0.2,0.2,0.2};
		GLfloat specularColor[3] = {0.8, 0.8,0.8};
		GLfloat lightPosition[3] = {2.0, 1.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 3.0;

	}
	if(key=='5')
	{
		//Gold
		GLfloat ambientColor[3] = {0.3735,0.3735,0.08475};
		GLfloat diffuseColor[3] = {0.0,0.0,0.0};
		GLfloat specularColor[3] = {1.022,0.782,0.344};
		GLfloat lightPosition[3] = {2.0, 1.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 3.0;
	}
	if(key=='6')
	{
		//光源的位置挪远            
		GLfloat ambientColor[3] = {0.3735,0.3735,0.08475};
		GLfloat diffuseColor[3] = {0.0,0.0,0.0};
		GLfloat specularColor[3] = {1.022,0.782,0.344};
		GLfloat lightPosition[3] = {2.0, 3.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 3.0;
	}
	if(key=='7')
	{
		//高光因子增大(光斑变小)
		GLfloat ambientColor[3] = {0.3735,0.3735,0.08475};
		GLfloat diffuseColor[3] = {0.0,0.0,0.0};
		GLfloat specularColor[3] = {1.022,0.782,0.344};
		GLfloat lightPosition[3] = {2.0, 3.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 10.0;
	}
	if(key=='8')
	{
		//半导体
		GLfloat ambientColor[3] = {0.3735,0.3735,0.08475};
		GLfloat diffuseColor[3] = {0.3735,0.3735,0.08475};
		GLfloat specularColor[3] = {1.022,0.782,0.344};
		GLfloat lightPosition[3] = {2.0, 3.0, 2.0};
		memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
		memcpy(vDiffuseColor,diffuseColor,sizeof(vDiffuseColor));
		memcpy(vSpecularColor,specularColor,sizeof(vSpecularColor));
		memcpy(vLightPosition,lightPosition,sizeof(vLightPosition));
		sh = 10.0;
	}
}
void MultiObj::ChangeObj(float x,float y,float lastX, float lastY, int type)
{
	if (type == 0)
	{

		glm::vec3 center = glm::vec3(0.0,0.0,0.0);
		glm::vec3 pointx = glm::vec3(0.25,0.0,0.0);

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::rotate(model, rotateAngle, axisvec);
		view  = glm::translate(view, movevec);
		projection = glm::perspective(45.0f,1.0f, 0.1f, 100.0f);

		glm::vec4 outCenter = projection*view*model*glm::vec4(center,1.0);
		glm::vec4 outPointx =  projection*view*model*glm::vec4(pointx,1.0);

		glm::vec3 outCenter1 = glm::vec3(outCenter)/outCenter.w;
		glm::vec3 outPointx1 = glm::vec3(outPointx)/outPointx.w;	

		float balldis = glm::distance(outCenter1, outPointx1);


		glm::vec3 trianglecenter = glm::vec3(0.0,0.0,0.0);//外接球心
		glm::vec3 trianglepointy = glm::vec3(0.0f, sqrt(6.0f)/4/2,  0);//y轴上的顶点
		float triangler = sqrt(6.0f)/4/2;//外接球半径

		glm::mat4 trianglemodel;
		glm::mat4 triangleview;
		glm::mat4 triangleprojection;

		trianglemodel = glm::rotate(trianglemodel, triangleRotateAngle, triangleaxisvec);		
		triangleview  = glm::translate(triangleview, trianglemovevec);
		triangleprojection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

		glm::mat4 M = triangleprojection*triangleview*trianglemodel;
		glm::vec4 outTriangleCenter = M*glm::vec4(trianglecenter,1.0);
		glm::vec4 outTrianglePointy =  M*glm::vec4(trianglepointy,1.0);

		glm::vec3 outTriangleCenter1 = glm::vec3(outTriangleCenter)/outTriangleCenter.w;
		glm::vec3 outTrianglePointy1 = glm::vec3(outTrianglePointy)/outTrianglePointy.w;	

		float triangledis = glm::distance(outTriangleCenter1, outTrianglePointy1);


		glm::vec3 currPoint = ProjectScreenToNormalize(800, 800, glm::vec2(0,800), glm::vec2(x,y));
		glm::vec3 farcurrPoint = glm::vec3(currPoint.x, currPoint.y, 1.0);
		glm::vec3 nearcurrPoint = glm::vec3(currPoint.x, currPoint.y, -1.0);
		glm::vec3 d = farcurrPoint - nearcurrPoint;
		d = glm::normalize(d);

		glm::vec3 v = farcurrPoint - outCenter1;
		float ddotv = glm::dot(d, v);
		float vdotv = glm::dot(v, v);		
		float ballsq = ddotv * ddotv - (vdotv - balldis*balldis);

		glm::vec3 v1 = farcurrPoint - outTriangleCenter1;
		float ddotv1 = glm::dot(d, v1);
		float v1dotv1 = glm::dot(v1, v1);		
		float trianglesq = ddotv1 * ddotv1 - (v1dotv1 - triangledis*triangledis);

		if (trianglesq > 0)
		{
			selectMode = 1;
			GLfloat goldambientColor[3] = {0.3735,0.3735,0.08475};
			GLfloat ambientColor[3] = {0.2,0.2,0.2};
			memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
			memcpy(trianglevAmbientColor,goldambientColor,sizeof(trianglevAmbientColor));			
		}
		else if (ballsq > 0)
		{
			selectMode = 0;
			GLfloat goldambientColor[3] = {0.3735,0.3735,0.08475};
			GLfloat ambientColor[3] = {0.2,0.2,0.2};
			memcpy(vAmbientColor,goldambientColor,sizeof(vAmbientColor));
			memcpy(trianglevAmbientColor,ambientColor,sizeof(trianglevAmbientColor));			
		}		
		else
		{
			selectMode = -1;
			GLfloat goldambientColor[3] = {0.3735,0.3735,0.08475};
			GLfloat ambientColor[3] = {0.2,0.2,0.2};
			memcpy(vAmbientColor,ambientColor,sizeof(vAmbientColor));
			memcpy(trianglevAmbientColor,ambientColor,sizeof(trianglevAmbientColor));			
		}
	}
	else if (type == 1)
	{
		glm::vec3 lastPoint = ProjectScreenToNormalize(800, 800, glm::vec2(0,800), glm::vec2(lastX,lastY));
		glm::vec3 currPoint = ProjectScreenToNormalize(800, 800, glm::vec2(0,800), glm::vec2(x,y));
		glm::vec3 mousemove = currPoint - lastPoint;
		if (selectMode == 0)
		{
			movevec = 	glm::vec3(lastPoint.x, lastPoint.y, -3.0f) + glm::vec3(mousemove.x, mousemove.y, 0.0f);
		}
		else if (selectMode == 1)
		{
			trianglemovevec = glm::vec3(lastPoint.x, lastPoint.y, -3.0f) + glm::vec3(mousemove.x, mousemove.y, 0.0f);
		}		
	}
	else if (type == 2)
	{
		float dangle = 0.0;
		glm::vec3 axis;
		MouseMove2TrackBallRotation(glm::vec2(lastX,lastY), 
			glm::vec2(x,y),
			glm::vec2(0,800), 
			800, 
			800,dangle,axis);
		if (selectMode == 0)
		{			
			rotateAngle += dangle;		
			axisvec = axis;
		}
		else if (selectMode == 1)
		{
			triangleRotateAngle += dangle;
			triangleaxisvec = axis;
		}		
	}
}

void MultiObj::MouseMove2TrackBallRotation(glm::vec2 prePosition2D,
	glm::vec2 currPosition2D,
	glm::vec2  BottomLeftCorner, 
	double width, 
	double height,float &dangle, glm::vec3& outaxisvec)
{
	if ((prePosition2D.x > BottomLeftCorner.x)&&
		(prePosition2D.x < BottomLeftCorner.x+width)&&
		(prePosition2D.y > BottomLeftCorner.y-height)&&
		(prePosition2D.y < BottomLeftCorner.y))
	{
		glm::vec3 prePosition3D = ProjectScreenToNormalize(width, height, BottomLeftCorner, prePosition2D);
		glm::vec3 currPosition3D = ProjectScreenToNormalize(width, height, BottomLeftCorner, currPosition2D);

		CalculateTrackBallRotation(prePosition3D, currPosition3D, glm::vec3(0.0,0.0,0.0), dangle, outaxisvec);
	}
}

glm::vec3 MultiObj::ProjectScreenToNormalize(double width, 
	double height, 
	glm::vec2 BottomLeftCorner, 
	glm::vec2 point)
{
	double x = (point.x - BottomLeftCorner.x) / (width / 2) - 1;
	double y = (BottomLeftCorner.y - point.y) / (height / 2) - 1;

	double z2 = 1.0 - x * x - y * y;
	double z = z2>0.5 ? sqrt(z2): 0.5 / sqrt(x * x + y * y);
	glm::vec3 pt(x,y,z);
	return pt;
}

void MultiObj::CalculateTrackBallRotation(const glm::vec3& prePosition3D, 
	const glm::vec3& currPosition3D,
	const glm::vec3& centerPoint3D,
	float &dangle,
	glm::vec3& outaxisvec)
{
	glm::vec3 currVector3D = currPosition3D - centerPoint3D;
	glm::vec3 preVector3D = prePosition3D - centerPoint3D;
	glm::vec3 axis = glm::cross(currVector3D, preVector3D);
	double angle = glm::dot(currVector3D,preVector3D);
	float arccos = glm::acos(angle);
	dangle = 180*arccos/PI;
	outaxisvec = axis;
}

MultiObj::~MultiObj(void)
{
}
