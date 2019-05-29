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
#include <vector>

// 窗口的大小
unsigned int WINDOWS_WIDTH = 600;
unsigned int WINDOWS_HEIGHT = 600;

// 计算组合数
int combination(int  n, int m);
// --------------------------------------------- 
// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window); // 处理键盘输入的回调函数
void mouse_button_callback(GLFWwindow* window,int button, int action, int mods);// 鼠标点击回调函数
double cursorX, cursorY; // 记录鼠标位置[-1 1]
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos); // 鼠标移动回调函数
// -------------------------------------------
//用于修改 shaderProgram中的 uniform 变量
void setShaderMat4(unsigned int ID, const std::string &name, const glm::mat4 &value);
void setShaderVec3(unsigned int ID, const std::string &name, const glm::vec3 &value);
void setShaderFloat(unsigned int ID, const std::string &name, const float &value);
void setShaderInt(unsigned int ID, const std::string &name, int value);
//	------------------------------------------ 
// 创建一个shaderProgram
unsigned int createShader(const char *vertexShaderSource, const char *fragmentShaderSource);

// 画物体
void renderPoint(); // 用于画出控制定点
void renderLine(const std::vector<float> &vertices); // 根据传入的控制顶点画出控制定点之间的连线
void renderBezier(const std::vector<float> &point);
void renderAssiantLine(const std::vector<float> &controlPoint,const float t);


const char* gl_version = "#version 330";

std::vector<float> points; // 记录bezier曲线的点
std::vector<float> controlPoint; // 记录控制顶点
float t = 0.001; // 参数t
float startTime = 0; // 用于t值得计算

int main()
{
	// 初始化 glfw 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建一个窗口
	GLFWwindow * window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "CUBE", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "窗口创建失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // 上下文

	// 初始化 glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "初始化GLAD失败" << std::endl;
		return -1;
	}
	// 设置开始时的视窗 大小
	glViewport(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT);

	// 注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ----------------------------------------------------------------
	// 调用函数生成相应的shaderProgram
	unsigned int shaderID = createShader(verticeShaderSource, fragmentShaderSource);
	

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 处理键盘输入
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderID);
		// 画出控制定点，用一个正方形表示
		for (int i = 0; i < controlPoint.size(); i += 2) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(controlPoint[i],controlPoint[i + 1], 0.0));
			setShaderMat4(shaderID, "model", model);
			setShaderVec3(shaderID, "color", glm::vec3(1.0, 0.0, 0.0));
			renderPoint();
		}
		glm::mat4 model = glm::mat4(1.0f);
		setShaderMat4(shaderID, "model", model);
		setShaderVec3(shaderID, "color", glm::vec3(0.0, 1.0, 0.0));
		// 将控制定点连起来，画出控制多边形
		renderLine(controlPoint);
		// 设置t的值，3秒画完bezier曲线
		t = (glfwGetTime() - startTime) / 3.0;
		t = t > 1 ? 1 : t; // t <= 1
		// -----------------------------------
		// 画出bezier曲线形成过程
		setShaderVec3(shaderID, "color", glm::vec3(0.0, 1.0, 1.0));
		renderAssiantLine(controlPoint, t); 
		setShaderVec3(shaderID, "color", glm::vec3(1.0, 0.0, 1.0));
		renderBezier(points);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//glDeleteVertexArrays(1, &pointVAO);
	//glDeleteBuffers(1, &pointVBO);

	// 释放窗口，回收资源
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


// ------------------------------------------------------------------------------
// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WINDOWS_WIDTH = width;
	WINDOWS_HEIGHT = height;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			controlPoint.push_back(cursorX);
			controlPoint.push_back(cursorY);
			std::cout << cursorX << "  " << cursorY << std::endl;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (controlPoint.size() > 0) {
				controlPoint.pop_back();
				controlPoint.pop_back();
			}
		}
		startTime = glfwGetTime();
		points.clear();
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	cursorX = (xpos - WINDOWS_WIDTH / 2) / (WINDOWS_WIDTH / 2);
	cursorY = (WINDOWS_HEIGHT / 2 - ypos) / (WINDOWS_HEIGHT / 2);
}
// -------------------------------------------------------------------------
// 用于修改shader中的uniform变量的值 
void setShaderMat4(unsigned int ID, const std::string &name, const glm::mat4 &value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void setShaderVec3(unsigned int ID, const std::string &name, const glm::vec3 &value) {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setShaderInt(unsigned int ID, const std::string &name, int value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void setShaderFloat(unsigned int ID, const std::string &name, const float &value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// -------------------------------------------------------------------------
// 用于创建 shaderProgram 
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



// -------------------------------------------------------------------------
// 用于画一个正方形表示一个控制点
unsigned int pointVAO = 0;
unsigned int pointVBO = 0;
void renderPoint() {
	// 第一次调用时生成pointVAO，pointVBO
	if (pointVAO == 0)
	{
		float vertices[] = {
			// 
			-0.02 ,0.02, 0.02, 0.02, -0.02, -0.02,
			//
			0.02, -0.02, 0.02, 0.02, -0.02, -0.02
		};
		glGenVertexArrays(1, &pointVAO);
		glGenBuffers(1, &pointVBO);
		// 给buffer添加值
		glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 设置定点属性 -----------------
		glBindVertexArray(pointVAO);
		// 顶点位置
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// 渲染立方体
	glBindVertexArray(pointVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
void renderLine(const std::vector<float> &vertices) {
	if (vertices.size() < 4)
		return;
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// 给buffer添加值
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// 设置定点属性 -----------------
	glBindVertexArray(VAO);
	// 顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 渲染立方体
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2);
	glBindVertexArray(0);
	// 释放缓存
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}

// 用于计算组合数
int combination(int  n, int m) {
	int N = 1;
	for (int i = n; i > n - m; i--) {
		N *= i;
	}
	int M = 1;
	for (int i = 1; i <= m; i++) {
		M *= i;
	}
	//std::cout << N / M << std::endl;
	return N / M;
}

// 根据所求出bezier曲线的点画出线
void renderBezier(const std::vector<float> &Point) {
	if (Point.size() < 4)
		return;
/*	// 根据控制顶点使用公式计算 bezier曲线
	std::vector<float> points;
	int N = controlPoint.size() / 2;
	for (float t = 0.0; t <= 1.0; t += 0.001) {
		float x = 0;
		float y = 0;
		for (int i = 0; i < N; i++) {
			float a = pow(1 - t, N - 1 - i );
			float b = pow(t, i );
			int c = combination(N - 1, i);
			x += c * a * b * controlPoint[i*2];
			y += c * a * b * controlPoint[ i * 2 + 1];
		}
		points.push_back(x);
		points.push_back(y);
	}
*/
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// 给buffer添加值
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
	// 设置定点属性 -----------------
	glBindVertexArray(VAO);
	// 顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 渲染点
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, points.size()/2);
	glBindVertexArray(0);
	// 释放缓存
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}

// 画出用于画bezier曲线的辅助线，并求bezier曲线的点的坐标
void renderAssiantLine(const std::vector<float> &controlPoint, const float t) {
	std::vector<float> tempPoints(controlPoint);
	std::vector<float> tempPoints02;
	while (tempPoints.size() / 2 > 2) {
		tempPoints02.clear();
		for (int i = 2; i < tempPoints.size(); i += 2) {
			float x = tempPoints[i - 2] + (tempPoints[i] - tempPoints[i - 2]) * t;
			float y = tempPoints[i + 1 - 2] + (tempPoints[i + 1] - tempPoints[i + 1 - 2]) * t;
			tempPoints02.push_back(x);
			tempPoints02.push_back(y);
		}
		renderLine(tempPoints02);
		tempPoints = tempPoints02;
	}
	// 存储形成bezier曲线的点
	if (t < 1 - 0.01 && tempPoints.size() == 4) {
		float x = tempPoints[0] + (tempPoints[2] - tempPoints[0]) * t;
		float y = tempPoints[1] + (tempPoints[3] - tempPoints[1]) * t;
		points.push_back(x);
		points.push_back(y);
	}
}