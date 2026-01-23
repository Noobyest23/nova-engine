#include "Camera2D.h"
#include "../../Core/Engine.h"

Camera2D::Camera2D() {
	glm::vec2 win_size = Engine::GetInstance()->window->GetSize();
	viewportWidth = win_size.x;
	viewportHeight = win_size.y;

	Engine::GetInstance()->window->resized.Connect([this](glm::vec2 _n_size) {
		this->SetViewportSize(_n_size);
		});

	UpdateProjection();
	UpdateView();
}

void Camera2D::SetViewportSize(glm::vec2 size) {
	viewportWidth = size.x;
	viewportHeight = size.y;
	UpdateProjection();
}

void Camera2D::SetZoom(float z) {
	zoom = z;
	UpdateProjection();
}

void Camera2D::UpdateProjection() {
	float hw = (viewportWidth / 2.0f) / zoom;
	float hh = (viewportHeight / 2.0f) / zoom;

	proj = glm::ortho(-hw, hw, -hh, hh, -1000.0f, 1000.0f);
}

void Camera2D::UpdateView() {
	view = glm::mat4(1.0f);
	view = glm::rotate(view, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	view = glm::translate(view, glm::vec3(-position, 0.0f));
}

void Camera2D::OnUpdate(float deltaTime) {
	UpdateView();
}
