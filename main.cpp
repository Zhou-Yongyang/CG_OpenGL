#include <glad/glad.h>	// glad �Լ� glfw �ǵ���opengl�Ŀ�
#include <GLFW/glfw3.h>
#include <iostream>	
#include <glm/glm.hpp>	//glm�������������
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <toolLib/Camera.h>		//���������
#include <toolLib/Model.h>		//ģ������
#include <toolLib/Shader.h>		//��ɫ��
#include <toolLib/stb_image.h>	//��������

//������/���������
#include "InitFunc.h"
#include "Light.h"
#include "Material.h"
using namespace glm;
using namespace std;


Material* materialList;

int work_1();	// normal show and texture show
int work_2();	// phong and blin-phong rendering
int work_3(); // PBR rendering
int work_4(int mi,int t);	// IBL + PBR
void renderSphere();

int main()
{

	Material m1("resources/textures/work_4/fabric/", "Fabric030_2K", vec3(0.02));
	Material m2("resources/textures/work_4/Concrete/", "Concrete036_2K", vec3(0.04));
	Material m5("resources/textures/work_4/gold/", "Metal034_2K", vec3(1.00, 0.86, 0.57));
	Material m6("resources/textures/work_4/marble/", "Marble009_2K", vec3(0.31, 0.31, 0.31));
	Material m4("resources/textures/work_4/metal/", "Metal012_4K", vec3(0.9, 0.8, 0.9));
	Material m3("resources/textures/work_4/wood/", "WoodFloor044_2K", vec3(0.13, 0.20, 0.40));
	materialList = new Material[6]{m1,m2,m3,m4,m5,m6};



	cout << "ѡ����Ŀ��\n1��ģ��չʾ" << endl << "2����ͨ����Ч��չʾ\n" << "3��PBR��ȾЧ��չʾ\n" << "4��PBR + IBL��ȾЧ��չʾ������Ч����" << endl;
	cout << "\n\n" << endl;
	cout << "����w a s d �������ӽ�, ���ַ���" << endl;
	int type = 0;
	cin >> type;
	if (type == 1)
	{
		work_1();
	}
	else if (type == 2)
	{
		work_2();
	}
	else if (type == 3)
	{

		work_3();
	}
	else if (type == 4)
	{
		cout << "���ʱ��(1 - 6 �ֱ�Ϊ��֯���� ˮ��أ� ľͷ�� ���� �� ����ʯ)" << endl;
		int mi = 0;
		cin >> mi;
		cout << "ԭ��ͼ(����0) or ����(����1)" << endl;
		int t = 0;
		cin >> t;
		work_4(mi, t);
	}
	delete[] materialList;
}

int work_1()
{
	// ����ģ�ͣ���չʾģ�͵����巨��Ч���Լ���ͼЧ����
	//��ʼ�����ô���
	GLFWwindow* window = InitSet();

	//���Ի��趨

	//���Ի��趨����

	//�����趨
	unsigned int texture1;
	texture1 = loadTexture("resources/textures/work_1/spot_texture.png");
	//�����趨����

	//shader����
	Shader shader_1("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/normal_fragShader.frag");		//������ɫ����
	Shader shader_2("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/texture_fragShader.frag");	//��ͼ��ɫ����
	Shader lightShader("resources/shader/light/lightVertex.vert", "resources/shader/light/lightFrag.frag");
	//shader�������

	//model����
	Model model_1("resources/models/spot_triangulated_good.obj");
	//model�������

	//�����趨
	shader_1.use();
	shader_1.setInt("texture_diffuse1", 0);

	while (!glfwWindowShouldClose(window))
	{
		//ʱ���¼����
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//ʱ���������

		//������
		processInput(window);

		//����ˢ������
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//���������
		cameraSet(shader_1);

		//�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		mat4 model;
		model = scale(model, vec3(1, 1, 1));
		float angle = 180.0;
		model = rotate(model, radians(angle), vec3(0.0f, 1.0f, 0.0f));
		//��������
		shader_1.setMat4("model", model, false);
		model_1.Draw(shader_1);

		//���潻��
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//����
	glfwTerminate();
	return 0;
}

int work_2()
{
	//��ӻ������� 3�飨1��ƽ�й�+ƽ�й⡣ 2��ƽ�й�+���Դ�� 3��ƽ�й�+�۹�ƹ�Դ��
	// ����ģ�ͣ���չʾģ�͵����巨��Ч���Լ���ͼЧ����
	//��ʼ�����ô���
	GLFWwindow* window = InitSet();

	//���Ի��趨

	//���Ի��趨����

	//�����趨
	unsigned int texture1;
	texture1 = loadTexture("resources/textures/work_1/spot_texture.png");
	//�����趨����

	//shader����
	Shader shader_1("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/normal_fragShader.frag");		//������ɫ����
	Shader shader_2("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/texture_fragShader.frag");	//��ͼ��ɫ����
	Shader lightShader("resources/shader/light/lightVertex.vert", "resources/shader/light/lightFrag.frag");
	//shader�������

	//model����
	Model model_1("resources/models/spot_triangulated_good.obj");
	SkyLight skyLight_1("SkyLight[0]", vec3(0,8,10), vec3(0,-8,-10), 0.5);
	SkyLight skyLight_2("SkyLight[1]", vec3(-8, 7, -4), vec3(8, -7, 4), 0.5, 0.5f, vec3(1.0, 1.0, 1.0));

	PointLight pointLight_1("PointLight[0]", vec3(0, 2, 2), 4, 0.2, vec3(1.0,0.7,0.4));
	PointLight pointLight_2("PointLight[1]", vec3(0, 1, -2), 4, 0.2, vec3(0.8, 0.6, 0.4));
	//model�������

	//�����趨
	shader_2.use();
	shader_2.setInt("texture_diffuse1", 0);

	while (!glfwWindowShouldClose(window))
	{
		//ʱ���¼����
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//ʱ���������

		//������
		processInput(window);

		//����ˢ������
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//���������
		cameraSet(shader_2);

		//�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		mat4 model;
		model = scale(model, vec3(1, 1, 1));
		float angle = 180.0;
		model = rotate(model, radians(angle), vec3(0.0f, 1.0f, 0.0f));
		//��������
		//shader_1.setMat4("model", model, false);
		//model_1.Draw(shader_1);
		//��ͼ����
		shader_2.setMat4("model", model, false);
		//glsl��������
		skyLight_1.setUniform(shader_2);
		skyLight_2.setUniform(shader_2);

		pointLight_1.position.x = 2 * cos(glfwGetTime());
		pointLight_1.position.y = 1;
		pointLight_1.position.z = 2 * sin(glfwGetTime());
		pointLight_1.setUniform(shader_2);
		pointLight_2.setUniform(shader_2);



		shader_2.setVec3("viewPos", camera.Position);

		model_1.Draw(shader_2);

		skyLight_1.renderLight(lightShader);
		skyLight_2.renderLight(lightShader);
		pointLight_1.renderLight(lightShader);
		pointLight_2.renderLight(lightShader);
		
		//���潻��
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//����
	glfwTerminate();
	return 0;
}

int work_3()
{
	//PBR��Ⱦ����ӷ��ն���ͼ������PBR shader��д
	GLFWwindow* window = InitSet();

	// ģ������
	Model model_1("resources/models/spot_triangulated_good.obj");

	//shader����
	Shader shader("resources/shader/work_3/vertexShader.vert", "resources/shader/work_3/fragShader.frag");	//������ɫ����
	Shader lightShader("resources/shader/light/lightVertex.vert", "resources/shader/light/lightFrag.frag");

	//��ͼ����
	unsigned int aoTex, colorTex, normalTex, roughTex, metallicTex;
	/*
	vec3 v = vec3(0.02);
	Material m1("resources/textures/work_4/fabric/", "Fabric030_2K", vec3(0.02));
	Material m2("resources/textures/work_4/Concrete/", "Concrete036_2K", vec3(0.04));
	Material m5("resources/textures/work_4/gold/", "Metal034_2K", vec3(1.00, 0.86, 0.57));
	Material m6("resources/textures/work_4/marble/", "Marble009_2K", vec3(0.31, 0.31, 0.31));
	Material m4("resources/textures/work_4/metal/", "Metal012_4K", vec3(0.9, 0.8, 0.9));
	Material m3("resources/textures/work_4/wood/", "WoodFloor044_2K", vec3(0.13, 0.20, 0.40));


	Material materialList[6] = { m1, m2, m3, m4, m5, m6};
	*/
	cout << "���ز���" << endl;
	Material subList[6] = { materialList[0], materialList[1], materialList[2], materialList[3], materialList[4], materialList[5] };
	for (int i = 0; i < 6; i++)
	{
		materialList[i].loadMaterial();
		materialList[i].setTexture("resources/textures/work_1/spot_texture.png", Material::COLOR);
		subList[i].loadMaterial();
	}
	cout << "�������" << endl;

	//��ͼ�������
	SkyLight skyLight_1("skyLight[0]", vec3(0, 10, 0), vec3(0, -1, 0), 20, 0.4);
	SkyLight skyLight_2("skyLight[1]", vec3(8, 6, 6), vec3(-8, -6, -6), 20, 0.4);

	//PointLight pointLight_1("pointLight[0]", vec3(0,0,0), 5, 0.3);
	PointLight pointLight_2("pointLight[1]", vec3(5, 0, 0), 15, 0.3);
	PointLight pointLight_3("pointLight[2]", vec3(0, 5, 0), 15, 0.3);
	PointLight pointLight_4("pointLight[3]", vec3(0, 0, 5), 15, 0.3);


	while (!glfwWindowShouldClose(window))
	{
		//ʱ���¼����
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//ʱ���������

		//������
		processInput(window);

		//����ˢ������
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//���������
		cameraSet(shader);
		
		

		shader.setVec3("viewPos", camera.Position);
		for (int i = 0; i < 6; i++)
		{
			mat4 model;
			model = scale(model, vec3(1, 1, 1));
			float angle = 180.0;
			model = translate(model, vec3(-3 + 1.5*i,0,-4));
			model = rotate(model, radians(angle), vec3(0.0f, 1.0f, 0.0f));

			shader.setMat4("model", model, false);

			materialList[i].shaderSet(shader, 0);
			materialList[i].activeMaterial(shader, 0);

			model_1.Draw(shader);
		}


		for (int i = 0; i < 6; i++)
		{
			mat4 model;
			model = scale(model, vec3(1, 1, 1));
			float angle = 180.0;
			model = translate(model, vec3(-3 + 1.5 * i, 0, -8));
			model = rotate(model, radians(angle), vec3(0.0f, 1.0f, 0.0f));

			shader.setMat4("model", model, false);

			subList[i].shaderSet(shader, 0);
			subList[i].activeMaterial(shader, 0);

			model_1.Draw(shader);
		}


		//pointLight_1.position.x = 2 * sin(glfwGetTime());
		//pointLight_1.position.y = 2 * cos(glfwGetTime());
		//pointLight_1.setUniform(shader);
		pointLight_2.setUniform(shader);
		pointLight_3.setUniform(shader);
		pointLight_4.setUniform(shader);

		skyLight_1.dir.x = 2*sin(glfwGetTime());
		skyLight_1.dir.y = 2*cos(glfwGetTime());
		skyLight_1.dir.z = 2 * sin(glfwGetTime())*cos(glfwGetTime());

		skyLight_1.setUniform(shader);
		skyLight_2.setUniform(shader);
		



		//pointLight_1.renderLight(lightShader);
		//pointLight_2.renderLight(lightShader);
		//pointLight_3.renderLight(lightShader);
		//pointLight_4.renderLight(lightShader);
		skyLight_1.renderLight(lightShader);
		skyLight_2.renderLight(lightShader);


		//���潻��
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//����
	glfwTerminate();
	return 0;
}

int work_4(int mi, int t)
{
	mi = mi - 1;
	//PBR��Ⱦ����ӷ��ն���ͼ������PBR shader��д
	GLFWwindow* window = InitSet();

	// ģ������
	Model model_1("resources/models/spot_triangulated_good.obj");

	//shader����
	Shader pbrShader("resources/shader/work_4/vertexShader.vert", "resources/shader/work_4/fragShader.frag");	//������ɫ����
	Shader lightShader("resources/shader/light/lightVertex.vert", "resources/shader/light/lightFrag.frag");
	Shader equirectangularToCubemapShader("resources/shader/work_4/cubeMap.vert", "resources/shader/work_4/equiretanguler_to_cubemap.frag");
	Shader irradianceShader("resources/shader/work_4/cubeMap.vert", "resources/shader/work_4/irradiance_convolution.frag");
	Shader backgroundShader("resources/shader/work_4/background.vert", "resources/shader/work_4/background.frag");
	Shader prefilterShader("resources/shader/work_4/cubeMap.vert", "resources/shader/work_4/prefilter.frag");
	Shader brdfShader("resources/shader/work_4/brdf.vert", "resources/shader/work_4/brdf.frag");

	
	pbrShader.use();
	pbrShader.setInt("irradianceMap", 0);
	pbrShader.setInt("prefilterMap", 1);
	pbrShader.setInt("brdfLUT", 2);
	
	backgroundShader.use();
	backgroundShader.setInt("environmentMap", 0);

	//shader�������

	
	//pbr: ����֡����
	unsigned int captureFBO = 0, captureRBO = 0;
	setFrameBuffer(captureFBO, captureRBO);

	//hdr��ͼ����
	unsigned hdrTexture = loadHDRenvMap("resources/textures/work_4/HDR/fireplace_4k.hdr");
	
	//������������ͼ����
	unsigned int envCubemap = setCubeMapFrameBuffer();
	
	//��������ͼת��
	equirectangularToCubemapShader.use();
	equirectangularToCubemapShader.setInt("equirectangularMap", 0);
	equirectangularToCubemapShader.setMat4("projection", captureProjection, false);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glViewport(0, 0, 512, 512); 
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	unsigned int cubeVAO = 0, cubeVBO = 0;
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.setMat4("view", captureViews[i], false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube(cubeVAO, cubeVBO);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//�������ն���ͼ

	unsigned int irradianceMap = setIrradianceMap(captureFBO,captureRBO);
	//��������
	unsigned int aoTex, colorTex, normalTex, roughTex, metallicTex;


	//���ն�shader����
	irradianceShader.use();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4("projection", captureProjection, false);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader.setMat4("view", captureViews[i], false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube(cubeVAO, cubeVBO);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//���

	//Ԥ�ȹ���������
	unsigned int prefilterMap = setPreFilterMap();
	
	//���ؿ������ģ��
	prefilterShader.use();
	prefilterShader.setInt("environmentMap", 0);
	prefilterShader.setMat4("projection", captureProjection, false);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterShader.setMat4("view", captureViews[i], false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube(cubeVAO, cubeVBO);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//
	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;
	renderQuad(quadVAO, quadVBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//ȫ�ֱ�����ʼ��
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//pbrShader.use();
	//pbrShader.setMat4("projection", projection, false);
	backgroundShader.use();
	backgroundShader.setMat4("projection", projection, false);
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);



		materialList[mi].loadMaterial();
		if(t == 0)
			materialList[mi].setTexture("resources/textures/work_1/spot_texture.png", Material::COLOR);

	//��ͼ�������
	//�Զ����Դ
	SkyLight skyLight_1("skyLight[0]", vec3(0,0,-10), vec3(0, 0, 10), 4, 0.4);
	SkyLight skyLight_2("skyLight[1]", vec3(0, 1, -10), vec3(0, -1, 10), 3, 0.4);
	SkyLight skyLight_3("skyLight[2]", vec3(0, -1, -10), vec3(0, 1, 10), 2, 0.4);


	PointLight pointLight_1("pointLight[0]", vec3(2, -3, 2), 40);
	PointLight pointLight_2("pointLight[1]", vec3(2.2, -3.1, 1.9), 10);
	PointLight pointLight_3("pointLight[2]", vec3(1.9, -2.9, 2.1), 10);
	PointLight pointLight_4("pointLight[3]", vec3(10, -10, 10), 0);


	while (!glfwWindowShouldClose(window))
	{
		//ʱ���¼����
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//ʱ���������

		//������
		processInput(window);

		//����ˢ������
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//���������
		cameraSet(pbrShader);
		pbrShader.setVec3("viewPos", camera.Position);

		//��IBL����
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		



		materialList[mi].shaderSet(pbrShader, 3);
		materialList[mi].activeMaterial(pbrShader, 3);
		mat4 model;
		model = scale(model, vec3(1));
		float angle = 180.0;
		model = translate(model, vec3(0,0,-5));
		model = rotate(model, -radians(angle), vec3(0.0f, 1.0f, 0.0f));

		pbrShader.setMat4("model", model, false);
		model_1.Draw(pbrShader);

	
		skyLight_1.setUniform(pbrShader);
		skyLight_2.setUniform(pbrShader);
		skyLight_3.setUniform(pbrShader);

		pointLight_1.setUniform(pbrShader);
		pointLight_2.setUniform(pbrShader);
		pointLight_3.setUniform(pbrShader);
		pointLight_4.setUniform(pbrShader);

		//��պ�
		backgroundShader.use();
		backgroundShader.setMat4("view", camera.GetViewMatrix(), false);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		renderCube(cubeVAO, cubeVBO);
		


		/*
		skyLight_1.renderLight(lightShader);
		skyLight_2.renderLight(lightShader);
		skyLight_3.renderLight(lightShader);
		pointLight_1.renderLight(lightShader);
		pointLight_2.renderLight(lightShader);
		*/
		
		//���潻��
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//����
	glfwTerminate();
	return 0;
}




unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}