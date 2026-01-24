#ifndef NOVA_INPUT_EVENT_MOUSE_BUTTON_H
#define NOVA_INPUT_EVENT_MOUSE_BUTTON_H

#include "InputEvent.h"

class InputEventMouseButton : public InputEvent {
public:

	int button;
	bool pressed;

	
};


#endif