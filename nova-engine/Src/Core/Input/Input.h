#ifndef NOVA_INPUT_H
#define NOVA_INPUT_H

#include "../../Platform/Agnostic/Window.h"


class Input {
public:
	static void Init(Window*);
	static void Update();
	static bool IsKeyPressed(int keycode);
	static bool IsKeyDown(int keycode);
	static glm::vec2 GetMousePos();
	
private:
	static Window* window;
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

};


#endif