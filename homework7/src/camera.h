#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// 枚举型变量，用于判断当前摄像机移动的方向
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// camera 初始值
const float YAW = -90.0f; 
const float PITCH = 0.0f;
const float SPEED = 25.0f;
const float SENSITIVITY = 0.05f;
// camera 类，用于使camera移动，
class Camera
{
public:

	// 构造函数
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
	{
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		MovementSpeed = SPEED;  
		MouseSensitivity = SENSITIVITY;
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// 返回view矩阵
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// 处理键盘输入wasd， 
	// direction 是输入的方向 
	// deltaTime 指的是窗口刷新时间间隔
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// 用于处理鼠标移动
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;
		// 确保视窗不会出现翻转
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		// 更新
		updateCameraVectors();
	}

	glm::vec3 getPosition() {
		return Position;
	}
private:
	// 根据欧拉角更新Front矩阵，并重新计算Right和Up矩阵
	void updateCameraVectors()
	{
		// 跟新Front矩阵
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// 计算 right和up矩阵
		Right = glm::normalize(glm::cross(Front, WorldUp));  
		Up = glm::normalize(glm::cross(Right, Front));
	}

	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;    // Forword | backword | right | left 
	float MouseSensitivity; // YAW | Pitch
};

#endif // !CAMERA_H
