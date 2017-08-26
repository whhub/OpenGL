#include "RenderBall.h"

#define PI 3.14159

	GLuint* vboId;
	GLuint vert,texcoord;
	float* verts;//���涥���뷨������ָ��
	GLfloat* texcoords;//�������������ָ��
	int vertNum;
    int slice ;
    int statck ;
    float r ;

RenderBall::RenderBall(void)
{
    r = 0.5;
    slice = 100;
    statck = 100;
}

void RenderBall::Init()
{
    vertNum=slice*statck*4;//��������,���ı������
	verts=new float[vertNum*3];//ÿ��������xyz��������,���*3
	texcoords=new GLfloat[vertNum*2];//ÿ�����������������uv��������,���*2

	float stepAngZ=PI/slice;//����Ƕ�ÿ�����ӵ�ֵ(0��~180��)
	float stepAngXY=(2*PI)/statck;//����Ƕ�ÿ�����ӵ�ֵ(0��~360��)
	float angZ=0.0;//��ʼ������Ƕ�
	float angXY=0.0;//��ʼ�ĺ���Ƕ�

	int index=0;
	int indexTex=0;
	for(int i=0;i<slice;i++) 
    {
		for(int j=0;j<statck;j++) 
        {
            //����һ������
            float x1=r*sin(angZ)*cos(angXY);
            float y1=r*sin(angZ)*sin(angXY);
			float z1=r*cos(angZ);
			verts[index]=x1; index++;
			verts[index]=y1; index++;
			verts[index]=z1; index++;
			/*float v1=angZ/PI;
			float u1=angXY/(2*PI);
			texcoords[indexTex]=u1; indexTex++;
			texcoords[indexTex]=v1; indexTex++;*/

			float x2=r*sin(angZ+stepAngZ)*cos(angXY);
			float y2=r*sin(angZ+stepAngZ)*sin(angXY);
			float z2=r*cos(angZ+stepAngZ);
			verts[index]=x2; index++;
			verts[index]=y2; index++;
			verts[index]=z2; index++;
			/*float v2=(angZ+stepAngZ)/PI;
			float u2=angXY/(2*PI);
			texcoords[indexTex]=u2; indexTex++;
			texcoords[indexTex]=v2; indexTex++;*/
            
			float x3=r*sin(angZ+stepAngZ)*cos(angXY+stepAngXY);
			float y3=r*sin(angZ+stepAngZ)*sin(angXY+stepAngXY);
			float z3=r*cos(angZ+stepAngZ);
			verts[index]=x3; index++;
			verts[index]=y3; index++;
			verts[index]=z3; index++;
		/*	float v3=(angZ+stepAngZ)/PI;
			float u3=(angXY+stepAngXY)/(2*PI);
			texcoords[indexTex]=u3; indexTex++;
			texcoords[indexTex]=v3; indexTex++;*/

			float x4=r*sin(angZ)*cos(angXY+stepAngXY);
			float y4=r*sin(angZ)*sin(angXY+stepAngXY);
			float z4=r*cos(angZ);
			verts[index]=x4; index++;
			verts[index]=y4; index++;
			verts[index]=z4; index++;
		/*	float v4=angZ/PI;
			float u4=(angXY+stepAngXY)/(2*PI);
			texcoords[indexTex]=u4; indexTex++;
			texcoords[indexTex]=v4; indexTex++;*/

			angXY+=stepAngXY;
        }
		angXY=0.0;//ÿ�κ��򵽴�2PI�Ƕ������Ƕȹ�0
		angZ+=stepAngZ;
	}
    
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertNum, verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,4*sizeof(float), (void*)0);	
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); 
        
	shader_program = LoadShader(vertStr,flagStr);	
	glUseProgram(shader_program);
}

void RenderBall::Display()
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
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.1f));
    projection = glm::perspective(glm::radians(45.0f),1.0f, 0.1f, 100.0f);
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
    
	glBindVertexArray(VAO);
	glDrawArrays(GL_QUAD_STRIP, 0, vertNum);  
	glBindVertexArray(0);
	//glFlush();
	glutSwapBuffers(); 
}


RenderBall::~RenderBall(void)
{
}
