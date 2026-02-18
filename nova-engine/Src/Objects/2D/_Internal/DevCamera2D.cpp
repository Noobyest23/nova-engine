#include "DevCamera2D.h"
#include "../../../Core/Input/InputEventKey.h"
#include "../../../Core/Input/Input.h"
#include <GLFW/glfw3.h>


void DevCamera2D::OnUpdate(float deltaTime) {
	Camera2D::OnUpdate(deltaTime);
	if (Input::IsKeyPressed(GLFW_KEY_F1)) {
		active = !active;
	}
	if (Input::IsKeyDown(GLFW_KEY_W)) {
		position.y += speed * deltaTime;
	}
	if (Input::IsKeyDown(GLFW_KEY_S)) {
		position.y -= speed * deltaTime;
	}
	if (Input::IsKeyDown(GLFW_KEY_A)) {
		position.x -= speed * deltaTime;
	}
	if (Input::IsKeyDown(GLFW_KEY_D)) {
		position.x += speed * deltaTime;
	}
}
