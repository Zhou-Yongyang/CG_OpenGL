#pragma once
#include <iostream>
#include <vector>
using namespace std;
class Sphere
{
private:
	unsigned int sphereVAO = 0;
	unsigned int indexCount = 0;
	std::vector<float> data;
	std::vector<unsigned int> indices;
public:
	void InitSpereData();
	void renderSphere();
};
