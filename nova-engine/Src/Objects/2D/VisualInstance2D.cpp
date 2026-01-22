#include "VisualInstance2D.h"


glm::vec4 VisualInstance2D::GetTint() {
	return GetTotalTint()* self_tint;
}

glm::vec4 VisualInstance2D::GetTotalTint() {
	if (VisualInstance2D* inst = dynamic_cast<VisualInstance2D*>(parent)) {
		return inst->GetTotalTint() * tint;
	}
	return tint;
}