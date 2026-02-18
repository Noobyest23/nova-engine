#include "Input.h"
#include "InputEventKey.h"
#include "../Engine.h"
#include "../../Scene/Scene.h"
#include <unordered_map>

Window* Input::window = nullptr;

static std::unordered_map<int, bool> s_KeyDown;
static std::unordered_map<int, bool> s_KeyPressed;
static std::unordered_map<int, bool> s_KeyReleased;

void Input::Init(Window* window) {
	Input::window = window;
	glfwSetKeyCallback(window->GetWindowHandle(), KeyCallback);
}

void Input::Update() {
	s_KeyPressed.clear();
	s_KeyReleased.clear();
}

bool Input::IsKeyPressed(int keycode) {
	auto it = s_KeyPressed.find(keycode);
	if (it == s_KeyPressed.end()) return false;
	return it->second;
}

bool Input::IsKeyDown(int keycode) {
	auto it = s_KeyDown.find(keycode);
	if (it == s_KeyDown.end()) return false;
	return it->second;
}

glm::vec2 Input::GetMousePos() {
	if (!window) return glm::vec2(0.0f, 0.0f);
	double x, y;
	glfwGetCursorPos(window->GetWindowHandle(), &x, &y);
	return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

void Input::KeyCallback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (!s_KeyDown[key]) {
			s_KeyPressed[key] = true;
		}
		s_KeyDown[key] = true;

		InputEventKey* e = new InputEventKey();
		e->is_pressed = true;
		e->is_released = false;
		e->key_code = key;

		if (Engine::GetInstance() && Engine::GetInstance()->scene) {
			Engine::GetInstance()->scene->Input(e);
		}
		delete e;
	}
	else if (action == GLFW_RELEASE) {
		s_KeyDown[key] = false;
		s_KeyReleased[key] = true;

		InputEventKey* e = new InputEventKey();
		e->is_pressed = false;
		e->is_released = true;
		e->key_code = key;

		if (Engine::GetInstance() && Engine::GetInstance()->scene) {
			Engine::GetInstance()->scene->Input(e);
		}
		delete e;
	}
	else if (action == GLFW_REPEAT) {
		s_KeyDown[key] = true;

		InputEventKey* e = new InputEventKey();
		e->is_pressed = false;
		e->is_released = false;
		e->key_code = key;

		if (Engine::GetInstance() && Engine::GetInstance()->scene) {
			Engine::GetInstance()->scene->Input(e);
		}
		delete e;
	}
}