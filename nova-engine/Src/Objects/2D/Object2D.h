#ifndef NOVA_OBJECT_2D_H
#define NOVA_OBJECT_2D_H

#include "../Object.h"
#include <glm/glm.hpp>

class Object2D : public Object {
public:

	glm::vec2 GetGlobalPosition();
	glm::vec2 GetGlobalScale();
	float GetGlobalRotation();

	void SetGlobalPosition(const glm::vec2& new_global_pos);
	void SetGlobalScale(const glm::vec2& new_global_scale);
	void SetGlobalRotation(float new_global_rotation);

	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

protected:

	glm::mat3 GetLocalTransform() const;
	glm::mat3 GetGlobalTransform() const;

	void OnNovaObject(Scope& inherited) override;

};

#endif