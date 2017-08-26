#pragma once
#include "renderobjectbase.h"

class MultiObj
{
public:
	MultiObj(void);
	~MultiObj(void);

	float* verts;
	int vertNum;
	int slice ;
	int statck ;
	float r ;
		
	char* trianglevertStr;
	char* triangleflagStr;

	GLuint triangleVAO,triangleVBO,triangleEBO;
	GLuint triangleshader_program;

	
	char* vertStr;
	char* flagStr;

	GLuint VAO[2],VBO[2];
	GLuint shader_program;

	GLfloat vDiffuseColor[3];
	GLfloat vAmbientColor[3];
	GLfloat vSpecularColor[3];
	GLfloat vLightPosition[3];
	GLfloat sh;
	
	glm::mat4 transform;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	 
	glm::mat4 trianglemodel;
	glm::mat4 triangleview;
	glm::mat4 triangleprojection;

	//选中颜色
	GLfloat trianglevAmbientColor[3];
	//选中对象
	int selectMode;
	//平移向量
	glm::vec3 movevec;
	glm::vec3 trianglemovevec;
	//旋转轴和角度，四元数
	float rotateAngle;
	glm::vec3 axisvec;
	float triangleRotateAngle;
	glm::vec3 triangleaxisvec;
	

	void Init(void);
	void Display(void);
	void ChangeLight(unsigned char);
	void ChangeObj(float x,float y,float lastX, float lastY, int type = -1);

	void MouseMove2TrackBallRotation(glm::vec2 prePosition2D,
		glm::vec2 currPosition2D,
		glm::vec2 BottomLeftCorner, 
		double width, 
		double height,float &dangle, glm::vec3& outaxisvec);

	glm::vec3 MultiObj::ProjectScreenToNormalize(double width, 
		double height, 
		glm::vec2 BottomLeftCorner, 
		glm::vec2 point);

	void MultiObj::CalculateTrackBallRotation(const glm::vec3& prePosition3D, 
		const glm::vec3& currPosition3D,
		const glm::vec3& centerPoint3D, 
		float &dangle, glm::vec3& outaxisvec);
};
