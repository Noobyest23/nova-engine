#include "Camera2D.h"
#include "../../Core/Engine.h"
#include "../../Platform/Agnostic/Window.h"
#include "../../../nova-script/NovaScript/Interpretor/Value.h"
#include "../../../nova-script/NovaScript/Library/nova_std_macro.h"
Camera2D::Camera2D() {
	glm::vec2 win_size = Engine::GetInstance()->window->GetSize();
	viewportWidth = win_size.x;
	viewportHeight = win_size.y;

	Engine::GetInstance()->window->resized.Connect([this](glm::vec2 _n_size) {
		this->SetViewportSize(_n_size);
		});

	UpdateProjection();
	UpdateView();

	name = "Camera2D";
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

	proj = glm::ortho(-hw, hw, -hh, hh, -10.0f, 10.0f);
}

void Camera2D::UpdateView() {
	view = glm::mat4(1.0f);
	view = glm::rotate(view, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	view = glm::translate(view, glm::vec3(-position, 0.0f));
}

void Camera2D::OnUpdate(float deltaTime) {
	UpdateView();
}

void Camera2D::OnLoad(LoadableValues values) {
	Object2D::OnLoad(values);
	for (LValuePair pair : values) {
		if (pair.first == "zoom") {
			SetZoom(*static_cast<float*>(pair.second));
		}
		else if (pair.first == "active") {
			active = *static_cast<bool*>(pair.second);
		}
	}
}

namespace camera_2d {
	static void PushError(const std::string& message) {
		Engine::GetInstance()->PushError("[NovaScript Camera2D] " + message);
	}

	NOVA_SETTER(Camera2D, SetZoom, float);
	NOVA_GETTER(Camera2D, GetZoom, float);

	Scope GetModule() {
		Scope scope;
		NOVA_BIND_METHOD(SetZoom);
		NOVA_BIND_METHOD(GetZoom);
		return scope;
	}
}

CPPObject Camera2D::GetNovaObject() {
	CPPObject ret = Object::GetNovaObject();
	Scope& scope = ret.scope;
	NOVA_BIND_WHOLE_NAMESPACE(camera_2d);
	NOVA_BIND_PROPERTY(active);
	return ret;
}