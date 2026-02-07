#ifndef NOVA_DEBUG_FREECAM
#define NOVA_DEBUG_FREECAM

#include "../Camera2D.h"

class DevCamera2D : public Camera2D {
public:
	DevCamera2D() { name = "DevCamera2D"; }

	float speed = 10.0f;


private:

	void OnInput(InputEvent* e) override;

};

#endif