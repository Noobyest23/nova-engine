#ifndef NOVA_SPRITE_2D_H
#define NOVA_SPRITE_2D_H

#include "VisualInstance2D.h"
#include "../../Assets/Image/Image.h"
#include "../../Assets/Mesh2D/BoxMesh2D.h"
#include "../../Core/AssetDB.h"

class Sprite2D : VisualInstance2D {
public:
	
	bool centered = true;
	bool flip_h = true;
	bool flip_v = true;

	glm::vec2 offset = { 0.0f, 0.0f };

	void SetImage(Image* image);
	Image* GetImage();

	~Sprite2D();

protected:

	Image* image = nullptr;

	BoxMesh2D* mesh = static_cast<BoxMesh2D*>(AssetDB::Get("BoxMesh2D"));

	virtual void OnDraw() override;

};

#endif