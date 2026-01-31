#include "Sprite2D.h"
#include "../../../Core/Engine.h"
#include "../../../Assets/Image/Image.h"
#include "../../../Assets/Mesh2D/BoxMesh2D.h"
#include "../../../Core/AssetDB.h"

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