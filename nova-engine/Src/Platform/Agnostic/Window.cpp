#include "Window.h"
#include <iostream>

#include "../../NovaScript/Interpretor/Scope.h"

Window* Window::current = nullptr;

void Window::Init() {
	if (!glfwInit()) {
		throw std::exception("Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
}

void Window::Cleanup() {
	glfwTerminate();
}

Window::Window(const std::string& name, const int& width, const int& height) {
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (!window) {
		throw std::exception("Failed to create window\n");
	}
	
	glfwSetFramebufferSizeCallback(window, SizeCallback);
	current = this;
}

Window::~Window() {
	glfwDestroyWindow(window);
}

bool Window::Update() {
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BI);

	glfwSwapBuffers(window);
	glfwPollEvents();

	return !glfwWindowShouldClose(window);
}

glm::vec2 Window::GetSize() {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	return glm::vec2(width, height);
}

std::string Window::GetName() {
	return std::string(glfwGetWindowTitle(window));
}

void Window::SetSize(const glm::vec2& size) {
	glfwSetWindowSize(window, size.x, size.y);
}

void Window::SetName(const std::string& name) {
	glfwSetWindowTitle(window, name.c_str());
}

void Window::SizeCallback(GLFWwindow* window, int width, int height) {
	current->resized.Emit(glm::vec2(width, height));
}