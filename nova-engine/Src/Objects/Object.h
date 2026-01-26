#ifndef NOVA_OBJECT_H
#define NOVA_OBJECT_H

#include <string>
#include <vector>

class Interpretor;
class InputEvent;
struct CPPObject;
struct Scope;

class Object {
public:

	void Ready();
	void Update(float deltaTime);
	void Input(InputEvent* e);
	void Draw();

	void AddChild(Object* object);
	Object* FindChild(const std::string& name);
	Object* GetChild(int index);
	std::vector<Object*>& GetChildren();
	Object* GetParent();
	CPPObject GetNovaObject();

	bool paused = false;
	bool visible = true;

	std::string name = GetClassName();

	virtual std::string GetClassName() const { return "Object"; };
	
	Interpretor* script = nullptr;

	virtual ~Object() = default;

protected:

	std::vector<Object*> children = {};
	Object* parent = nullptr;

	virtual void OnReady() {};
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnInput(InputEvent* e) {};
	virtual void OnDraw() {};
	virtual void OnNovaObject(Scope& inherited) {};

};


#endif