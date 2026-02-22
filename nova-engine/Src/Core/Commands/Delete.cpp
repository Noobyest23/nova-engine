#include "Delete.h"
#include "../Engine.h"
#include "../../Scene/Scene.h"

void DeleteCommand::Execute(const std::vector<std::string>& args) {
	if (args.size() < 1) {
		Engine::GetInstance()->PushError("Usage: " + Usage());
		return;
	}

	Object* obj = Engine::GetInstance()->scene->root->GetChild(args[1]);
	if (obj) {
		obj->Delete();
	}
}