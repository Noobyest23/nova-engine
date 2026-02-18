#ifndef NOVA_DEBUG_FREECAM
#define NOVA_DEBUG_FREECAM

#include "../Camera2D.h"

class DevCamera2D : public Camera2D {
public:
	DevCamera2D() { name = "DevCamera2D"; }

	const float speed = 100.0f;


private:

	void OnUpdate(float deltaTime) override;

};

#endif