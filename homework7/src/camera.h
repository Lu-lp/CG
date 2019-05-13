#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// ö���ͱ����������жϵ�ǰ������ƶ��ķ���
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// camera ��ʼֵ
const float YAW = -90.0f; 
const float PITCH = 0.0f;
const float SPEED = 25.0f;
const float SENSITIVITY = 0.05f;
// camera �࣬����ʹcamera�ƶ���
class Camera
{
public:

	// ���캯��
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
	// ����view����
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// �����������wasd�� 
	// direction ������ķ��� 
	// deltaTime ָ���Ǵ���ˢ��ʱ����
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

	// ���ڴ�������ƶ�
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;
		// ȷ���Ӵ�������ַ�ת
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		// ����
		updateCameraVectors();
	}

	glm::vec3 getPosition() {
		return Position;
	}
private:
	// ����ŷ���Ǹ���Front���󣬲����¼���Right��Up����
	void updateCameraVectors()
	{
		// ����Front����
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// ���� right��up����
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
