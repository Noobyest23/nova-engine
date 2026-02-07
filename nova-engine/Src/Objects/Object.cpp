#include "Object.h"
#include "../Core/Engine.h"
#include "../Assets/Script/Script.h"
#include "../../../nova-script/NovaScript/Interpretor/Value.h"
#include "../../../nova-script/NovaScript/Library/nova_std_macro.h"

void Object::Ready() {
	if (paused) return;
	for (Object* child : GetChildren()) {
		child->Ready();
	}
	if (script) {
		script->Call("Ready", {});
		return;
	}
	OnReady();
}

void Object::Update(float deltaTime) {
	if (paused) return;
	for (Object* child : GetChildren()) {
		child->Update(deltaTime);
	}
	if (script) {
		Value* dt = new Value(deltaTime);
		dt->is_manually_created = true;
		script->Call("Update", {dt});
		return;
	}
	OnUpdate(deltaTime);
}

void Object::Input(InputEvent* e) {
	if (paused) return;
	for (Object* child : GetChildren()) {
		child->Input(e);
	}
	OnInput(e);
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
	object->parent = this;
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

void Object::OnLoad(std::unordered_map<std::string, void*> values) {
	for (std::pair<std::string, void*> pair : values) {
		if (pair.first == "visible") {
			visible = *static_cast<bool*>(pair.second);
		}
		else if (pair.first == "paused") {
			paused = *static_cast<bool*>(pair.second);
		}
		else if (pair.first == "script") {
			Script* new_script = static_cast<Script*>(pair.second);
			SetScript(new_script);
		}
		else if (pair.first == "parent") {
			parent = static_cast<Object*>(pair.second);
			parent->AddChild(this);
		}
	}
}

void Object::SetScript(Script* new_script) {
	if (script) {
		script->Release();
	}

	if (new_script) {
		new_script->AddRef();
		auto obj = GetNovaObject();
		new_script->Set("this", obj);
	}

	script = new_script;
}

namespace nova_object {
	static void PushError(const std::string& message) {
		Engine::GetInstance()->PushError("[NovaScript Object] " + message);
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

	Scope GetModule() {
		Scope scope;
		NOVA_BIND_METHOD(AddChild);
		NOVA_BIND_METHOD(FindChild);
		NOVA_BIND_METHOD(GetChild);
		NOVA_BIND_METHOD(GetChildren);
		NOVA_BIND_METHOD(GetParent);
		return scope;
	}

}

CPPObject Object::GetNovaObject() {
	Scope scope = nova_object::GetModule();
	CPPObject object;
	object.ptr = this;
	NOVA_BIND_PROPERTY(visible);
	NOVA_BIND_PROPERTY(paused);
	NOVA_BIND_PROPERTY(name);

	object.scope = scope;
	return object;
}