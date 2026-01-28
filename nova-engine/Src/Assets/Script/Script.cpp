#include "Script.h"
#include "../../Core/Engine.h"
#include "Added Modules/EngineModule.h"

Script::Script(const std::string& filepath) {
	interpretor = static_cast<InterpretorHandle>(BuildScript(filepath.c_str()));
	PushModule(interpretor, new NovaEngineModule);

	ExecuteScript(interpretor);
}

void Script::OnDestroy() {
	DestroyScript(interpretor);
}
