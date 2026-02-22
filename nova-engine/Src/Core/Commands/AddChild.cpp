#include "AddChild.h"
#include "../Engine.h"
#include "../../Scene/SceneParser.h"
#include "../../Scene/Scene.h"

void AddChildCommand::Execute(const std::vector<std::string>& args) {
	if (args.size() < 2) {
		Engine::GetInstance()->PushError("Usage: " + Usage());
		return;
	}
	if (Engine::command_objects[args[1]] == nullptr) {
		Engine::GetInstance()->PushError("No object created with new command");
		return;
	}
	if (Engine::command_objects[args[1]]->kind != SceneEntryInst::Kind::Object) {
		Engine::GetInstance()->PushError("Last created entry is not an object");
		return;
	}
	
	Object* obj = static_cast<Object*>(Engine::command_objects[args[1]]->ptr);
	Object* parent = nullptr;  
	std::string p_path = (args.size() == 3 ? args[2] : "");
	if (p_path == "") {
		parent = Engine::GetInstance()->scene->root;
	}
	else {
		parent = Engine::GetInstance()->scene->root->GetChild(p_path);
	}
	if (parent == nullptr) {
		Engine::GetInstance()->PushError("Parent object not found: " + p_path);
		return;
	}
	parent->AddChild(obj);
	Engine::command_objects.erase(args[1]);
}
