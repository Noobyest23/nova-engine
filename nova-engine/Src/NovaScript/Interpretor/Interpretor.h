#ifndef NOVASCRIPT_INTERPRETOR_H
#define NOVASCRIPT_INTERPRETOR_H

#include "Scope.h"
#include "../ASTNodes/ProgramNode.h"
#include "../../Core/Engine.h"

#include "../Parser/Parser.h"
#include "../Parser/Lexer.h"

class Interpretor {
public:
	Interpretor(ProgramNode* program) : program(program) { Init(); };
	Interpretor(const std::string& filepath) {
		Lexer lexer(filepath.c_str());
		auto tokens = lexer.Parse();
		Parser parser(tokens);
		program = parser.Parse();

		Init();
	}
	~Interpretor() { program->Delete(); while (scope) { PopScope(); } };

	// Executes the body of the script. Initializes the global scope
	void Init();

	// Calls a Function from nova script
	Value Call(const std::string& func_name, std::vector<Value*>& args) {
		Value* func = scope->Get(func_name);
		if (func->data.index() == 8) { // cpp function
			CPPFunction f = std::get<CPPFunction>(func->data);
			return f(args);
		}
		else { // nova function
			NovaFunction f = std::get<NovaFunction>(func->data);
			Scope* s = new Scope(scope);
			scope = s;
			for (int i = 0; i < f->args.size(); i++) {
				s->Set(f->args[i], *args[i]);
			}

			for (StmtNode* stmt : f->body) {
				EvaluateStatement(stmt);
				if (return_flag) {
					for (Value* arg : args) {
						arg->Release();
					}
					return_flag = false;
					PopScope();
					return return_val;
				}
			}

			for (Value* arg : args) {
				arg->Release();
			}

			PopScope();
			return nullval;
		}
	}
	/*Value* CallPtr(const std::string& func_name, std::vector<Value*>& args) {
		Value* func = scope->Get(func_name);
		if (func->data.index() == 8) { // cpp function
			CPPFunction f = std::get<CPPFunction>(func->data);
			return f(args);
		}
		else { // nova function
			NovaFunction f = std::get<NovaFunction>(func->data);
			Scope* s = new Scope(scope);
			scope = s;
			for (int i = 0; i < f->args.size(); i++) {
				s->Set(f->args[i], *args[i]);
			}

			for (StmtNode* stmt : f->body) {
				EvaluateStatement(stmt);
				if (return_flag) {
					for (Value* arg : args) {
						arg->Release();
					}
					return_flag = false;
					PopScope();
					return return_val_ptr;
				}
			}

			for (Value* arg : args) {
				arg->Release();
			}

			PopScope();
			return nullptr;
		}
	}*/

	Value* Get(const std::string& var_name) {
		return scope->Get(var_name);
	}

	Scope* GetScopeAsObj() {
		return scope;
	}

	void PushScope() {
		Scope* n_scope = new Scope(scope);
		scope = n_scope;
	}

	void EvaluateStatement(StmtNode*);
	Value EvaluateExpression(ExprNode*);
private:

	#pragma region Evaluate Statement Overloads
	
	void EvaluateStatement(VarDeclNode*);
	void EvaluateStatement(AssignmentNode*);
	void EvaluateStatement(FuncDeclNode*);
	void EvaluateStatement(CompoundOp*);
	void EvaluateStatement(IfStmtNode*);
	void EvaluateStatement(TypeDeclNode*);
	void EvaluateStatement(IncludeNode*);
	void EvaluateStatement(ReturnStmtNode*);
	void EvaluateStatement(BreakPointNode*);
	void EvaluateStatement(ExprAsStmt*);
	#pragma endregion
	#pragma region Evaluate Expression Overloads
	
	Value EvaluateExpression(VariableNode*);
	Value EvaluateExpression(OpNode*);
	Value EvaluateExpression(FuncCallNode*);
	Value EvaluateExpression(TernaryNode*);
	Value EvaluateExpression(IntLiteralNode*);
	Value EvaluateExpression(FloatLiteralNode*);
	Value EvaluateExpression(BoolLiteralNode*);
	Value EvaluateExpression(StringLiteralNode*);
	Value EvaluateExpression(ArrayLiteralNode*);
	Value EvaluateExpression(DotAccessNode*);
	Value EvaluateExpression(Vector2LiteralNode*);
	Value EvaluateExpression(Vector3LiteralNode*);
	Value EvaluateExpression(Vector4LiteralNode*);
	Value EvaluateExpression(ArrayAccessNode*);

	Value* EvaluateExpressionPtr(ExprNode*);
	Value* EvaluateExpressionPtr(VariableNode*);
	Value* EvaluateExpressionPtr(TernaryNode*);
	Value* EvaluateExpressionPtr(DotAccessNode*);
	Value* EvaluateExpressionPtr(ArrayAccessNode*);

	#pragma endregion

	ProgramNode* program;
	Scope* scope = nullptr;

	void PopScope() {
		if (scope->parent) {
			Scope* parent = scope->parent;
			delete scope;
			scope = parent;
		}
		else {
			delete scope;
			scope = nullptr;
		}
	}

	void PushError(const std::string& message) {
		Engine* engine = Engine::GetInstance();
		engine->PushError("[Interpretor] " + message);
	};

	bool return_flag = false;
	Value return_val = Value();
	Value* return_val_ptr = nullptr;

	Value nullval = Value();
};

// Todo
// signal support
// std lib stuff
// vector
// array

#endif