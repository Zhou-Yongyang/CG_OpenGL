#pragma once
#include <string>
#include <glm/glm.hpp>	//glm «œÚ¡ø°¢æÿ’Ûø‚
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "InitFunc.h"
using namespace std;
using namespace glm;

class Material
{
public:
	static const int COLOR = 0, AO = 1, NORMAL = 2, ROUGH = 3, METALLIC = 4;
private:
	string path;
	string name;
	vec3 f0;
	unsigned int aoTex, colorTex, normalTex, roughTex, metallicTex;
public:
	Material(string path, string name, vec3 fi)
	{
		this->path = path;
		this->name = name;
		f0 = fi;
	}
	void loadMaterial()
	{
		
		colorTex = loadTexture((path + name +  "_Color.jpg").c_str());
		normalTex = loadTexture((path + name + "_Normal.jpg").c_str());
		roughTex = loadTexture((path + name + "_Roughness.jpg").c_str());
		aoTex = loadTexture((path + name + "_AmbientOcclusion.jpg").c_str());
		metallicTex = loadTexture((path + name + "_Metalness.jpg").c_str());
	}
	void setTexture(string subPath, int type)
	{
		if (type == COLOR)
			colorTex = loadTexture(subPath.c_str());
		else if (type == AO)
			aoTex = loadTexture(subPath.c_str());
		else if (type == NORMAL)
			normalTex = loadTexture(subPath.c_str());
		else if (type == ROUGH)
			roughTex = loadTexture(subPath.c_str());
		else if (type == METALLIC)
			metallicTex = loadTexture(subPath.c_str());
	}
	void shaderSet(Shader& shader, int start)
	{
		shader.use();
		if (colorTex != 0)
			shader.setInt("albedoMap", start + 0);
		if(normalTex != 0)
			shader.setInt("normalMap", start + 1);
		if(roughTex != 0)
			shader.setInt("roughnessMap", start + 2);
		if(aoTex != 0)
			shader.setInt("aoMap", start + 3);
		if(metallicTex != 0)
			shader.setInt("metallicMap", start + 4);
	}
	void activeMaterial(Shader& shader, int start)
	{
		shader.setVec3("f0", f0);
		if (colorTex != 0) {
			glActiveTexture(GL_TEXTURE0 + start);
			glBindTexture(GL_TEXTURE_2D, colorTex);
		}
		if (normalTex != 0) {
			glActiveTexture(GL_TEXTURE1 + start);
			glBindTexture(GL_TEXTURE_2D, normalTex);
		}
		if (roughTex != 0) {
			glActiveTexture(GL_TEXTURE2 + start);
			glBindTexture(GL_TEXTURE_2D, roughTex);
		}
		if (aoTex != 0) {
			glActiveTexture(GL_TEXTURE3 + start);
			glBindTexture(GL_TEXTURE_2D, aoTex);
		}
		if (metallicTex != 0) {
			glActiveTexture(GL_TEXTURE4 + start);
			glBindTexture(GL_TEXTURE_2D, metallicTex);
		}
	}
};