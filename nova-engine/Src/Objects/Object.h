#ifndef NOVA_OBJECT_H
#define NOVA_OBJECT_H

#include <string>
#include <vector>
#include <unordered_map>

class Script;
class InputEvent;
struct CPPObject;
struct Scope;
struct Value;

using LoadableValues = std::unordered_map<std::string, void*>;
using LValuePair = std::pair<std::string, void*>;

class Object {
public:

	Object() { name = "Object"; }

	void Ready();
	void Update(float deltaTime);
	void Input(InputEvent* e);
	void Draw();
	void Load(LoadableValues values) { OnLoad(values); }
	void Delete();

	void MoveChild(int from, int to);
	void AddChild(Object* object);
	Object* FindChild(const std::string& name);
	Object* GetChild(int index);
	Object* GetChild(const std::string& path);
	std::vector<Object*>& GetChildren();
	int GetChildrenCount();
	Object* GetParent();

	bool paused = false;
	bool visible = true;

	std::string name;

	virtual std::string GetClassName() const { return "Object"; };
	virtual CPPObject GetNovaObject();

	void SetScript(Script* script);
	Script* GetScript() const { return script; }
	

	virtual ~Object() {};

	bool waiting_for_deletion = false;
protected:

	std::vector<Object*> children = {};
	Object* parent = nullptr;
	Script* script = nullptr;
	

	virtual void OnReady() {};
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnInput(InputEvent* e) {};
	virtual void OnDraw() {};
	virtual void OnLoad(std::unordered_map<std::string, void*> values);
	virtual void OnDelete() {};
};


#endif