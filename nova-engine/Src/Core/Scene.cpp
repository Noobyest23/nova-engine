#include "Scene.h"

Scene::Scene(const std::string& scene_file) {
	LoadScene(scene_file);
}

void Scene::ChangeScene(const std::string& scene_file) {
	LoadScene(scene_file);
}

void Scene::Ready() {
	root.Ready();
}

void Scene::Update() {
	root.Update(16.0f);
}

void Scene::Draw() {
	root.Draw();
}

void Scene::LoadScene(const std::string& scene_file) {
	// haha ill do this later
	// DEFINITLY
}
