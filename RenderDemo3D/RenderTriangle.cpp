#include "RenderTriangle.h"



RenderTriangle::RenderTriangle()
{
	rotateAngle=-100;
}

void RenderTriangle::Init()
{
	
	glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
	glGenBuffers(1, &EBO);
 
	float vertices[]={ 
	0.5f,-sqrt(6.0f)/12,-sqrt(3.0f)/6,0.5,0,0,
	-0.5f,-sqrt(6.0f)/12,-sqrt(3.0f)/6,0,0.5,0,
	0.0f,-sqrt(6.0f)/12, sqrt(3.0f)/3,0,0,0.5,
	0.0f, sqrt(6.0f)/4,  0,0.5,0.5,0
	};	  

	unsigned int indices[] = {  // note that we start from 0!
        1, 0, 3,  // first Triangle
        0,1, 2, // second Triangle
		 0,2, 3,
		 2,1,3
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,6*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT,  GL_FALSE,6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
	
	
	shader_program = LoadShader(vertStr,flagStr);	
	glUseProgram(shader_program);
}

void RenderTriangle::Display()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );
	 // update shader uniform
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t= localtime(&tt);
	float timeValue = t->tm_sec;	
	float delta = fabs(sin(timeValue) / 2.0f);	
	int vertexColor = glGetUniformLocation(shader_program, "colorZ");
	glUniform1f(vertexColor,delta);

	glm::mat4 transform;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    model = glm::rotate(model,rotateAngle, glm::vec3(1.0f, 0.0f, 1.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
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


	glEnable(GL_DEPTH_TEST);  
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	//glFlush();
	glutSwapBuffers(); 
}


RenderTriangle::~RenderTriangle(void)
{
}
