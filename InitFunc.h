#pragma once
#include <string>
#include <glad/glad.h>	// glad �Լ� glfw �ǵ���opengl�Ŀ�
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>	//glm�������������
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <toolLib/Shader.h>		//��ɫ��
#include <toolLib/Camera.h>
using namespace glm;
// �趨��������
GLFWwindow* InitSet();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
void cameraSet(Shader& shader);
void setFrameBuffer(unsigned int& FBO, unsigned int& RBO);
const unsigned int loadHDRenvMap(std::string path);
const unsigned int setCubeMapFrameBuffer();
void renderCube(unsigned int cubeVAO, unsigned int cubeVBO);
unsigned int setIrradianceMap(unsigned int& FBO, unsigned& RBO);
unsigned int setPreFilterMap();
void renderQuad(unsigned int& quadVAO, unsigned int& quadVBO);


//������ȫ�����趨
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

// camera
extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;

// timing
extern float deltaTime;
extern float lastFrame;

// cubeMapdata

extern mat4 captureProjection;
extern mat4 captureViews[];