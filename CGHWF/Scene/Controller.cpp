#include "Controller.h"
#include "../utils.h"
#include <iostream>

Controller* Controller::THIS_PTR = NULL;
int Controller::find_key = 0;

Controller::Controller(ControlledObject* obj, GLFWwindow* window) : dst(obj), window(window)
{
	// GLFW can't register callback function with this pointer(object), so use the class's static function and set This pointer manually
	BindThis();
	glfwMakeContextCurrent(window);

	// ��ʼʱ��������꣬�������ӽǿ���
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, Controller::ScrollCallback);
	glfwSetFramebufferSizeCallback(window, Controller::FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, Controller::MouseCallback);
	glfwSetMouseButtonCallback(window, Controller::MouseClickCallback); // Register the mouse click callback
}

Controller::~Controller()
{
	UnbindThis();
}

void Controller::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	if (THIS_PTR != NULL)
	{

		GLCALL(glViewport(0, 0, width, height));
	}
	else
	{
		std::cout << "[ERROR]Controller.cpp:THIS pointer is not bind to any controller object.";
	}
}

void Controller::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (THIS_PTR != NULL)
	{

		THIS_PTR->dst->ProcessMouseScroll((float)yoffset);
	}
	else
	{
		std::cout << "[ERROR]Controller.cpp:THIS pointer is not bind to any controller object.";
	}
}

void Controller::MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (THIS_PTR != NULL)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (THIS_PTR->firstmouse)
		{
			THIS_PTR->lastX = xpos;
			THIS_PTR->lastY = ypos;
			THIS_PTR->firstmouse = false;
		}

		if (THIS_PTR->isClickMode)
		{
			// ������ڵ��ģʽ�������ӽǿ��ƣ�ֻ�������λ��
			// ���������ʵ���������������Ϊ
			// ���磺��ȡ�����λ�û��߽������߼��
			return; // �������ӽǿ��ƵĴ���
		}

		float xoffset = xpos - THIS_PTR->lastX;
		float yoffset = THIS_PTR->lastY - ypos; // reversed since y-coordinates go from bottom to top

		THIS_PTR->lastX = xpos;
		THIS_PTR->lastY = ypos;

		THIS_PTR->dst->ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		std::cout << "[ERROR]Controller.cpp:THIS pointer is not bind to any controller object.";
	}
}


// New MouseClick callback function
void Controller::MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (THIS_PTR != NULL && THIS_PTR->isClickMode)
	{
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			// Get the Modelview and Projection matrices from OpenGL
			glm::mat4 mvmatrix = glm::mat4(1.0f);
			glm::mat4 projmatrix = glm::mat4(1.0f);
			glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(mvmatrix));
			glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projmatrix));

			// Get the depth value at the mouse position
			float depth;
			glReadPixels(xpos, viewport[3] - ypos - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

			// Convert the window coordinates to normalized device coordinates (NDC)
			GLint realy = viewport[3] - static_cast<int>(ypos) - 1;
			float x = (xpos - viewport[0]) / viewport[2] * 2.0f - 1.0f;
			float y = (realy - viewport[1]) / viewport[3] * 2.0f - 1.0f;
			float z = depth;

			glm::vec4 screenPos(x, y, z, 1.0f); // NDC (Normalized Device Coordinates)

			// Combine the projection and modelview matrices to get the combined transform
			glm::mat4 combinedMatrix = projmatrix * mvmatrix;

			// Invert the combined matrix
			glm::mat4 inverseMatrix = glm::inverse(combinedMatrix);

			// Convert from NDC to world coordinates
			glm::vec4 worldPos = inverseMatrix * screenPos;

			// Divide by w to get the final world position (homogeneous coordinates to Cartesian)
			glm::vec3 worldCoords = glm::vec3(worldPos.x / worldPos.w, worldPos.y / worldPos.w, worldPos.z / worldPos.w);

			// Output world coordinates at z=0
			std::cout << " World coords: (" << worldCoords.x << ", " << worldCoords.y << ", " << worldCoords.z << ")" << std::endl;

			// Define the camera position (assuming it's at (0, 0, 3) in world space)
			// glm::vec3 cameraPosition = glm::vec3(glm::inverse(mvmatrix)[3]);
			glm::vec3 cameraPosition = THIS_PTR->camera.GetPos();

			// Mouse position in world space
			glm::vec3 mousePos = worldCoords;

			// Calculate the direction of the ray (normalized)
			glm::vec3 rayDirection = glm::normalize(mousePos - cameraPosition);

			// Create the ray object
			Ray ray = { cameraPosition, rayDirection };

			// Perform ray intersection check (assuming this function is implemented elsewhere)
			bool found = THIS_PTR->isfind(ray);

			if (found)
			{
				std::cout << "Found the key!" << std::endl;
			}
			else
			{
				std::cout << "Key not found." << std::endl;
			}
		}
	}
	else
	{
		std::cout << "[ERROR]Controller.cpp: THIS pointer is not bound to any controller object." << std::endl;
	}
}
/*
void Controller::MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (THIS_PTR != NULL && THIS_PTR->isClickMode)
	{
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			// ��ȡ���ڴ�С
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			// ��׼���豸����
			float x = (2.0f * xpos) / width - 1.0f;
			float y = 1.0f - (2.0f * ypos) / height; // ע�� y �᷽���ת��
			float z = 1.0f; // ������������Ļ����Զ�ĵ�

			glm::vec3 ray_nds(x, y, z);
			glm::vec4 ray_clip(ray_nds, 1.0);

			// ��ȡͶӰ�������ͼ����
			glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
			glm::mat4 view_matrix = glm::lookAt(THIS_PTR->camera.GetPos(), THIS_PTR->camera.GetPos() + THIS_PTR->camera.GetFront(), THIS_PTR->camera.GetUp());

			// �����ߴӲü�����ϵת�����������ϵ
			glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
			ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

			// �����ߴ��������ϵת������������ϵ
			glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view_matrix) * ray_eye));

			std::cout << "Ray Direction: (" << ray_world.x << ", " << ray_world.y << ", " << ray_world.z << ")\n";

			// �������λ�ÿ�ʼ������
			glm::vec3 ray_origin = THIS_PTR->camera.GetPos();

			// �������߶���
			Ray ray = { ray_origin, ray_world };

			// ִ�����߼��
			bool found = THIS_PTR->isfind(ray);

			if (found) {
				std::cout << "Found the key!" << std::endl;
			}
			else {
				std::cout << "Key not found." << std::endl;
			}
		}
	}
	else
	{
		std::cout << "[ERROR]Controller.cpp: THIS pointer is not bound to any controller object." << std::endl;
	}
}
*/
//*/
// New MouseClick callback function
/*
void Controller::MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (THIS_PTR != NULL) {
		if (action == GLFW_PRESS) {
			// ��ȡ���λ��
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			GLint viewport[4];
			GLdouble mvmatrix[16], projmatrix[16];
			GLdouble wx, wy, wz;

			// ��ȡ��ǰ OpenGL ���ڵ��ӿ���Ϣ             glGetIntegerv(GL_VIEWPORT, viewport);

			// ��ȡ��ǰ��ģ����ͼ�����ͶӰ����
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

			// OpenGL ������ϵ�봰������ϵ�в��죬��Ҫת������ y ����
			GLint realy = viewport[3] - static_cast<int>(ypos) - 1;

			// ʹ�� gluUnProject �����������Ļ����ת��Ϊ��������
			gluUnProject(xpos, realy, 0.0f, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

			// ���ת�������������
			//std::cout << "World coords at z=0: (" << wx << ", " << wy << ", " << wz << ")" << std::endl;

			// ��ȡ�����λ�ã��ӵ㣩 �� ���߷���
			glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);  // ���������λ���� (0, 0, 3)
			glm::vec3 mousePos = glm::vec3(wx, wy, wz);
			glm::vec3 rayDirection = glm::normalize(mousePos - cameraPosition);

			Ray ray = { cameraPosition, rayDirection };

			// ���� isfind ���������������������ײ��� raycasting
			bool found = THIS_PTR->isfind(ray);

			if (found) {
				std::cout << "Found the key!" << std::endl;
			}
			else {
				std::cout << "Key not found." << std::endl;
			}
		}
	}
	else {
		std::cout << "[ERROR]Controller.cpp: THIS pointer is not bind to any controller object.";
	}
}
*/
/*
void Controller::ProcessInput(float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::SWITCHSPOTLIGHT);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::MODE_SWITCH);

}
*/
void Controller::ProcessInput(GLFWwindow* window, float deltaTime)
{
	if (THIS_PTR == nullptr)
	{
		std::cerr << "[ERROR] THIS_PTR is null!" << std::endl;
		return; // ���������ʵ��Ĵ������߼�
	}
	// ������ڵ��ģʽ�����η�����ƵĲ���
	if (THIS_PTR != NULL)
	{
		// ��ͨ�ļ��̲�����������������ƶ�
		if (!THIS_PTR->isClickMode)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::FORWARD, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::BACKWARD, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::LEFT, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::RIGHT, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::UP, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::DOWN, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::SWITCHSPOTLIGHT);
			if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
				dst->ProcessKeyboard(KEYBOARD_ACTIONS::MODE_SWITCH);
			if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			{
				// �л������ģʽ
				THIS_PTR->isClickMode = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // ��ʾ���
			}
		}

		// ��������İ����¼������� T �� R
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			// �л������ģʽ
			THIS_PTR->isClickMode = true;
			std::cout << "Click Mode" << std::endl;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // ��ʾ���
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			// �л����ӽǿ���ģʽ
			THIS_PTR->isClickMode = false;
			std::cout << "Click Mode Off" << std::endl;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // �������
		}
	}
}

void Controller::SetInputMode(int mode, int value)
{
	glfwSetInputMode(window, mode, value);
}

// �Ƿ��ҵ�Կ�׺���
/*
bool Controller::isfind(Ray& ray)
{
	glm::vec3 keyPosition = glm::vec3(55.0f, 0.2f, 28.3f); // Կ�׵�λ��
	float keyRadius = 0.5f;

	// ��������������������
	float t = glm::dot(keyPosition - ray.origin, ray.direction);

	glm::vec3 closestPoint = ray.origin + t * ray.direction;
	float distance = glm::length(closestPoint - keyPosition);

	if (distance < keyRadius)
	{
		this->find_key = 1;
		return true;
	}

	return false;
}
//*/

bool Controller::isfind(Ray& ray)
{
	//glm::vec3 keyPosition = glm::vec3(55.0f, 0.2f, 28.3f);
	//glm::vec3 keyPosition = glm::vec3(55.0f, 10.0f, 67.0f); // Կ�׵�λ��
	glm::vec3 keyPosition = glm::vec3(0.3f, -1.0f, 1.0f);
	float keyRadius = 0.4f;

	// ���������������ཻ
	glm::vec3 oc = ray.origin - keyPosition;  // ��������㵽���ĵ�����

	// �������߷��������ĵľ���
	float b = glm::dot(oc, ray.direction);
	float c = glm::dot(oc, oc) - keyRadius * keyRadius;

	// �ж� discriminant
	float discriminant = b * b - c;

	if (discriminant > 0) {
		// ��� discriminant > 0����ʾ�����������ཻ
		// ���㽻��� t ֵ
		/*
		float t1 = -b - sqrt(discriminant);
		float t2 = -b + sqrt(discriminant);

		// ��� t ֵ�Ƿ�Ϊ���������߷����ϣ�
		if (t1 > 0 || t2 > 0) {
			find_key = 1;
			return true;
		}
		*/
		find_key = 1;
		return true;
	}

	return false;
}

