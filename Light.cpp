#include "Light.h"
#include <toolLib/Shader.h>
#include "InitFunc.h"
#include "Sphere.h"

#include <glm/glm.hpp>	//glm «œÚ¡ø°¢æÿ’Ûø‚
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Light::renderLight (Shader& shader)
{
	glm::vec3 pos = position;
	cameraSet(shader);
	//model = mat4(1);
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::translate(model, pos * 1.0f / scale);
	shader.setMat4("model", model, false);
	shader.setVec3("color", color);
	lightSource.renderSphere();
}

void SkyLight::setUniform(Shader& tmpShader)
{
	string s_intensity = shader_name + ".intensity";
	tmpShader.setFloat(s_intensity.c_str(), intensity);

	string s_dir = shader_name + ".dir";
	tmpShader.setVec3(s_dir.c_str(), dir);

	string s_color = shader_name + ".color";
	tmpShader.setVec3(s_color.c_str(), color);

	/*
	string s_pos = shader_name + ".pos";
	tmpShader.setVec3(s_pos.c_str(), position);
	*/
}


void PointLight::setUniform(Shader& tmpShader)
{
	string s_intensity = shader_name + ".intensity";
	tmpShader.setFloat(s_intensity.c_str(), intensity);

	string s_color = shader_name + ".color";
	tmpShader.setVec3(s_color.c_str(), color);

	
	string s_pos = shader_name + ".pos";
	tmpShader.setVec3(s_pos.c_str(), position);
}
