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
}

void Object2D::SetGlobalScale(const glm::vec2& new_global_scale) {
	if (!parent) { scale = new_global_scale; return; };
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		glm::vec2 p_scale = p_toodee->GetGlobalScale();
		scale = new_global_scale / p_scale;
		return;
	}
}

void Object2D::SetGlobalRotation(float new_global_rotation) {
	if (!parent) { rotation = new_global_rotation; return; }
	if (Object2D* p_toodee = dynamic_cast<Object2D*>(parent)) {
		rotation = new_global_rotation - p_toodee->GetGlobalRotation();
		return;
	}
	rotation = new_global_rotation;
}

glm::mat3 Object2D::GetLocalTransform() const {
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

	return T * R * S;


}

glm::mat3 Object2D::GetGlobalTransform() const {
	if (!parent) return GetLocalTransform();
	if (Object2D* p = dynamic_cast<Object2D*>(parent)) {
		return p->GetGlobalTransform() * GetLocalTransform();
	}
	return GetLocalTransform();
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

	Scope GetModule() {
		Scope scope;
		NOVA_BIND_METHOD(GetGlobalPosition);
		NOVA_BIND_METHOD(GetGlobalScale);
		NOVA_BIND_METHOD(GetGlobalRotation);
		NOVA_BIND_METHOD(SetGlobalPosition);
		NOVA_BIND_METHOD(SetGlobalScale);
		NOVA_BIND_METHOD(SetGlobalRotation);
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