#pragma once
#include "ControlledObject.h"
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "..\Global.h"
#include "Camera.h"

class Ray;

// TODO: Better way to address the callback func issue: now using explicit this pointer
/*BEWARE OF THE STATIC THIS POINTER
MAKE SURE YOU BIND IT TO THE RIGHT PLACE*/
class Controller
{
private:
	ControlledObject* dst;
	GLFWwindow* window;

	bool firstmouse = true;
	bool isClickMode = false;
	float lastX = SCREEN_WIDTH / 2.0f;
	float lastY = SCREEN_HEIGHT / 2.0f;
	Camera camera;

public:
	/*Explicitly direct THIS pointer*/
	static Controller* THIS_PTR;
	static int find_key;
	Controller(ControlledObject* obj, GLFWwindow* window);
	~Controller();
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods); // New callback for mouse click

	void BindObject(ControlledObject* const dst) { this->dst = dst; };
	void UnbindObject() { this->dst = nullptr; };
	void BindWindow(GLFWwindow* window) { this->window = window; };
	void UnbindWindow() { this->window = nullptr; };
	// calculate delta time externally
	void ProcessInput(GLFWwindow* window, float deltaTime);

	void SetInputMode(int mode, int value);
	void BindThis()
	{
		if (this != NULL)
			THIS_PTR = this;
	};
	void UnbindThis() { THIS_PTR = NULL; };
	bool isfind(Ray& ray);
};

class Ray
{
public:
	glm::vec3 origin;	 // 射线起点（视点或摄像机位置）
	glm::vec3 direction; // 射线方向（从视点穿过鼠标点击位置）

	// 可选：构造函数、getter 和 setter
	// Ray(const glm::vec3& origin, const glm::vec3& direction)
	//	: origin(origin), direction(direction) {}
};


