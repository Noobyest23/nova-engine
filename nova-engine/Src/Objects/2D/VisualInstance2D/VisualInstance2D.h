#ifndef NOVA_VISUAL_INSTANCE_2D_H
#define NOVA_VISUAL_INSTANCE_2D_H

#include "../Object2D.h"
#include "../../../Assets/Material/Material.h"
#include "../../../Assets/Mesh2D/Mesh2D.h"
class VisualInstance2D : public Object2D {
public:

	VisualInstance2D();

	int layer = 0;

	glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 self_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	glm::vec4 GetTint();

	void SetMaterial(Material* mat);
	Material* GetMaterial() { return material; };

	virtual ~VisualInstance2D() override;
protected:

	Material* material = nullptr;
	Mesh2D* mesh = nullptr;

	glm::vec4 GetTotalTint();

	virtual void OnDraw() override;

	virtual void OnLoad(LoadableValues values) override;
	//void OnNovaObject(Scope& scope) override;

};


#endif