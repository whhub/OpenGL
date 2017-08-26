#include "RenderCone.h"

#define PI 3.14159

RenderCone::RenderCone(void)
{
}

int vCountFan=0;
int vCountCircle=0;

void RenderCone::Init()
{
	Cone();
	
}

void RenderCone::Cone()
{ 	

	int divAngle=10;
	int n= 360/divAngle ;
	float *verticesFan= new float[n*3*10];
	float *verticesCircle= new float[n*3*10];	 
	
	int i=0;
	int j=0;
	
	//ªÊ÷∆≤‡√Ê
	verticesFan[i++]=0;
	verticesFan[i++]=0;
	verticesFan[i++]=0.75;

	for(float angle=0;angle<=360 ;angle+=divAngle)
	{ 		
		float rad=angle*PI/180;

		verticesFan[i++]=0.5*sin(rad);		
		verticesFan[i++]=0.5*cos(rad);
		verticesFan[i++]=0;
	}
	vCountFan=i;

   //ªÊ÷∆µ◊≈Ã
	verticesCircle[j++]=0;
	verticesCircle[j++]=0;
	verticesCircle[j++]=0;

	for(float angle=0;angle<=360 ;angle+=divAngle)
	{ 		
		float rad=angle*PI/180;

		verticesCircle[j++]=0.5*sin(rad);		
		verticesCircle[j++]=0.5*cos(rad);
		verticesCircle[j++]=0;
	}
	
	vCountCircle=j;	

	glGenVertexArrays(2,VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vCountFan, verticesFan, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3*sizeof(float), (void*)0);	
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); 

	glBindVertexArray(VAO[1]);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vCountCircle, verticesCircle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, (void*)0);	
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); 

	shader_program = LoadShader(vertStr,flagStr);	
	glUseProgram(shader_program);
	
}

void RenderCone::Display()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);  
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );
	
	time_t tt = time(NULL);
	tm* t= localtime(&tt);
	float timeValue = t->tm_sec;	
	float delta = fabs(sin(timeValue) / 2.0f);	
	int vertexColor = glGetUniformLocation(shader_program, "colorZ");
	glUniform1f(vertexColor,delta);  // update shader uniform

	glm::mat4 transform;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    model = glm::rotate(model,rotateAngle, glm::vec3(1.0f, 0.0f, 1.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(45.0f,1.0f, 0.1f, 100.0f);
	//transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5)); 
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
    unsigned int viewLoc  = glGetUniformLocation(shader_program, "view");
	unsigned int porjectionLoc= glGetUniformLocation(shader_program, "projection");
  

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(porjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
  //  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)); 

	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vCountFan);  
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vCountCircle);  
	glBindVertexArray(0);
	//glFlush();
	glutSwapBuffers(); 
}


RenderCone::~RenderCone(void)
{
}
