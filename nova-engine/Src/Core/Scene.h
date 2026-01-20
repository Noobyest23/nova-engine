#ifndef NOVA_SCENE_H
#define NOVA_SCENE_H

#include "../Objects/Object.h"

class Scene {
public:

	Scene(const std::string& scene_file);

	void ChangeScene(const std::string& scene_file);

	void Ready();
	void Update();
	void Draw();

private:

	void LoadScene(const std::string& scene_file);

	Object root;

};


#endif
