#pragma once
//�˴���LightΪ����Դ��֮�������һ�����Դ��һ��ƽ�й�Դ
#include <toolLib/Shader.h>
#include "InitFunc.h"
#include "Sphere.h"
#include <string>
#include <glm/glm.hpp>	//glm�������������
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Light
{
public:
	string shader_name;
	Sphere lightSource;
	glm::vec3 position;	//��Դλ�ã��������ֹ�Դ��������һ���ԡ�
	float intensity;		//��Դǿ��
	glm::vec3 color;				//��Դ��ɫ
	float scale;				//���ƴ�С
public:
	void renderLight(Shader& shader); 	//��Դ����
	void setUniform(Shader& shader);
};

//ƽ�й�
class SkyLight :virtual public Light
{
public:
	glm::vec3 dir;
	SkyLight(string name, glm::vec3 pos, glm::vec3 direction, float INTENSITY = 1, float SCALE = 1, glm::vec3 COLOR = glm::vec3(1, 1, 1))
	{
		shader_name = name;
		lightSource.InitSpereData();
		position = pos;
		dir = direction;
		intensity = INTENSITY;
		this->scale = SCALE;
		color = COLOR;
	}
	void setUniform(Shader& shader);
};

class PointLight : virtual public Light
{
public:
	PointLight(string name, glm::vec3 pos, float INTENSITY = 1, float SCALE = 1, glm::vec3 COLOR = glm::vec3(1, 1, 1))
	{
		shader_name = name;
		lightSource.InitSpereData();
		position = pos;
		intensity = INTENSITY;
		this->scale = SCALE;
		color = COLOR;
	}
	void setUniform(Shader& shader);
};