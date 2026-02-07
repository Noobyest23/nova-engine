#include "DevCamera2D.h"
#include "../../../Core/Input/InputEventKey.h"
#include <GLFW/glfw3.h>

void DevCamera2D::OnInput(InputEvent* e) {
	if (InputEventKey* key = dynamic_cast<InputEventKey*>(e)) {
		if (key->is_pressed) {
			if (key->key_code == GLFW_KEY_F1) {
				active = !active;
			}
		}
		if (active) {
			switch (key->key_code) {
			case GLFW_KEY_W:
				SetPosition(glm::vec2(GetPosition().x, GetPosition().y + (static_cast<float>(active) * speed)));
				break;
			case GLFW_KEY_A:
				SetPosition(glm::vec2(GetPosition().x + (-static_cast<float>(active) * speed), GetPosition().y));
				break;
			case GLFW_KEY_S:
				SetPosition(glm::vec2(GetPosition().x, GetPosition().y + (-static_cast<float>(active) * speed)));
				break;
			case GLFW_KEY_D:
				SetPosition(glm::vec2(GetPosition().x + (static_cast<float>(active) * speed), GetPosition().y));
				break;
			}
		}
	}
}

