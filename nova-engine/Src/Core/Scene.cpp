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
	root.Update(0.16f);
}

void Scene::Draw() {
	root.Draw();
}

Camera2D* Scene::GetActiveCamera2D() {
	Camera2D* cam = nullptr;
	for (Object* child : root.GetChildren()) {
		cam = RecursiveGetCam2D(child);
		if (cam) {
			return cam;
		}
	}
	return cam;
}

void Scene::LoadScene(const std::string& scene_file) {
	// haha ill do this later
	// DEFINITLY
}

Camera2D* Scene::RecursiveGetCam2D(Object* obj) {
	if (!obj) return nullptr;

	if (Camera2D* cam = dynamic_cast<Camera2D*>(obj)) {
		if (cam->enabled) return cam;
	}

	for (Object* child : obj->GetChildren()) {
		if (Camera2D* cam = dynamic_cast<Camera2D*>(RecursiveGetCam2D(child))) {
			return cam;
		}
	}

	return nullptr;
}
