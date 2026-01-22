#ifndef NOVA_OBJECT_2D_H
#define NOVA_OBJECT_2D_H

#include "../Object.h"
#include <glm/glm.hpp>

class Object2D : public Object {
public:

	const glm::vec2& GetPosition() const { return position; };
	glm::vec2 GetGlobalPosition();
	const glm::vec2& GetScale() const { return scale; };
	glm::vec2 GetGlobalScale();
	const float& GetRotation() const { return rotation; };
	float GetGlobalRotation();

	void SetPosition(const glm::vec2& new_pos) { position = new_pos; };
	void SetGlobalPosition(const glm::vec2& new_global_pos);

	void SetScale(const glm::vec2& new_scale) { scale = new_scale; };
	void SetGlobalScale(const glm::vec2& new_global_scale);

	void SetRotation(float new_rotation) { rotation = new_rotation; };
	void SetGlobalRotation(float new_global_rotation);

protected:

	glm::mat3 GetLocalTransform() const;
	glm::mat3 GetGlobalTransform() const;

	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

	void OnNovaObject(Scope& inherited) override;

};

#endif