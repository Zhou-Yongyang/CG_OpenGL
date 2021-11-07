#pragma once
//此处的Light为基光源，之后会派生一个点光源，一个平行光源
#include <toolLib/Shader.h>
#include "InitFunc.h"
#include "Sphere.h"
#include <string>
#include <glm/glm.hpp>	//glm是向量、矩阵库
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Light
{
public:
	string shader_name;
	Sphere lightSource;
	glm::vec3 position;	//光源位置，对于两种光源均存在这一属性。
	float intensity;		//光源强度
	glm::vec3 color;				//光源颜色
	float scale;				//绘制大小
public:
	void renderLight(Shader& shader); 	//光源绘制
	void setUniform(Shader& shader);
};

//平行光
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