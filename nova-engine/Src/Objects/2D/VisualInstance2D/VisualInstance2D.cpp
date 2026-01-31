#include "VisualInstance2D.h"
#include "../../../Core/Engine.h"
#include "../../../Scene/Scene.h"

VisualInstance2D::VisualInstance2D() {
	name = "VisualInstance2D";
}

glm::vec4 VisualInstance2D::GetTint() {
	return GetTotalTint()* self_tint;
}

glm::vec4 VisualInstance2D::GetTotalTint() {
	if (VisualInstance2D* inst = dynamic_cast<VisualInstance2D*>(parent)) {
		return inst->GetTotalTint() * tint;
	}
	return tint;
}

void VisualInstance2D::SetMaterial(Material* mat) {
	if (material) {
		material->Release();
	}

	if (mat) {
		mat->AddRef();
	}

	material = mat;
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
		material->SetUniform("u_model", GetGlobalTransform4());
		if (cam) {
			material->SetUniform("u_view", cam->GetView());
			material->SetUniform("u_projection", cam->GetProjection());
		}
	}
}

void VisualInstance2D::OnLoad(LoadableValues values) {
	Object2D::OnLoad(values);
	for (LValuePair pair : values) {
		if (pair.first == "tint") {
			tint = *static_cast<glm::vec4*>(pair.second);
		}
		else if (pair.first == "self_tint") {
			self_tint = *static_cast<glm::vec4*>(pair.second);
		}
		else if (pair.first == "material") {
			SetMaterial(static_cast<Material*>(pair.second));
		}
		else if (pair.first == "layer") {
			layer = *static_cast<int*>(pair.second);
		}
	}
}
