#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderSource.h"
#include <stdio.h>
#include <iostream>
// the size of the window
const unsigned int WINDOWS_WIDTH = 800;
const unsigned int WINDOWS_HEIGHT = 800;
// 用于初始化鼠标的位置 
bool firstMouse = true;
float lastX, lastY;
// callback function 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window); // 处理键盘输入的回调函数

// brief code function
void setShaderMat4(unsigned int ID, const std::string &name, const glm::mat4 &value);
void setShaderVec3(unsigned int ID, const std::string &name, const glm::vec3 &value);
void setShaderFloat(unsigned int ID, const std::string &name, const float &value);

unsigned int createShader(const char *vertexShaderSource, const char *fragmentShaderSource);



// shader source 
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 texture;\n"
"out  vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = vec3(aPos);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
"}\n\0";
const char* gl_version = "#version 330";

// lampPos
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec3 viewPos = glm::vec3(6.0f, 0.0f, 10.0f);
int main()
{
	// initialize glfw 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window 
	GLFWwindow * window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "CUBE", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "窗口创建失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // 上下文

									// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "初始化GLAD失败" << std::endl;
		return -1;
	}
	// 设置开始时的视窗 大小
	glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

	// 注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(gl_version);
	// shader program ----------------------------------------------------------------
	// vertex shader
	unsigned int lampShaderProgram = createShader(lampVerticeShaderSource, lampFragmentShaderSource);
	unsigned int gouraudShaderProgram = createShader(gouraudVerticeSharderSource, gouraudFragmentSharderSource);
	unsigned int phongShaderProgram = createShader(phongVerticeSharderSource, phongFragmentSharderSource);
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// 生成VAO、VBO
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	// 绑定定点数组对象
	glBindVertexArray(cubeVAO);
	// 缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 设置顶点位置属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 设置顶点对应法向量指针
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float ambientStrngth = 0.1f, diffuseStrength = 1.0f, specularStrength = 1.0f,shiness = 2.0f;
	bool isLampPositionChanged = false;
	bool PhongShading = true, GouraudShiding = false;
	unsigned int objectShaderProgram = phongShaderProgram;
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f), ObjectColor(1.0f, 0.5f, 0.31f);
	glm::vec3 lightPosTemp = lightPos;
	// render Loop
	while (!glfwWindowShouldClose(window))
	{
		// 处理键盘输入
		processInput(window);
		// 创建imgUI窗口
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("control");
		ImGui::ColorEdit3("lightColor", (float*)&lightColor);
		ImGui::ColorEdit3("ObjectColor", (float*)&ObjectColor);
		if (ImGui::RadioButton("PhongShading", PhongShading)) {
			PhongShading = true;
			GouraudShiding = false;
			objectShaderProgram = phongShaderProgram;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("GouraudShiding", GouraudShiding)) {
			GouraudShiding = true;
			PhongShading = false;
			objectShaderProgram = gouraudShaderProgram;
		}
		
		ImGui::SliderFloat("ambientStrngth", &ambientStrngth,0.0f,1.0f);
		ImGui::SliderFloat("diffuseStrength", &diffuseStrength,0.0f,1.0f);
		ImGui::SliderFloat("specularStrength", &specularStrength,0.0f,1.0f);
		ImGui::SliderFloat("shiness", &shiness, 0, 16,"%.0f", 1.0f);
		ImGui::Checkbox("isLampPositionChanged", &isLampPositionChanged);
		ImGui::End();
		
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 创建程序对象
		glUseProgram(lampShaderProgram);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::mat4(1.0f);
		if (isLampPositionChanged) {
			lightPos.x =  sin(glfwGetTime()) * 3.0f;
			lightPos.z = cos(glfwGetTime()) * 3.0f;
		}
		else
		{
			lightPos = lightPosTemp;
		}
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		setShaderMat4(lampShaderProgram, "model", model);
		setShaderMat4(lampShaderProgram, "projection", projection);
		setShaderMat4(lampShaderProgram, "view", view);
		setShaderVec3(lampShaderProgram, "lightColor", lightColor);
		glEnableVertexAttribArray(1);
		glBindVertexArray(lampVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUseProgram(objectShaderProgram);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		setShaderMat4(objectShaderProgram, "model", model);
		setShaderMat4(objectShaderProgram, "projection", projection);
		setShaderMat4(objectShaderProgram, "view", view);
		setShaderVec3(objectShaderProgram, "lightPos", lightPos);
		setShaderVec3(objectShaderProgram, "viewPos", viewPos);
		setShaderVec3(objectShaderProgram, "lightColor", lightColor);
		setShaderVec3(objectShaderProgram, "ObjectColor", ObjectColor);
		setShaderFloat(objectShaderProgram, "ambientStrngth", ambientStrngth);
		setShaderFloat(objectShaderProgram, "diffuseStrength", diffuseStrength);
		setShaderFloat(objectShaderProgram, "specularStrength", specularStrength);
		setShaderFloat(objectShaderProgram, "shiness", shiness);
		glEnableVertexAttribArray(1);
		glBindVertexArray(cubeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// 释放窗口，回收资源
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void setShaderMat4(unsigned int ID, const std::string &name, const glm::mat4 &value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void setShaderVec3(unsigned int ID, const std::string &name, const glm::vec3 &value) {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
unsigned int createShader(const char *vertexShaderSource, const char *fragmentShaderSource) {
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char errors[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errors);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errors << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shaders compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errors);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << errors << std::endl;
	}
	// link shaders 
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, errors);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errors << std::endl;
	}
	// after linking delete 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
void setShaderFloat(unsigned int ID, const std::string &name, const float &value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}