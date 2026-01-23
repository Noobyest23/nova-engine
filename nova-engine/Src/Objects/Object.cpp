#include "Object.h"
#include "../NovaScript/Interpretor/Scope.h"
#include "../NovaScript/Interpretor/Value.h"
#include "../NovaScript/Library/nova_std_macro.h"
#include "../Core/Engine.h"

void Object::Ready() {
	if (paused) return;
	for (Object* child : GetChildren()) {
		child->Ready();
	}
	OnReady();
}

void Object::Update(float deltaTime) {
	if (paused) return;
	for (Object* child : GetChildren()) {
		child->Update(deltaTime);
	}
	OnUpdate(deltaTime);
}

void Object::Draw() {
	if (!visible) return;
	for (auto& child : GetChildren()) {
		child->Draw();
	}
	OnDraw();
}

void Object::AddChild(Object* object) {
	if (paused) return;
	children.push_back(object);
}

Object* Object::FindChild(const std::string& object) {
	for (auto& child : GetChildren()) {
		if (child->name == object) {
			return child;
		}
	}

	return nullptr;
}

Object* Object::GetChild(int index) {
	if (index < children.size()) {
		return children[index];
	}
	return nullptr;
}

std::vector<Object*>& Object::GetChildren() {
	return children;
}

Object* Object::GetParent() {
	return parent;
}

namespace nova_object {
	static void PushError(const std::string& message) {
		Engine::GetInstance()->PushError("[NovaScript Object] " + message);
	}

	nova_std_decl(Ready) {
		req_args(1);
		objget(obj, Object, 0);
		obj->Ready();
		return null_value;
	}

	nova_std_decl(Update) {
		req_args(2);
		objget(obj, Object, 0);
		floatget(delta, 1);
		obj->Update(delta);
		return null_value;
	}

	nova_std_decl(AddChild) {
		req_args(2);
		objget(obj, Object, 0);
		objget(child, Object, 1);
		obj->AddChild(child);
		return null_value;
	}

	nova_std_decl(FindChild) {
		req_args(2);
		objget(obj, Object, 0);
		strget(name, 1);
		Object* child = obj->FindChild(name);
		return Value(child->GetNovaObject());
	}

	nova_std_decl(GetChild) {
		req_args(2);
		objget(obj, Object, 0);
		intget(index, 1);

		Object* child = obj->GetChild(index);
		return Value(child->GetNovaObject());
	}

	nova_std_decl(GetChildren) {
		req_args(1);
		objget(obj, Object, 0);

		std::vector<Object*>& children = obj->GetChildren();
		std::vector<Value> ret_val;
		for (Object* child : children) {
			ret_val.push_back(child->GetNovaObject());
		}
		return ret_val;
	}

	nova_std_decl(GetParent) {
		req_args(1);
		objget(obj, Object, 0);
		Value ret = Value(CPPObject(obj->GetParent()->GetNovaObject()));
		return ret;
	}

	nova_std_decl(GetClassName) {
		req_args(1);
		objget(obj, Object, 0);
		return Value(obj->GetClassName());
	}

	Scope GetModule() {
		Scope scope;
		scope.Set("Ready", Ready);
		scope.Set("Update", Update);
		scope.Set("AddChild", AddChild);
		scope.Set("FindChild", FindChild);
		scope.Set("GetChild", GetChild);
		scope.Set("GetChildren", GetChildren);
		scope.Set("GetClassName", GetClassName);
		return scope;
	}

}

CPPObject Object::GetNovaObject() {
	CPPObject self;
	Scope scope = nova_object::GetModule();
	scope.Set("name", Value(CPPVariable(std::reference_wrapper<std::string>(name))));
	scope.Set("paused", Value(CPPVariable(std::reference_wrapper<bool>(paused))));
	scope.Set("visible", Value(CPPVariable(std::reference_wrapper<bool>(visible))));
	self.scope = scope;
	OnNovaObject(scope);

	if (script) {
		Scope* nova_script_inherited = script->GetScopeAsObj();
		for (std::pair<std::string, Value> pair : nova_script_inherited->GetAll()) {
			scope.Set(pair.first, pair.second);
		}
	}

	self.ptr = this;
	return self;
}