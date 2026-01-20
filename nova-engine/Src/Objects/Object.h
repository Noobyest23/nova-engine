#ifndef NOVA_OBJECT_H
#define NOVA_OBJECT_H

#include <string>
#include <vector>

class Object {
public:

	void Ready();
	void Update();
	//void Input();
	void Draw();

	void AddChild(Object* object);
	Object* FindChild(const std::string& name);
	std::vector<Object*>& GetChildren();
	Object* GetParent();

	bool paused = false;
	bool visible = true;

	std::string name = "";

protected:

	std::vector<Object*> children = {};
	Object* parent;

	virtual void OnReady() {};
	virtual void OnUpdate() {};
	virtual void OnDraw() {};
	virtual std::string GetClassName();

};


#endif