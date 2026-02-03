#include "Interpretor.h"
#include "../parser/Lexer.h"
#include "../parser/parser.h"
#include "../Library/nova_std.h"
#include "../ASTNodes/StmtNode.h"

#define es_decl(type) void Interpretor::EvaluateStatement(type)
#define es(type) if (type n = dynamic_cast<type>(node)) { EvaluateStatement(n); return; }

#define STR(type) #type



es_decl(StmtNode* node) {
	if (!node) {
		PushError("Statement was null");
		return;
	}
	es(VarDeclNode*)
	es(FuncDeclNode*)
	es(IfStmtNode*)
	es(TypeDeclNode*)
	es(IncludeNode*)
	es(ReturnStmtNode*)
	es(BreakPointNode*)
	es(ASTPrintNode*)
	es(ExprAsStmt*)
	PushError("Unrecognized Statement " + node->Print());
}

es_decl(VarDeclNode* node) {
	// Add a variable to the scope
	Value value;
	if (node->right) {
		value = EvaluateExpression(node->right);
	}

	scope->Set(node->identifier, value);
}

es_decl(FuncDeclNode* node) {
	Value var(node);
	scope->Set(node->func_id, var);
}

es_decl(IfStmtNode* node) {
	Value value = EvaluateExpression(node->expression);
	
	if (value.IsBool()) {
		if (value.GetBool()) {
			for (StmtNode* stmt : node->body) {
				EvaluateStatement(stmt);
			}
		}
		else {
			for (StmtNode* stmt : node->else_body) {
				EvaluateStatement(stmt);
			}
		}
	}
}

es_decl(TypeDeclNode* node) {
	Value val(node);
	scope->Set(node->type_name, val);
}

es_decl(IncludeNode* node) {
	// Parse through the file
	Value val = EvaluateExpression(node->file_path);
	if (!val.IsString()) {
		PushError("Filepath must evaluate to string " + node->Print());
		return;
	}
	std::string filepath = val.GetString();
	if (!filepath.ends_with(".ns")) {
		Value val;
		auto it = modules.find(filepath);
		if (it != modules.end()) {
			NovaType type = modules[filepath]->GetModule();
			val = Value(type);
		}
		if (val.data.index() == 0) {
			PushError("Cannot include standard lib " + node->file_path->Print() + " because it can't be found");
			return;
		}

		if (VariableNode* var = dynamic_cast<VariableNode*>(node->as)) {
			scope->Set(var->identifier, val);
		}
		else {
			for (std::pair<std::string, Value> pair : std::get<Scope>(val.data).variables) {
				scope->Set(pair.first, pair.second);
			}
		}
	}
	else {
		filepath = Callbacker::_proj_path + filepath;
		Lexer lexer(filepath.c_str());
		ProgramNode* program = nullptr;
		{
			std::vector<Token> tokens = lexer.Parse();
			Parser parser(tokens);
			program = parser.Parse();
		}

		if (!program) {
			PushError("Error including file " + node->Print());
		}

		Interpretor interpretor(program);
		Scope* s = interpretor.GetScopeAsObj();

		
		if (VariableNode* var = dynamic_cast<VariableNode*>(node->as)) {
			scope->Set(var->identifier, val);
		}
		else {
			for (std::pair<std::string, Value> pair : s->variables) {
				scope->Set(pair.first, pair.second);
			}
		}
	}
}

es_decl(ReturnStmtNode* node) {
	return_flag = true;
	return_val = EvaluateExpression(node->return_value);
}

es_decl(BreakPointNode* node) {
	if (program) {
		Callbacker::PushError("[NovaScript] Breakpoint Hit!", 2);
		Callbacker::PushError(("[NovaScript] AST of this statement: " + node->stmt->Print()).c_str(), 1);
		Callbacker::PushError("[NovaScript] Current variables in scope: ", 1);
		for (std::pair<std::string, Value> var : scope->variables) {
			Callbacker::PushError(("[NovaScript] " + var.first + " = " + var.second.ToString()).c_str(), 1);
		}
	}
	EvaluateStatement(node->stmt);
}

es_decl(ASTPrintNode* node) {
	if (program) {
		Callbacker::PushError("[NovaScript] Super Breakpoint Hit!", 2);
		Callbacker::PushError(("[NovaScript] AST of this script: " + program->Print()).c_str(), 1);
	}
}


es_decl(ExprAsStmt* node) {
	EvaluateExpression(node->expr);
}