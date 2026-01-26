#include "Camera2D.h"
#include "../../Core/Engine.h"
#include "../../NovaScript/Library/nova_std_macro.h"
#include "../../Platform/Agnostic/Window.h"

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
	zoom = std::max(0.001f, z);
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

namespace nova_camera2D {

	NOVA_ERR_PUSHER(Camera2D);

	NOVA_SETTER(Camera2D, SetZoom, float);

	NOVA_GETTER(Camera2D, GetZoom);

	Scope GetModule() {
		Scope scope;
		NOVA_BIND_METHOD(SetZoom);
		NOVA_BIND_METHOD(GetZoom);
		return scope;
	}

}

void Camera2D::OnNovaObject(Scope& scope) {
	Object2D::OnNovaObject(scope);
	NOVA_BIND_PROPERTY(active);
	NOVA_BIND_WHOLE_NAMESPACE(nova_camera2D);
}