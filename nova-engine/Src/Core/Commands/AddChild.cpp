#include "AddChild.h"
#include "../Engine.h"
#include "../../Scene/SceneParser.h"
#include "../../Scene/Scene.h"

void AddChildCommand::Execute(const std::vector<std::string>& args) {
	if (args.size() != 2) {
		Engine::GetInstance()->PushError("Usage: " + Usage());
		return;
	}
	if (Engine::cmd_current_obj == nullptr) {
		Engine::GetInstance()->PushError("No object created with new command");
		return;
	}
	if (Engine::cmd_current_obj->kind != SceneEntryInst::Kind::Object) {
		Engine::GetInstance()->PushError("Last created entry is not an object");
		return;
	}
	
	Object* obj = static_cast<Object*>(Engine::cmd_current_obj->ptr);
	Object* parent = nullptr;  
	if (args[1] == "") {
		parent = Engine::GetInstance()->scene->root;
	}
	else {
		parent = Engine::GetInstance()->scene->root->GetChild(args[1]);
	}
	if (parent == nullptr) {
		Engine::GetInstance()->PushError("Parent object not found: " + args[1]);
		return;
	}
	parent->AddChild(obj);
}
