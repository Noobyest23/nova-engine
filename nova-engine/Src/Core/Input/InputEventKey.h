#ifndef NOVA_INPUT_EVENT_KEY_H
#define NOVA_INPUT_EVENT_KEY_H

#include "InputEvent.h"

class InputEventKey : public InputEvent {
public:

	bool is_pressed;
	bool is_released;
	int key_code;

};

#endif