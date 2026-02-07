#ifndef NOVA_OBJECT_2D_H
#define NOVA_OBJECT_2D_H

#include "../Object.h"
#include <glm/glm.hpp>

class Object2D : public Object {
public:

	Object2D();

	glm::vec2 GetGlobalPosition();
	glm::vec2 GetGlobalScale();
	float GetGlobalRotation();

	void SetGlobalPosition(const glm::vec2& new_global_pos);
	void SetGlobalScale(const glm::vec2& new_global_scale);
	void SetGlobalRotation(float new_global_rotation);

	void SetPosition(const glm::vec2& position);
	glm::vec2 GetPosition() const { return position; };
	void SetRotation(float new_rotation);
	float GetRotation() const { return rotation; };
	void SetScale(const glm::vec2& scale);
	glm::vec2 GetScale() const { return scale; };
	
	CPPObject GetNovaObject() override;

	std::string GetClassName() const override { return "Object2D"; };

protected:

	void MarkLocalCacheDirty();
	void MarkGlobalCacheDirty();

	bool _dirty_local = true;
	bool _dirty_global = true;

	glm::mat3 global_transform;
	glm::mat3 local_transform;

	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 scale = glm::vec2(1.0f);
	float rotation = 0.0f;

	glm::mat3 GetLocalTransform();
	glm::mat3 GetGlobalTransform();
	glm::mat4 GetGlobalTransform4();

	virtual void OnLoad(std::unordered_map<std::string, void*> values) override;

};

#endif