#include "LoadScene.h"
#include "../Engine.h"
#include "../../Scene/Scene.h"
void LoadSceneCommand::Execute(const std::vector<std::string>& args) {
	Engine* engine = Engine::GetInstance();
	if (args.size() < 2) {
		engine->PushError("Usage: " + Usage());
	}
	Scene* new_scene = new Scene(args[1]);
	if (new_scene) {
		engine->scene->Shutdown();
		engine->scene = new_scene;
	}
	else {
		engine->PushError("Scene is null!");
	}
}
