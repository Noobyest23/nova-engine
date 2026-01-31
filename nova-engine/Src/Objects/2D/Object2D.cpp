#include "Object2D.h"
#include "../../Core/Engine.h"
#include "../glm/gtc/matrix_transform.hpp"
Object2D::Object2D() {
	name = "Object2D";
}

glm::vec2 Object2D::GetGlobalPosition() {
	glm::mat3 t = GetGlobalTransform();
	return { t[2][0], t[2][1] };
}

glm::vec2 Object2D::GetGlobalScale() {
	if (!parent) return scale;
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		return p_toodee->GetGlobalScale() * scale;
	}
	return scale;
}

float Object2D::GetGlobalRotation() {
	if (!parent) return rotation;
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		return p_toodee->GetGlobalRotation() + rotation;
	}
	return rotation;
}

void Object2D::SetGlobalPosition(const glm::vec2& new_global_pos) {
	if (!parent) { position = new_global_pos; MarkLocalCacheDirty(); return; };
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		glm::mat3 inv_parent = glm::inverse(p_toodee->GetGlobalTransform());
		glm::vec3 local = inv_parent * glm::vec3(new_global_pos, 1.0f);
		position = { local.x, local.y };
		return;
	}
	position = new_global_pos;
	MarkGlobalCacheDirty();
}

void Object2D::SetGlobalScale(const glm::vec2& new_global_scale) {
	if (!parent) { scale = new_global_scale; MarkLocalCacheDirty(); return; };
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		glm::vec2 p_scale = p_toodee->GetGlobalScale();
		scale = new_global_scale / p_scale;
		MarkGlobalCacheDirty();
		return;
	}
	scale = new_global_scale;
	MarkGlobalCacheDirty();
}

void Object2D::SetGlobalRotation(float new_global_rotation) {
	if (!parent) { rotation = new_global_rotation; MarkLocalCacheDirty(); return; }
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		rotation = new_global_rotation - p_toodee->GetGlobalRotation();
		MarkGlobalCacheDirty();
		return;
	}
	rotation = new_global_rotation;
	MarkGlobalCacheDirty();
}

glm::mat3 Object2D::GetLocalTransform() {
	if (_dirty_local) {
		float c = cos(rotation);
		float s = sin(rotation);

		glm::mat3 S = {
			scale.x, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f,
			0.0f, 0.0f, 1.0f,
		};

		glm::mat3 R = {
			c, s, 0.0f,
			-s, c, 0.0f,
			0.0f, 0.0f, 1.0f
		};

		glm::mat3 T = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			position.x, position.y, 1.0f
		};
		local_transform = T * R * S;
		_dirty_local = false;
	}
	return local_transform;
}

glm::mat3 Object2D::GetGlobalTransform() {
	if (_dirty_global) {
		if (!parent) {
			global_transform = GetLocalTransform();
		}
		else if (Object2D* p = dynamic_cast<Object2D*>(parent)) {
			global_transform = p->GetGlobalTransform() * GetLocalTransform();
		}
		else {
			global_transform = GetLocalTransform();
		}
		_dirty_global = false;
	}
	return global_transform;
}

void Object2D::SetPosition(const glm::vec2& pos) {
	position = pos;
	MarkLocalCacheDirty();
}

void Object2D::SetRotation(float new_rotation) {
	rotation = new_rotation;
	constexpr float TAU = 6.283185307179586f;

	rotation = std::fmod(new_rotation, TAU);
	if (rotation < 0.0f) rotation += TAU;
	MarkLocalCacheDirty();
}

void Object2D::SetScale(const glm::vec2& s) {
	scale = s;
	MarkLocalCacheDirty();
}

void Object2D::MarkLocalCacheDirty() {
	_dirty_local = true;
	for (Object* child : children) {
		if (Object2D* c_toodee = dynamic_cast<Object2D*>(child)) {
			c_toodee->MarkGlobalCacheDirty();
		}
	}
}

void Object2D::MarkGlobalCacheDirty() {
	_dirty_global = true;
	for (Object* child : children) {
		if (Object2D* c_toodee = dynamic_cast<Object2D*>(child)) {
			c_toodee->MarkGlobalCacheDirty();
		}
	}
	MarkLocalCacheDirty();
}


void Object2D::OnLoad(std::unordered_map<std::string, void*> values) {
	Object::OnLoad(values);
	for (std::pair<std::string, void*> pair : values) {
		if (pair.first == "position") {
			SetPosition(*static_cast<glm::vec2*>(pair.second));
		}
		if (pair.first == "scale") {
			SetScale(*static_cast<glm::vec2*>(pair.second));
		}
		if (pair.first == "rotation") {
			SetRotation(*static_cast<float*>(pair.second));
		}
	}
}

glm::mat4 Object2D::GetGlobalTransform4() {
	glm::mat3 t = GetGlobalTransform();

	glm::mat4 m(1.0f);
	m[0][0] = t[0][0];
	m[0][1] = t[0][1];
	m[1][0] = t[1][0];
	m[1][1] = t[1][1];
	m[3][0] = t[2][0];
	m[3][1] = t[2][1];

	return m;
}