#include "Interpretor.h"
#include "../../Core/Engine.h"
void Interpretor::Init() {
	if (not program->valid_program) {
		PushError("Parser errors detected, script will not run");
		return;
	}
	scope = new Scope();
	// std lib
	for (StmtNode* stmt : program->Statements) {
		EvaluateStatement(stmt);
	}
}

void Interpretor::PushError(const std::string& message) {
	Engine* engine = Engine::GetInstance();
	engine->PushError("[Interpretor] " + message);
};