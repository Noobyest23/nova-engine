#include "Sprite2D.h"
#include "../../../Core/Engine.h"
#include "../../../Assets/Image/Image.h"
#include "../../../Assets/Mesh2D/BoxMesh2D.h"
#include "../../../Core/AssetDB.h"
#include "../../../nova-script/NovaScript/Interpretor/Value.h"
#include "../../../nova-script/NovaScript/Library/nova_std_macro.h"
Sprite2D::Sprite2D() {
	material = static_cast<Material*>(AssetDB::Get("sprite_2D_shader"));
	mesh = static_cast<BoxMesh2D*>(AssetDB::Get("BoxMesh2D"));
	name = "Sprite2D";
}

Image* Sprite2D::GetImage() {
	image->AddRef();
	return image;
}

void Sprite2D::SetImage(Image* new_image) {
	if (image) {
		image->Release();
	}
	
	if (new_image) {
		new_image->AddRef();
		size = glm::vec2(new_image->GetWidth(), new_image->GetHeight());
	}
	image = new_image;
}

Sprite2D::~Sprite2D() {
	if (image) image->Release();
}

void Sprite2D::OnDraw() {
	if (!image || !mesh || !material) return;
	// GetGlobalTransform, GetCamera, and GetTint, should all be cached for optimization
	
	glm::vec4 final_color = GetTint();

	if (!image->IsUploaded()) {
		image->Upload();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VisualInstance2D::OnDraw();
	material->SetUniform("u_color", final_color);
	material->SetUniform("u_img_size", size);

	image->Bind(0);
	material->SetUniform("u_texture", 0);

	mesh->Bind();
	glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	mesh->Unbind();
	material->Unbind();
}

void Sprite2D::OnLoad(LoadableValues values) {
	VisualInstance2D::OnLoad(values);
	for (LValuePair pair : values) {
		if (pair.first == "image") {
			SetImage(static_cast<Image*>(pair.second));
		}
		else if (pair.first == "offset") {
			offset = *static_cast<glm::vec2*>(pair.second);
		}
		else if (pair.first == "flip_h") {
			flip_h = *static_cast<bool*>(pair.second);
		}
		else if (pair.first == "flip_v") {
			flip_v = *static_cast<bool*>(pair.second);
		}
	}
}

namespace sprite_2d {
	static void PushError(const std::string& message) {
		Engine::GetInstance()->PushError("[NovaScript Object] " + message);
	}




	Scope GetModule() {
		Scope scope;

		return scope;
	}

}

CPPObject Sprite2D::GetNovaObject() {
	CPPObject ret = VisualInstance2D::GetNovaObject();
	Scope& scope = ret.scope;
	NOVA_BIND_PROPERTY(flip_h);
	NOVA_BIND_PROPERTY(flip_v);
	NOVA_BIND_PROPERTY(offset);
	return ret;
}

