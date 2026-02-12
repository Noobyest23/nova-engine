#include "VisualInstance2D.h"
#include "../../../Core/Engine.h"
#include "../../../Scene/Scene.h"
#include "../../../nova-script/NovaScript/Interpretor/Value.h"
#include "../../../nova-script/NovaScript/Library/nova_std_macro.h"
#include "../../../Assets/Image/FileImage.h"
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
		material->SetUniform("u_layer", layer);
		material->SetUniform("u_color", GetTint());
		if (cam) {
			material->SetUniform("u_view", cam->GetView());
			material->SetUniform("u_projection", cam->GetProjection());
		}
	}
	if (mesh) {
		mesh->Bind();
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

namespace visual_instance_2d {
	static void PushError(const std::string& message) {
		Engine::GetInstance()->PushError("[NovaScript Object] " + message);
	}

	nova_std_decl(SetMaterial) {
		req_args(3);
		objget(inst, VisualInstance2D, 0);
		strget(vert, 1);
		strget(frag, 2);
		Material* mat = new Material(vert, frag);
		inst->SetMaterial(mat);
		mat->Release();
	}

	nova_std_decl(GetShaderUniform) {
		req_args(2);
		objget(inst, VisualInstance2D, 0);
		strget(uni, 1);

		Material* mat = inst->GetMaterial();
		Uniform uniform = mat->GetUniform(uni);
		
		return std::visit([&](auto&& value) {
			using T = std::decay_t<decltype(value)>;

			if constexpr (std::is_same_v<T, int>) {
				return Value(std::get<int>(uniform));
			}
			else if constexpr (std::is_same_v<T, float>) {
				return Value(std::get<float>(uniform));
			}
			else if constexpr (std::is_same_v<T, bool>) {
				return Value(std::get<bool>(uniform));
			}
			else if constexpr (std::is_same_v<T, glm::vec2>) {
				return Value(std::get<glm::vec2>(uniform));
			}
			else if constexpr (std::is_same_v<T, glm::vec3>) {
				return Value(std::get<glm::vec3>(uniform));
			}
			else if constexpr (std::is_same_v<T, glm::vec4>) {
				return Value(std::get<glm::vec4>(uniform));
			}
			else {
				return Value();
			}
		}, uniform);
	}

	nova_std_decl(SetShaderUniform) {
		req_args(3);
		objget(inst, VisualInstance2D, 0);
		strget(uni, 1);
		Value* uniform_value = args[2];

		Material* mat = inst->GetMaterial();

		std::visit([&](auto&& value) {
			using T = std::decay_t<decltype(value)>;

			if constexpr (
				std::is_same_v<T, int> ||
				std::is_same_v<T, float> ||
				std::is_same_v<T, bool> ||
				std::is_same_v<T, glm::vec2> ||
				std::is_same_v<T, glm::vec3> ||
				std::is_same_v<T, glm::vec4>
				) {
				inst->GetMaterial()->SetUniform(uni, value);
			}
		}, uniform_value->data);
	}
	
	nova_std_decl(SetShaderImgUniform) {
		req_args(3);
		objget(inst, VisualInstance2D, 0);
		strget(uni, 1);
		strget(filepath, 2);
		FileImage* img = new FileImage(filepath);
		inst->GetMaterial()->SetImageUniform(uni, img);
		img->Release();
	}
	
	Scope GetModule() {
		Scope scope;
		NOVA_BIND_METHOD(SetMaterial);
		NOVA_BIND_METHOD(GetShaderUniform);
		NOVA_BIND_METHOD(SetShaderUniform);
		NOVA_BIND_METHOD(SetShaderImgUniform);
		return scope;
	}

}

CPPObject VisualInstance2D::GetNovaObject() {
	CPPObject ret = Object2D::GetNovaObject();
	Scope& scope = ret.scope;
	NOVA_BIND_PROPERTY(tint);
	NOVA_BIND_PROPERTY(self_tint);
	NOVA_BIND_PROPERTY(layer);
	return ret;
}

