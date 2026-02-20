#include "PrintTree.h"

#include "../Engine.h"
#include "../../Scene/Scene.h"

void PrintTreeCommand::Execute(const std::vector<std::string>& args) {
	Object* obj = nullptr;
	if (args.size() == 2) {
		obj = Engine::GetInstance()->scene->root->FindChild(args[1]);
		if (!obj) {
			Engine::GetInstance()->PushError("Object not found: " + args[1]);
			return;
		}
	}
	else {
		obj = Engine::GetInstance()->scene->root;
	}
	
	std::string output = obj->name + " (" + obj->GetClassName() + ")\n";
	int current_depth = 1;
	for (Object* child : obj->GetChildren()) {
		output += std::string(current_depth * 2, ' ') + child->name + " (" + child->GetClassName() + ")\n";
		if (!child->GetChildren().empty()) {
			std::vector<std::pair<Object*, int>> stack;
			for (Object* grandchild : child->GetChildren()) {
				stack.push_back({ grandchild, current_depth + 1 });
			}
			while (!stack.empty()) {
				auto [current, depth] = stack.back();
				stack.pop_back();
				output += std::string(depth * 2, ' ') + current->name + " (" + current->GetClassName() + ")\n";
				for (Object* grandchild : current->GetChildren()) {
					stack.push_back({ grandchild, depth + 1 });
				}
			}
		}
	}

	Engine::GetInstance()->PushMessage(output);

}

