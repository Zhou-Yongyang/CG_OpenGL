#include <glad/glad.h>	// glad 以及 glfw 是调用opengl的库
#include <GLFW/glfw3.h>
#include <iostream>	
#include <glm/glm.hpp>	//glm是向量、矩阵库
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <toolLib/Camera.h>		//控制摄像机
#include <toolLib/Model.h>		//模型载入
#include <toolLib/Shader.h>		//着色器
#include <toolLib/stb_image.h>	//载入纹理

//特殊类/自由类调用
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



	cout << "选择项目：\n1、模型展示" << endl << "2、普通光照效果展示\n" << "3、PBR渲染效果展示\n" << "4、PBR + IBL渲染效果展示（最终效果）" << endl;
	cout << "\n\n" << endl;
	cout << "可用w a s d 鼠标控制视角, 滚轮放缩" << endl;
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
		cout << "材质编号(1 - 6 分别为：织布， 水泥地， 木头， 铁， 金， 大理石)" << endl;
		int mi = 0;
		cin >> mi;
		cout << "原贴图(输入0) or 材质(输入1)" << endl;
		int t = 0;
		cin >> t;
		work_4(mi, t);
	}
	delete[] materialList;
}

int work_1()
{
	// 载入模型，并展示模型的整体法线效果以及贴图效果。
	//初始化设置窗口
	GLFWwindow* window = InitSet();

	//个性化设定

	//个性化设定结束

	//纹理设定
	unsigned int texture1;
	texture1 = loadTexture("resources/textures/work_1/spot_texture.png");
	//纹理设定结束

	//shader声明
	Shader shader_1("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/normal_fragShader.frag");		//法线着色器组
	Shader shader_2("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/texture_fragShader.frag");	//贴图着色器组
	Shader lightShader("resources/shader/light/lightVertex.vert", "resources/shader/light/lightFrag.frag");
	//shader声明完毕

	//model声明
	Model model_1("resources/models/spot_triangulated_good.obj");
	//model声明完毕

	//各类设定
	shader_1.use();
	shader_1.setInt("texture_diffuse1", 0);

	while (!glfwWindowShouldClose(window))
	{
		//时间记录设置
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//时间设置完毕

		//输入检测
		processInput(window);

		//窗口刷新设置
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//摄像机设置
		cameraSet(shader_1);

		//纹理绑定
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		mat4 model;
		model = scale(model, vec3(1, 1, 1));
		float angle = 180.0;
		model = rotate(model, radians(angle), vec3(0.0f, 1.0f, 0.0f));
		//法线设置
		shader_1.setMat4("model", model, false);
		model_1.Draw(shader_1);

		//缓存交换
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//结束
	glfwTerminate();
	return 0;
}

int work_2()
{
	//添加基本光照 3组（1、平行光+平行光。 2、平行光+点光源。 3、平行光+聚光灯光源）
	// 载入模型，并展示模型的整体法线效果以及贴图效果。
	//初始化设置窗口
	GLFWwindow* window = InitSet();

	//个性化设定

	//个性化设定结束

	//纹理设定
	unsigned int texture1;
	texture1 = loadTexture("resources/textures/work_1/spot_texture.png");
	//纹理设定结束

	//shader声明
	Shader shader_1("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/normal_fragShader.frag");		//法线着色器组
	Shader shader_2("resources/shader/work_1/vertexShader.vert", "resources/shader/work_1/texture_fragShader.frag");	//贴图着色器组
	Shader lightShader("resources/shader/light/lightVertex.vert", "resources/shader/light/lightFrag.frag");
	//shader声明完毕

	//model声明
	Model model_1("resources/models/spot_triangulated_good.obj");
	SkyLight skyLight_1("SkyLight[0]", vec3(0,8,10), vec3(0,-8,-10), 0.5);
	SkyLight skyLight_2("SkyLight[1]", vec3(-8, 7, -4), vec3(8, -7, 4), 0.5, 0.5f, vec3(1.0, 1.0, 1.0));

	PointLight pointLight_1("PointLight[0]", vec3(0, 2, 2), 4, 0.2, vec3(1.0,0.7,0.4));
	PointLight pointLight_2("PointLight[1]", vec3(0, 1, -2), 4, 0.2, vec3(0.8, 0.6, 0.4));
	//model声明完毕

	//各类设定
	shader_2.use();
	shader_2.setInt("texture_diffuse1", 0);

	while (!glfwWindowShouldClose(window))
	{
		//时间记录设置
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//时间设置完毕

		//输入检测
		processInput(window);

		//窗口刷新设置
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//摄像机设置
		cameraSet(shader_2);

		//纹理绑定
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		mat4 model;
		model = scale(model, vec3(1, 1, 1));
		float angle = 180.0;
		model = rotate(model, radians(angle), vec3(0.0f, 1.0f, 0.0f));
		//法线设置
		//shader_1.setMat4("model", model, false);
		//model_1.Draw(shader_1);
		//贴图设置
		shader_2.setMat4("model", model, false);
		//glsl变量设置
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
		
		//缓存交换
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//结束
	glfwTerminate();
	return 0;
}

int work_3()
{
	//PBR渲染，添加辐照度贴图，进行PBR shader编写
	GLFWwindow* window = InitSet();

	// 模型载入
	Model model_1("resources/models/spot_triangulated_good.obj");

	//shader载入
	Shader shader("resources/shader/work_3/vertexShader.vert", "resources/shader/work_3/fragShader.frag");	//物体着色器组
	Shader lightShader("resources/shader/light/lightVertex.vert", "resources/shader/light/lightFrag.frag");

	//贴图载入
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
	cout << "加载材质" << endl;
	Material subList[6] = { materialList[0], materialList[1], materialList[2], materialList[3], materialList[4], materialList[5] };
	for (int i = 0; i < 6; i++)
	{
		materialList[i].loadMaterial();
		materialList[i].setTexture("resources/textures/work_1/spot_texture.png", Material::COLOR);
		subList[i].loadMaterial();
	}
	cout << "加载完毕" << endl;

	//贴图载入完毕
	SkyLight skyLight_1("skyLight[0]", vec3(0, 10, 0), vec3(0, -1, 0), 20, 0.4);
	SkyLight skyLight_2("skyLight[1]", vec3(8, 6, 6), vec3(-8, -6, -6), 20, 0.4);

	//PointLight pointLight_1("pointLight[0]", vec3(0,0,0), 5, 0.3);
	PointLight pointLight_2("pointLight[1]", vec3(5, 0, 0), 15, 0.3);
	PointLight pointLight_3("pointLight[2]", vec3(0, 5, 0), 15, 0.3);
	PointLight pointLight_4("pointLight[3]", vec3(0, 0, 5), 15, 0.3);


	while (!glfwWindowShouldClose(window))
	{
		//时间记录设置
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//时间设置完毕

		//输入检测
		processInput(window);

		//窗口刷新设置
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//摄像机设置
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


		//缓存交换
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//结束
	glfwTerminate();
	return 0;
}

int work_4(int mi, int t)
{
	mi = mi - 1;
	//PBR渲染，添加辐照度贴图，进行PBR shader编写
	GLFWwindow* window = InitSet();

	// 模型载入
	Model model_1("resources/models/spot_triangulated_good.obj");

	//shader载入
	Shader pbrShader("resources/shader/work_4/vertexShader.vert", "resources/shader/work_4/fragShader.frag");	//物体着色器组
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

	//shader载入完成

	
	//pbr: 设置帧缓冲
	unsigned int captureFBO = 0, captureRBO = 0;
	setFrameBuffer(captureFBO, captureRBO);

	//hdr贴图载入
	unsigned hdrTexture = loadHDRenvMap("resources/textures/work_4/HDR/fireplace_4k.hdr");
	
	//环境立方体贴图载入
	unsigned int envCubemap = setCubeMapFrameBuffer();
	
	//立方体贴图转换
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
	
	//创建辐照度贴图

	unsigned int irradianceMap = setIrradianceMap(captureFBO,captureRBO);
	//材质载入
	unsigned int aoTex, colorTex, normalTex, roughTex, metallicTex;


	//辐照度shader设置
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
	//完毕

	//预先过滤立方体
	unsigned int prefilterMap = setPreFilterMap();
	
	//蒙特卡洛积分模拟
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
	//全局变量初始化
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

	//贴图载入完毕
	//自定义光源
	SkyLight skyLight_1("skyLight[0]", vec3(0,0,-10), vec3(0, 0, 10), 4, 0.4);
	SkyLight skyLight_2("skyLight[1]", vec3(0, 1, -10), vec3(0, -1, 10), 3, 0.4);
	SkyLight skyLight_3("skyLight[2]", vec3(0, -1, -10), vec3(0, 1, 10), 2, 0.4);


	PointLight pointLight_1("pointLight[0]", vec3(2, -3, 2), 40);
	PointLight pointLight_2("pointLight[1]", vec3(2.2, -3.1, 1.9), 10);
	PointLight pointLight_3("pointLight[2]", vec3(1.9, -2.9, 2.1), 10);
	PointLight pointLight_4("pointLight[3]", vec3(10, -10, 10), 0);


	while (!glfwWindowShouldClose(window))
	{
		//时间记录设置
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//时间设置完毕

		//输入检测
		processInput(window);

		//窗口刷新设置
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//摄像机设置
		cameraSet(pbrShader);
		pbrShader.setVec3("viewPos", camera.Position);

		//绑定IBL数据
		
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

		//天空盒
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
		
		//缓存交换
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//结束
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