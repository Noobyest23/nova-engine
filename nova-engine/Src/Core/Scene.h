#ifndef NOVA_SCENE_H
#define NOVA_SCENE_H

#include "../Objects/Object.h"
#include "../Objects/2D/Camera2D.h"
class Scene {
public:

	Scene(const std::string& scene_file);

	void ChangeScene(const std::string& scene_file);

	void Ready();
	void Update();
	void Draw();

	Camera2D* GetActiveCamera2D();

//private:

	void LoadScene(const std::string& scene_file);

	Object root;

	Camera2D* RecursiveGetCam2D(Object*);

};


#endif
