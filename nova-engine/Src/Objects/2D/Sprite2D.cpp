#include "Sprite2D.h"

Image* Sprite2D::GetImage() {
	image->AddRef();
	return image;
}

void Sprite2D::SetImage(Image* new_image) {
	if (image) {
		image->Release();
	}
	
	if (new_image) {
		image->AddRef();
	}
	image = new_image;
}

Sprite2D::~Sprite2D() {
	mesh->Release();
	if (image) {
		image->Release();
	}
}
