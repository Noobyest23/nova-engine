#include "Interpretor.h"

#define ee_decl(type) Value Interpretor::EvaluateExpression(type)
#define eep_decl(type) Value* Interpretor::EvaluateExpressionPtr(type)
#define ee(type) if (type n = dynamic_cast<type>(node)) {return EvaluateExpression(n);}
#define eep(type) if (type n = dynamic_cast<type>(node)) {return EvaluateExpressionPtr(n);}

#define STR(type) #type

#define iiop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetInt() _op rhs.GetInt());}
#define ifop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetInt() _op rhs.GetFloat());}
#define ffop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetFloat() _op rhs.GetInt());}
#define fiop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetFloat() _op rhs.GetFloat());}
#define bop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetBool() _op rhs.GetBool());}

ee_decl(ExprNode* node) {
	ee(VariableNode*)
	ee(OpNode*)
	ee(FuncCallNode*)
	ee(TernaryNode*)
	ee(IntLiteralNode*)
	ee(FloatLiteralNode*)
	ee(BoolLiteralNode*)
	ee(StringLiteralNode*)
	ee(ArrayLiteralNode*)
	ee(DotAccessNode*)
	ee(Vector2LiteralNode*)
	ee(Vector3LiteralNode*)
	ee(Vector4LiteralNode*)
	ee(ArrayAccessNode*)
	PushError("Unrecognized Expression " + node->Print() + " (this expression has returned null)");
	return Value();
}

ee_decl(VariableNode* node) {
	Value* val = scope->Get(node->identifier);
	if (val) {
		return *val;
	}
	else {
		return Value();
	}
}

ee_decl(OpNode* node) {
	Value lhs = EvaluateExpression(node->left);
	Value rhs = EvaluateExpression(node->right);

	if (lhs.data.index() == 0) {
		PushError(node->Print() + " Left side evaluated to null");
	}
	if (rhs.data.index() == 0) {
		PushError(node->Print() + " Right side evaluated to null");
	}
	Value r_value;

	try {
		
		if (lhs.IsInt()) {
			if (rhs.IsInt()) {
				iiop(+);
				iiop(-);
				iiop(*);
				iiop(/);
				iiop(>);
				iiop(<);
				iiop(>=);
				iiop(<=);
				iiop(==);
				return r_value;
			}
			else if (rhs.IsFloat()) {
				ifop(+);
				ifop(-);
				ifop(*);
				ifop(/);
				ifop(>);
				ifop(<);
				ifop(>=);
				ifop(<=);
				ifop(==);
				return r_value;
			}
			PushError("Cannot add type to a int " + node->Print());
			return nullval;
		}
		if (lhs.IsFloat()) {
			if (rhs.IsInt()) {
				fiop(+);
				fiop(-);
				fiop(*);
				fiop(/);
				fiop(>);
				fiop(<);
				fiop(>=);
				fiop(<=);
				fiop(==);
				return r_value;
			}
			else if (rhs.IsFloat()) {
				ffop(+);
				ffop(-);
				ffop(*);
				ffop(/);
				ffop(>);
				ffop(<);
				ffop(>=);
				ffop(<=);
				ffop(==);
				return r_value;
			}
			PushError("Cannot add type to a float " + node->Print());
			return nullval;
		}
		if (lhs.IsString()) {
			if (node->op == "+") {
				r_value = Value(lhs.GetString() + rhs.GetString());
				return r_value;
			}
			else {
				PushError("Cannot perform " + node->op + " on a string");
				return nullval;
			}
		}
		if (lhs.IsBool()) {
			if (rhs.IsBool()) {
				bop(==);
				bop(&&);
				bop(||);
				return r_value;
			}
			else {
				PushError("Cannot perform boolean operations with type " + rhs.Type());
			}
		}
		throw std::exception((std::string("Cannot perform an op on a variable of type ") + lhs.Type()).c_str());
	}
	catch (std::exception e) {
		PushError(e.what() + '\n' + lhs.ToString() + '\n' + rhs.ToString());
		return nullval;
	}
}

ee_decl(FuncCallNode* node) {
	Value* func = scope->Get(node->func_id);
	if (func) {
		if (func->IsFunction()) {
			std::vector<Value*> args;
			if (scope->Has("SELF")) {
				args.push_back(scope->Get("SELF"));
			}
			for (ExprNode* arg : node->args) {
				Value* a_value = EvaluateExpressionPtr(arg);
				if (a_value) {
					args.push_back(a_value);
				}
				else {
					a_value = new Value(EvaluateExpression(arg));
					a_value->is_manually_created = true;
					args.push_back(a_value);
				}
			}
			return Call(node->func_id, args);
		}
		else {
			PushError(node->func_id + " is not a function");
		}
	}
	else {
		PushError("Function (" + node->func_id + ") not found in scope");
	}
}

ee_decl(TernaryNode* node) {
	Value val = EvaluateExpression(node->expression);

	if (val.IsBool()) {
		if (val.GetBool()) {
			return EvaluateExpression(node->truthy_value);
		}
		else {
			return EvaluateExpression(node->falsey_value);
		}
	}
	else {
		PushError("Ternary expression does not evaluate to boolean type " + node->Print());
	}
}

ee_decl(IntLiteralNode* node) {
	return Value(node->number);
}

ee_decl(FloatLiteralNode* node) {
	return Value(node->number);
}

ee_decl(BoolLiteralNode* node) {
	return Value(node->value);
}

ee_decl(StringLiteralNode* node) {
	return Value(node->string);
}

ee_decl(ArrayLiteralNode* node) {
	std::vector<Value> arr;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		arr.push_back(val);
	}
	return Value(arr);
}

ee_decl(DotAccessNode* node) {
	Value* obj = EvaluateExpressionPtr(node->left);
	Value* left = nullptr;

	if (!obj) {
		PushError("Could not resolve this obj " + node->Print());
		return Value();
	}

	Scope* n_scope = nullptr;
	if (obj->IsCPP()) {
		n_scope = &std::get<CPPObject>(obj->data).scope;
		n_scope->Set("SELF", std::get<CPPObject>(obj->data));
	}
	else {
		n_scope = &std::get<Scope>(obj->data);
	}

	if (FuncCallNode* func = dynamic_cast<FuncCallNode*>(node->right)) {
		left = n_scope->Get(func->func_id);
		if (!left) {
			PushError(func->func_id + " is not in the scope");
			return Value();
		}
		if (left->IsFunction()) {
			std::vector<Value*> args;
			if (n_scope->Has("SELF")) {
				args.push_back(n_scope->Get("SELF"));
			}
			for (ExprNode* arg : func->args) {
				Value* a_value = EvaluateExpressionPtr(arg);
				if (a_value) {
					args.push_back(a_value);
				}
				else {
					a_value = new Value(EvaluateExpression(arg));
					a_value->is_manually_created = true;
					args.push_back(a_value);
				}
			}
			if (left->IsCPP()) {
				CPPFunction& func = std::get<CPPFunction>(left->data);
				Value r_value = func(args);
				for (Value* arg : args) {
					arg->Release();
				}
				obj->Release();
				return r_value;
			}
			else {
				NovaFunction& func = std::get<NovaFunction>(left->data);
				Scope* p_scope = scope;
				scope = n_scope;

				for (int i = 0; i < func->args.size(); i++) {
					scope->Set(func->args[i], *args[i]);
				}

				for (StmtNode* stmt : func->body) {
					EvaluateStatement(stmt);
					if (return_flag) {
						return_flag = false;
						scope = p_scope;
						for (Value* arg : args) {
							arg->Release();
						}
						obj->Release();
						return return_val;
					}
				}
				scope = p_scope;
				for (Value* arg : args) {
					arg->Release();
				}
				obj->Release();
				return Value();

			}
		}
	}
	else if (VariableNode* var = dynamic_cast<VariableNode*>(node->right)) {
		obj->Release();
		return *n_scope->Get(var->identifier);
	}
	else {
		Scope* p_scope = scope;
		scope = n_scope;
		Value val = EvaluateExpression(node->right);
		obj->Release();
		scope = p_scope;
		return val;
	}
}

ee_decl(Vector2LiteralNode* node) {
	std::vector<Value> values;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		if (val.IsInt() or val.IsFloat() or val.IsVec()) {
			values.push_back(val);
		}
		else {
			PushError("Cannot assign " + val.Type() + " to a Vector");
		}
	}

	switch (values.size()) {
	case 1: {
		if (values[0].data.index() == 12) {
			return glm::vec2(values[0].GetVec2());
		}
		else if (values[0].IsNum()) {
			return glm::vec2(values[0].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector2");
		break;
	}
	case 2: {
		if (values[0].IsNum() and values[1].IsNum()) {
			return glm::vec2(values[0].GetNum(), values[1].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector2");
	}
	}
	PushError("Invalid vector construction size" + node->Print());
}

ee_decl(Vector3LiteralNode* node) {
	std::vector<Value> values;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		if (val.IsInt() or val.IsFloat() or val.IsVec()) {
			values.push_back(val);
		}
		else {
			PushError("Cannot assign " + val.Type() + " to a Vector");
		}
	}

	switch (values.size()) {
	case 1: {
		if (values[0].data.index() == 13) {
			return glm::vec3(values[0].GetVec3());
		}
		else if (values[0].IsNum()) {
			return glm::vec3(values[0].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector3");
		break;
	}
	case 3: {
		if (values[0].IsNum() and values[1].IsNum() and values[2].IsNum()) {
			return glm::vec3(values[0].GetNum(), values[1].GetNum(), values[2].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector3");
	}
	}
	PushError("Invalid vector construction size" + node->Print());
}

ee_decl(Vector4LiteralNode* node) {
	std::vector<Value> values;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		if (val.IsInt() or val.IsFloat() or val.IsVec()) {
			values.push_back(val);
		}
		else {
			PushError("Cannot assign " + val.Type() + " to a Vector");
		}
	}

	switch (values.size()) {
	case 1: {
		if (values[0].data.index() == 14) {
			return glm::vec4(values[0].GetVec4());
		}
		else if (values[0].IsNum()) {
			return glm::vec4(values[0].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector4");
		break;
	}
	case 3: {
		if (values[0].IsNum() and values[1].IsNum() and values[2].IsNum() and values[3].IsNum()) {
			return glm::vec4(values[0].GetNum(), values[1].GetNum(), values[2].GetNum(), values[3].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector4");
	}
	}
	PushError("Invalid vector construction size" + node->Print());
}

ee_decl(ArrayAccessNode* node) {
	Value arr = EvaluateExpression(node->arr);
	Value index = EvaluateExpression(node->index);

	if (arr.IsArray()) {
		std::vector<Value>& list = arr.GetArray();
		if (index.IsInt()) {
			return list[index.GetInt()];
		}
		else {
			PushError("Index is not an int" + node->Print());
		}
	}
	else {
		PushError("Cannot use array access on a non array" + node->Print());
	}

}


// If we are trying to modify a place in memory then we want a pointer to a value instead of just a value
// 
// EvaluateExpressionPtr
// 



eep_decl(ExprNode* node) {
	eep(VariableNode*)
		eep(TernaryNode*)
		eep(DotAccessNode*)
		/*
	eep(IntLiteralNode*)
	eep(FloatLiteralNode*)
	eep(BoolLiteralNode*)
	eep(StringLiteralNode*)
	eep(ArrayLiteralNode*)
	eep(FuncCallNode*) */
	// if the pointer cannot be obtained we must manually create and destroy the object
	Value* val = new Value(EvaluateExpression(node));
	val->is_manually_created = true;
	return val;
}

eep_decl(VariableNode* node) {
	return scope->Get(node->identifier);
}

eep_decl(TernaryNode* node) {
	Value val = EvaluateExpression(node->expression);

	if (val.IsBool()) {
		if (val.GetBool()) {
			return EvaluateExpressionPtr(node->truthy_value);
		}
		else {
			return EvaluateExpressionPtr(node->falsey_value);
		}
	}
	else {
		PushError("Ternary expression does not evaluate to boolean type " + node->Print());
	}
}

eep_decl(DotAccessNode* node) {
	Value* obj = EvaluateExpressionPtr(node->left);
	Value* left = nullptr;

	Scope* n_scope = nullptr;
	if (obj->IsCPP()) {
		n_scope = &std::get<CPPObject>(obj->data).scope;
		n_scope->Set("SELF", std::get<CPPObject>(obj->data));
	}
	else {
		n_scope = &std::get<Scope>(obj->data);
	}

	if (FuncCallNode* func = dynamic_cast<FuncCallNode*>(node->right)) {
		left = n_scope->Get(func->func_id);
		if (left->IsFunction()) {
			std::vector<Value*> args;
			for (ExprNode* arg : func->args) {
				args.push_back(EvaluateExpressionPtr(arg));
			}

			if (n_scope->Has("SELF")) {
				args.push_back(n_scope->Get("SELF"));
			}

			if (left->IsCPP()) {
				CPPFunction& func = std::get<CPPFunction>(left->data);
				Value* r_value = new Value(func(args));
				for (Value* arg : args) {
					arg->Release();
				}
				r_value->is_manually_created = true;
				return r_value;
			}
			else {
				NovaFunction& func = std::get<NovaFunction>(left->data);
				Scope* p_scope = scope;
				scope = n_scope;

				for (int i = 0; i < func->args.size(); i++) {
					scope->Set(func->args[i], *args[i]);
				}

				for (StmtNode* stmt : func->body) {
					EvaluateStatement(stmt);
					if (return_flag) {
						return_flag = false;
						scope = p_scope;
						for (Value* arg : args) {
							arg->Release();
						}
						return &return_val;
					}
				}
				scope = p_scope;
				for (Value* arg : args) {
					arg->Release();
				}
				Value* null_val = new Value();
				null_val->is_manually_created = true;
				return null_val;

			}
		}
	}
	else if (VariableNode* var = dynamic_cast<VariableNode*>(node->right)) {
		obj->Release();
		return left = n_scope->Get(var->identifier);
	}
	else {
		Scope* p_scope = scope;
		scope = n_scope;
		Value* val = EvaluateExpressionPtr(node->right);
		obj->Release();
		scope = p_scope;
		if (val) {
			return val;
		}
		else {
			PushError("Invalid dot access" + node->Print());
			return val;
		}
	}
}

eep_decl(ArrayAccessNode* node) {
	Value* arr = EvaluateExpressionPtr(node->arr);
	Value index = EvaluateExpression(node->index);

	if (arr->IsArray()) {
		std::vector<Value>& list = arr->GetArray();
		if (index.IsInt()) {
			return &list[index.GetInt()];
		}
		else {
			PushError("Index is not an int" + node->Print());
		}
	}
	else {
		PushError("Cannot use array access on a non array" + node->Print());
	}

}