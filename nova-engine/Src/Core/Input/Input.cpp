#include "Input.h"

Window* Input::window = nullptr;

void Input::Init(Window* window) {
	Input::window = window;
	glfwSetKeyCallback(window->GetWindowHandle(), KeyCallback);
}

void Input::Update() {

}

bool Input::IsKeyPressed(int keycode) {
	return false;
}


void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}
