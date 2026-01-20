#include "Interpretor.h"

void Interpretor::Init() {
	scope = new Scope();
	// std lib
	for (StmtNode* stmt : program->Statements) {
		EvaluateStatement(stmt);
	}
}