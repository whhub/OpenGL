#include "RenderBall.h"

#define PI 3.14159
    
RenderBall::RenderBall(void)
{
    r = 0.6;
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
    sh = 3.0;
}

void RenderBall::Init()
{
    vertNum=slice*statck*4;//��������,���ı������
	verts=new float[vertNum*3];//ÿ��������xyz��������,���*3

	float stepAngZ=PI/slice;//����Ƕ�ÿ�����ӵ�ֵ(0��~180��)
	float stepAngXY=(2*PI)/statck;//����Ƕ�ÿ�����ӵ�ֵ(0��~360��)
	float angZ=0.0;//��ʼ������Ƕ�
	float angXY=0.0;//��ʼ�ĺ���Ƕ�

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
		angXY=0.0;//ÿ�κ��򵽴�2PI�Ƕ������Ƕȹ�0
		angZ+=stepAngZ;
	}

	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertNum*3, verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); 
        
	shader_program = LoadShader(vertStr,flagStr);	
	glUseProgram(shader_program);
}

void RenderBall::Display()
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

    model = glm::rotate(model, rotateAngle, glm::vec3(1.0f, 0.0f, 1.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
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
	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, vertNum);  
	glBindVertexArray(0);
	//glFlush();
	glutSwapBuffers(); 
}

void RenderBall::ChangeLight(unsigned char key)
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
            //��������ǿ
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
            //�������ǿ
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
            //��Ե��
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
            //���淴�����ǿ
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
            //��Դ��λ��ŲԶ            
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
            //�߹���������(��߱�С)
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
            //�뵼��
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

RenderBall::~RenderBall(void)
{
}
