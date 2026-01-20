#include "Object.h"

void Object::Ready() {
	for (const auto child : GetChildren()) {
		child->Ready();
	}
	OnReady();
}

void Object::Update() {
	if (paused) {
		return;
	}
	for (const auto child : GetChildren()) {
		child->Update();
	}
	OnUpdate();
}

void Object::Draw() {
	if (!visible) {
		return;
	}
	for (const auto child : GetChildren()) {
		child->Draw();
	}
	OnDraw();
}

void Object::AddChild(Object* object) {
	children.push_back(object);
}

Object* Object::FindChild(const std::string& object) {
	for (const auto child : GetChildren()) {
		if (child->name == object) {
			return child;
		}
	}

	return nullptr;
}

std::vector<Object*>& Object::GetChildren() {
	return children;
}

Object* Object::GetParent() {
	return parent;
}

std::string Object::GetClassName() {
	return "Object";
}
