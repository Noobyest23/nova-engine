#include "Object2D.h"
#include "../../Core/Engine.h"

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
	if (!parent) { position = new_global_pos; return; };
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		glm::mat3 inv_parent = glm::inverse(p_toodee->GetGlobalTransform());
		glm::vec3 local = inv_parent * glm::vec3(new_global_pos, 1.0f);
		position = { local.x, local.y };
		return;
	}
	position = new_global_pos;
	MarkCacheDirty();
}

void Object2D::SetGlobalScale(const glm::vec2& new_global_scale) {
	if (!parent) { scale = new_global_scale; return; };
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		glm::vec2 p_scale = p_toodee->GetGlobalScale();
		scale = new_global_scale / p_scale;
		MarkCacheDirty();
		return;
	}
	scale = new_global_scale;
	MarkCacheDirty();
}

void Object2D::SetGlobalRotation(float new_global_rotation) {
	if (!parent) { rotation = new_global_rotation; return; }
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		rotation = new_global_rotation - p_toodee->GetGlobalRotation();
		MarkCacheDirty();
		return;
	}
	rotation = new_global_rotation;
	MarkCacheDirty();
}

glm::mat3 Object2D::GetLocalTransform() {
	if (_dirty_cache) {
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
			1.0f, 0.0f, position.x,
			0.0f, 1.0f, position.y,
			0.0f, 0.0f, 1.0f
		};
		local_transform = T * R * S;
		_dirty_cache = false;
		return local_transform;
	}
	else {
		return local_transform;
	}

}

glm::mat3 Object2D::GetGlobalTransform() {
	if (_dirty_cache) {
		if (!parent) return GetLocalTransform();
		if (Object2D* p = dynamic_cast<Object2D*>(parent)) {
			global_transform = p->GetGlobalTransform() * GetLocalTransform();
			return global_transform;
		}
		return GetLocalTransform();
	}
	else {
		return global_transform;
	}
}

void Object2D::SetPosition(const glm::vec2& pos) {
	position = pos;
	MarkCacheDirty();
}

void Object2D::SetRotation(float new_rotation) {
	rotation = new_rotation;
	constexpr float TAU = 6.283185307179586f;

	rotation = std::fmod(new_rotation, TAU);
	if (rotation < 0.0f) rotation += TAU;
	MarkCacheDirty();
}

void Object2D::SetScale(const glm::vec2& s) {
	scale = s;
	MarkCacheDirty();
}

void Object2D::MarkCacheDirty() {
	_dirty_cache = true;
	for (Object* child : children) {
		if (Object2D* c_toodee = dynamic_cast<Object2D*>(child)) {
			c_toodee->MarkCacheDirty();
		}
	}
}

#include "../../NovaScript/Library/nova_std_macro.h"
namespace nova_object2D {
	
	static void PushError(const std::string& message) {
		Engine::GetInstance()->PushError("[NovaScript Object2D] " + message);
	}

	NOVA_GETTER(Object2D, GetGlobalPosition);

	NOVA_GETTER(Object2D, GetGlobalRotation);

	NOVA_GETTER(Object2D, GetGlobalScale);

	NOVA_SETTER(Object2D, SetGlobalPosition, vec2);

	NOVA_SETTER(Object2D, SetGlobalRotation, float);

	NOVA_SETTER(Object2D, SetGlobalScale, vec2);

	NOVA_GETTER(Object2D, GetPosition);

	NOVA_GETTER(Object2D, GetRotation);

	NOVA_GETTER(Object2D, GetScale);

	NOVA_SETTER(Object2D, SetPosition, vec2);

	NOVA_SETTER(Object2D, SetRotation, float);

	NOVA_SETTER(Object2D, SetScale, vec2);

	Scope GetModule() {
		Scope scope;
		NOVA_BIND_METHOD(GetGlobalPosition);
		NOVA_BIND_METHOD(GetGlobalScale);
		NOVA_BIND_METHOD(GetGlobalRotation);
		NOVA_BIND_METHOD(SetGlobalPosition);
		NOVA_BIND_METHOD(SetGlobalScale);
		NOVA_BIND_METHOD(SetGlobalRotation);
		NOVA_BIND_METHOD(GetPosition);
		NOVA_BIND_METHOD(GetRotation);
		NOVA_BIND_METHOD(GetScale);
		NOVA_BIND_METHOD(SetPosition);
		NOVA_BIND_METHOD(SetRotation);
		NOVA_BIND_METHOD(SetScale);
		return scope;
	}

}

void Object2D::OnNovaObject(Scope& scope) {
	NOVA_BIND_PROPERTY(position);
	NOVA_BIND_PROPERTY(rotation);
	NOVA_BIND_PROPERTY(scale);
	Scope s = nova_object2D::GetModule();

	for (std::pair<std::string, Value> pair : s.GetAll()) {
		scope.Set(pair.first, pair.second);
	}

}