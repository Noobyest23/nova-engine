#ifndef NOVA_WINDOW_H
#define NOVA_WINDOW_H

#include <string>
#include <glm/glm.hpp>
#include "../../Core/Signal.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../NovaScript/Interpretor/Value.h"

class Window {
public:

	static void Init();
	static void Cleanup();

	Window(const std::string& name, const int& width, const int& height);
	~Window();

	bool Update();

	GLFWwindow* GetWindowHandle() { return window; };
	glm::vec2 GetSize();
	std::string GetName();
	void SetSize(const glm::vec2& new_size);
	void SetName(const std::string& new_name);

	Signal<glm::vec2> resized;

private:

	GLFWwindow* window = nullptr;

	static Window* current;
	static void SizeCallback(GLFWwindow* window, int width, int height);

};


#endif
