#include "Script.h"
#include "../../Core/Engine.h"
#include "Added Modules/EngineModule.h"

Script::Script(const std::string& filepath) {
	interpretor = static_cast<InterpretorHandle>(BuildScript(filepath.c_str()));
	PushModule(interpretor, new NovaEngineModule);
	ExecuteScript(interpretor);
}

void Script::Set(const std::string& name, Value val) {
	PushVariable(interpretor, name.c_str(), &val);
}

Value* Script::Get(const std::string& name) {
	return static_cast<Value*>(GetVariable(interpretor, name.c_str()));
}

Scope* Script::GetAll() {
	return static_cast<Scope*>(GetScope(interpretor));
}

Value Script::Call(const std::string& name, std::vector<Value*> args) {
	ValueHandle val = CallFunc(interpretor, name.c_str(), &args);
	Value ret = *static_cast<Value*>(val);
	delete val;
	return ret;
}

void Script::OnDestroy() {
	DestroyScript(interpretor);
}
