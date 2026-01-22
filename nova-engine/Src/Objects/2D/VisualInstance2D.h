#ifndef NOVA_VISUAL_INSTANCE_2D_H
#define NOVA_VISUAL_INSTANCE_2D_H

#include "Object2D.h"
#include "../../Assets/Material/Material.h"

class VisualInstance2D : public Object2D {
public:

	int z_index = 0;

	glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 self_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	glm::vec4 GetTint();

	Material* material;

protected:

	glm::vec4 GetTotalTint();

};


#endif