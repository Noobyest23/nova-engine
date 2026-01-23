#include "Sprite2D.h"

Sprite2D::Sprite2D() {
	material = static_cast<Material*>(AssetDB::Get("sprite_2D_shader"));
	mesh = static_cast<BoxMesh2D*>(AssetDB::Get("BoxMesh2D"));
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

	image->Bind(0);
	material->SetUniform("u_texture", 0);

	mesh->Bind();
	glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	mesh->Unbind();
	material->Unbind();
}
