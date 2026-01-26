#include "VisualInstance2D.h"
#include "../../../Core/Engine.h"
#include "../../../Core/Scene.h"
#include "../../../NovaScript/Library/nova_std_macro.h"

glm::vec4 VisualInstance2D::GetTint() {
	return GetTotalTint()* self_tint;
}

glm::vec4 VisualInstance2D::GetTotalTint() {
	if (VisualInstance2D* inst = dynamic_cast<VisualInstance2D*>(parent)) {
		return inst->GetTotalTint() * tint;
	}
	return tint;
}

VisualInstance2D::~VisualInstance2D() {
	if (mesh) {
		mesh->Release();
	}
	if (material) {
		material->Release();
	}
}

void VisualInstance2D::OnDraw() {
	if (material) {
		Camera2D* cam = Engine::GetInstance()->scene->GetActiveCamera2D();
		material->Bind();
		material->SetUniform("u_model", GetGlobalTransform());
		if (cam) {
			material->SetUniform("u_view", cam->GetView());
			material->SetUniform("u_projection", cam->GetProjection());
		}
	}
}

namespace nova_visualinstance2D {
	NOVA_ERR_PUSHER(VisualInstance2D);

	NOVA_GETTER(VisualInstance2D, GetTint);

	Scope GetModule() {
		Scope scope;
		NOVA_BIND_METHOD(GetTint);
		return scope;
	}

}

void VisualInstance2D::OnNovaObject(Scope& scope) {
	Object2D::OnNovaObject(scope);
	NOVA_BIND_PROPERTY(self_tint);
	NOVA_BIND_WHOLE_NAMESPACE(nova_visualinstance2D);
}