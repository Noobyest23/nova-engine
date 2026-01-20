#include "Interpretor.h"
#include "../parser/Lexer.h"
#include "../parser/parser.h"

#include "../Library/nova_std.h"

#define es_decl(type) void Interpretor::EvaluateStatement(type)
#define es(type) if (type n = dynamic_cast<type>(node)) { EvaluateStatement(n); return; }

#define STR(type) #type

#define iiop(_op) if (node->op == STR(_op)) {val _op rhs.GetInt();}
#define ifop(_op) if (node->op == STR(_op)) {val _op rhs.GetFloat();}
#define ffop(_op) if (node->op == STR(_op)) {val _op rhs.GetInt();}
#define fiop(_op) if (node->op == STR(_op)) {val _op rhs.GetFloat();}
#define bop(_op) if (node->op == STR(_op)) {val _op rhs.GetBool();}

es_decl(StmtNode* node) {
	if (!node) {
		PushError("Statement was null");
		return;
	}
	es(VarDeclNode*)
	es(AssignmentNode*)
	es(FuncDeclNode*)
	es(CompoundOp*)
	es(IfStmtNode*)
	es(TypeDeclNode*)
	es(IncludeNode*)
	es(ReturnStmtNode*)
	es(BreakPointNode*)
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

es_decl(AssignmentNode* node) {
	Value* lhs = EvaluateExpressionPtr(node->left);
	if (!lhs || lhs->IsFunction()) {
		PushError("Left side is not a modifiable value " + node->Print());
		return;
	}

	Value rhs = EvaluateExpression(node->right);

	try {
		if (lhs->IsInt()) {
			int& v = lhs->GetInt();
			if (rhs.IsInt()) v = rhs.GetInt();
			else if (rhs.IsFloat()) v = int(rhs.GetFloat());
			else PushError("Cannot convert to int");
			return;
		}

		if (lhs->IsFloat()) {
			float& v = lhs->GetFloat();
			if (rhs.IsInt()) v = float(rhs.GetInt());
			else if (rhs.IsFloat()) v = rhs.GetFloat();
			else PushError("Cannot convert to float");
			return;
		}

		if (lhs->IsBool()) {
			lhs->GetBool() = rhs.GetBool();
			return;
		}

		if (lhs->IsString()) {
			lhs->GetString() = rhs.GetString();
			return;
		}

		PushError("Cannot assign to this type " + node->Print());
	}
	catch (const std::exception& e) {
		PushError(
			std::string(e.what()) + "\n" +
			lhs->ToString() + "\n" +
			rhs.ToString()
		);
	}
}

es_decl(FuncDeclNode* node) {
	Value var(node);
	scope->Set(node->func_id, var);
}

es_decl(CompoundOp* node) {
	// Set a value
	Value* lhs = EvaluateExpressionPtr(node->lhs);
	if (!lhs) {
		PushError("Left side is not a modifyable value " + node->Print());
		return;
	}
	Value rhs = EvaluateExpression(node->rhs);

	try {
		if (lhs) {
			if (lhs->IsInt()) {
				int& val = lhs->GetInt();
				if (rhs.IsInt()) {
					iiop(+=);
					iiop(-=);
					iiop(*=);
					iiop(/=);
					return;
				}
				else if (rhs.IsFloat()) {
					ifop(+=);
					ifop(-=);
					ifop(*=);
					ifop(/=);
					return;
				}
				return;
			}
			if (lhs->IsFloat()) {
				float& val = lhs->GetFloat();
				if (rhs.IsInt()) {
					fiop(+=);
					fiop(-=);
					fiop(*=);
					fiop(/=);
					return;
				}
				else if (rhs.IsFloat()) {
					ffop(+=);
					ffop(-=);
					ffop(*=);
					ffop(/=);
					return;
				}
				return;
			}
			if (lhs->IsString()) {
				std::string& val = lhs->GetString();
				if (node->op == "+=") {
					val += rhs.GetString();
				}
				else {
					PushError("Cannot perform " + node->op + " on a string");
				}
				
				return;
			}

			throw std::exception((std::string("Cannot perform a compound op on a variable of type ") + lhs->Type()).c_str());
		}
		else {
			PushError("Cannot assign to an expression of this type" + node->Print());
		}
	}
	catch (std::exception e) {
		PushError(e.what() + '\n' + lhs->ToString() + '\n' + rhs.ToString());
	}
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
	std::string& filepath = val.GetString();
	if (!filepath.ends_with(".ns")) {
		Value val;
		if (filepath == "io") {
			val = nova_std_io::GetModule();
		}
		if (filepath == "engine") {
			val = nova_std_engine::get_module();
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
			for (std::pair<std::string, Value> pair : scope->variables) {
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
	Engine* engine = Engine::GetInstance();
	engine->PushError("[NovaScript] Breakpoint Hit!");
	engine->PushMessage("[NovaScript] AST of this statement: " + node->stmt->Print(), true);
	engine->PushMessage("[NovaScript] Current variables in scope: ");
	for (std::pair<std::string, Value> var : scope->variables) {
		engine->PushMessage("[NovaScript] " + var.first + " = " + var.second.ToString(), true);
	}
	EvaluateStatement(node->stmt);
}

es_decl(ExprAsStmt* node) {
	EvaluateExpression(node->expr);
}