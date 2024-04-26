#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED
#include "glad/glad.h"
#include <GLFW/glfw3.h>
//*
// �R���g���[���[�ł̑���
// *//
class Controller
{
public:
	Controller() = default;
	~Controller() = default;

	bool GetButton(int button) const
	{
		return glfwJoystickPresent(button) == GLFW_PRESS;
	}

	// �L�[��������Ă�����true, ������ĂȂ����false��Ԃ�
	bool GetKey(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;

	}

private:
	GLFWwindow* window = nullptr;				//�E�B���h�E�I�u�W�F�N�g

};

#endif //CONTROLLER_H_INCLUDED