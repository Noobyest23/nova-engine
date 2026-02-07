#ifndef NOVA_CAMERA_2D_H
#define NOVA_CAMERA_2D_H

#include "Object2D.h"
#include <glm/gtc/matrix_transform.hpp>

class Camera2D : public Object2D {
public:

	Camera2D();

	void SetViewportSize(glm::vec2 size);
	void SetZoom(float z);
	float GetZoom() const { return zoom; };

	const glm::mat4& GetProjection() const { return proj; };
	const glm::mat4& GetView() const { return view; };
	const glm::mat4& GetProjectionView() const { return GetProjection() * GetView(); }

	bool active = true;

	virtual CPPObject GetNovaObject() override;

	std::string GetClassName() const override { return "Camera2D"; }

protected:

	float viewportWidth;
	float viewportHeight;
	float zoom = 1.0f;

	glm::mat4 proj{ 1.0f };
	glm::mat4 view{ 1.0f };

	void UpdateProjection();

	void UpdateView();

	void OnUpdate(float deltaTime) override;

	virtual void OnLoad(LoadableValues values) override;

};


#endif