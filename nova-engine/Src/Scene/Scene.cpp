#include "Scene.h"
#include "SceneParser.h"
Scene::Scene(const std::string& scene_file) {
	root = LoadScene(scene_file);
}

void Scene::ChangeScene(const std::string& scene_file) {
	if (root) {
		root->Delete();
	}
	root = LoadScene(scene_file);
}

void Scene::Ready() {
	root->Ready();
}

void Scene::Update(float deltaTime) {
	root->Update(deltaTime);
}

void Scene::Input(InputEvent* e) {
	root->Input(e);
}

void Scene::Draw() {
	root->Draw();
}

Camera2D* Scene::GetActiveCamera2D() {
	Camera2D* cam = nullptr;
	for (Object* child : root->GetChildren()) {
		cam = RecursiveGetCam2D(child);
		if (cam) {
			return cam;
		}
	}
	return cam;
}

Object* Scene::LoadScene(const std::string& scene_file) {
	SceneParser parser;
	return parser.Parse(scene_file);
}

Camera2D* Scene::RecursiveGetCam2D(Object* obj) {
	if (!obj) return nullptr;

	if (Camera2D* cam = dynamic_cast<Camera2D*>(obj)) {
		if (cam->active) return cam;
	}

	for (Object* child : obj->GetChildren()) {
		if (Camera2D* cam = dynamic_cast<Camera2D*>(RecursiveGetCam2D(child))) {
			return cam;
		}
	}

	return nullptr;
}

void Scene::AppendScene(const std::string& scene_file, Object* child) {
	SceneParser parser;
	Object* temp = parser.Parse(scene_file);
	for (Object* object : temp->GetChildren()) {
		child->AddChild(object);
	}
	delete temp;
}

void Scene::Shutdown() {
	if (root) {
		root->Delete();
		root = nullptr;
	}
}


