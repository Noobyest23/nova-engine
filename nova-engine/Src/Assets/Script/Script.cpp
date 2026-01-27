#include "Script.h"
#include "../../Core/Engine.h"

Script::Script(const std::string& filepath) {
	interpretor = static_cast<InterpretorHandle>(BuildScript(filepath.c_str()));
}

void Script::OnDestroy() {
	DestroyScript(interpretor);
}
