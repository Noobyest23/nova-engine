#ifndef NOVA_SPRITE_2D_H
#define NOVA_SPRITE_2D_H

#include "VisualInstance2D.h"
#include "../../../Assets/Image/Image.h"
#include "../../../Assets/Mesh2D/BoxMesh2D.h"
#include "../../../Core/AssetDB.h"

class Sprite2D : public VisualInstance2D {
public:
	
	Sprite2D();

	bool flip_h = false;
	bool flip_v = false;

	glm::vec2 offset = { 0.0f, 0.0f };

	void SetImage(Image* image);
	Image* GetImage();

	virtual ~Sprite2D() override;

	std::string GetClassName() const override { return "Sprite2D"; }

protected:

	Image* image = nullptr;

	virtual void OnDraw() override;

};

#endif