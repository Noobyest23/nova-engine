#include "Window.h"
#include <iostream>

#include "../../NovaScript/Interpretor/Scope.h"

Window* Window::current = nullptr;

void Window::Init() {
	if (!glfwInit()) {
		throw std::exception("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	
	

}

void Window::Cleanup() {
	glfwTerminate();
}

Window::Window(const std::string& name, const int& width, const int& height) {
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		throw std::exception("Failed to create window");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::exception("Failed to load OpenGL");
	}

	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(window, SizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	current = this;
}

Window::~Window() {
	glfwDestroyWindow(window);
}

bool Window::Update() {
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
	glfwSetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y));
}

void Window::SetName(const std::string& name) {
	glfwSetWindowTitle(window, name.c_str());
}

void Window::SizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	if (current) {
		current->resized.Emit(glm::vec2(width, height));
	}
}

#include "../../Core/Input/InputEventKey.h"
#include <deque>
InputEvent* Window::EventGetNext() {
	InputEvent* e = queue.front();
	queue.pop_front();
	return e;
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_UNKNOWN) return;
	InputEventKey* e = new InputEventKey();
	
	e->key_code = key;
	e->is_pressed = (action == GLFW_PRESS);
	e->is_released = (action == GLFW_RELEASE);
	if (current) {
		current->queue.push_back(e);
	}
}