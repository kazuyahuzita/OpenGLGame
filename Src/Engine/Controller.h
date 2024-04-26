#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED
#include "glad/glad.h"
#include <GLFW/glfw3.h>
//*
// コントローラーでの操作
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

	// キーが押されていたらtrue, 押されてなければfalseを返す
	bool GetKey(int key) const
	{
		return glfwGetKey(window, key) == GLFW_PRESS;

	}

private:
	GLFWwindow* window = nullptr;				//ウィンドウオブジェクト

};

#endif //CONTROLLER_H_INCLUDED