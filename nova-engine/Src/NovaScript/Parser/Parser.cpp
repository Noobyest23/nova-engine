#include "Parser.h"
#include "../../Core/Engine.h"

// TODO:
// Eric stupid he realise you can use switch on TokenType


ProgramNode* Parser::Parse() {
	std::vector<StmtNode*> statements;
	while (t_index < tokens.size() - 1 and not force_stop and Current().type != TokenType::Eof) {
		statements.push_back(ParseStatement());
	}
	ProgramNode* program = new ProgramNode(statements);
	program->valid_program = not force_stop;
	return program;
}

StmtNode* Parser::ParseStatement() {
	// Variable Declaration Node
	if (Accept(TokenType::VarDecl)) {
		// Next token is an identifier
		Advance();
		if (Accept(TokenType::Identifier)) {
			std::string var_name = Current().content;
			Advance();

			// Optionally check for initialization
			ExprNode* expression = nullptr;
			if (Accept(TokenType::Assignment)) {
				Advance();
				expression = ParseExpression();
			}
			
			return new VarDeclNode(var_name, expression);
		}
		else {
			PushError("Expected variable name");
		}
	}
	// Function Declaration Node
	if (Accept(TokenType::FuncDecl)) {
		Advance();
		if (Accept(TokenType::Identifier)) {
			std::string func_name = Current().content;
			std::vector<std::string> args;
			std::vector<StmtNode*> body;
			Advance();
			
			if (Accept(TokenType::OpenParen)) {
				Advance();
				while (!Accept(TokenType::CloseParen)) {
					if (Accept(TokenType::Identifier)) {
						args.push_back(Current().content);
						Advance();
					}
					else {
						PushError("Expected identifier");
						break;
					}

				}
				Advance();
				if (Accept(TokenType::OpenBracket)) {
					Advance();
					while (!Accept(TokenType::CloseBracket)) {
						body.push_back(ParseStatement());
					}
				}
				else {
					PushError("Expected {");
				}
				Advance();

				return new FuncDeclNode(func_name, args, body);
			}
			else {
				PushError("Expected (");
			}

		}
		else {
			PushError("Expected function name");
		}
	}
	// If Statement
	if (Accept(TokenType::IfStmt)) {
		Advance();
		// Parenthesis optional on if statements
		if (Accept(TokenType::OpenParen)) {
			Advance();
		}
		ExprNode* expression = ParseExpression();
		if (Accept(TokenType::CloseParen)) {
			Advance();
		}

		if (Accept(TokenType::OpenBracket)) {
			std::vector<StmtNode*> body;
			std::vector<StmtNode*> else_body;
			Advance();
			while (!Accept(TokenType::CloseBracket)) {
				StmtNode* statement = ParseStatement();
				body.push_back(statement);
			}
			Advance();
			if (Accept(TokenType::ElseStmt)) {
				Advance();
				if (Accept(TokenType::OpenBracket)) {
					Advance();
					while (!Accept(TokenType::CloseBracket)) {
						StmtNode* statement = ParseStatement();
						else_body.push_back(statement);
					}
					Advance();
				}
				else {
					PushError("Expected {");
				}
				
			}
			return new IfStmtNode(expression, body, else_body);
		}
		else {
			PushError("Expected {");
		}


	}
	// Type Declaration Node
	if (Accept(TokenType::TypeDecl)) {
		Advance();
		if (Accept(TokenType::Identifier)) {
			std::string type_id = Current().content;
			std::string mirror_id = "";
			Advance();
			// Optional inheritence
			if (Accept(TokenType::Mirror)) {
				Advance();
				if (Accept(TokenType::Identifier)) {
					mirror_id = Current().content;
					Advance();
				}
			}
			std::vector<StmtNode*> scope;
			if (Accept(TokenType::OpenBracket)) {
				Advance();
				while (!Accept(TokenType::CloseBracket)) {
					StmtNode* stmt = ParseStatement();
					scope.push_back(stmt);
				}
				Advance();

				return new TypeDeclNode(type_id, mirror_id, scope);
			}
			else {
				PushError("Expected {");
			}

		}
		else {
			PushError("Expected an identifier");
		}
	}
	// Include Statement
	if (Accept(TokenType::Include)) {
		Advance();
		ExprNode* expr = ParseExpression();
		if (expr) {
			if (Accept(TokenType::Identifier) and Current().content == "as") {
				Advance();
				ExprNode* as = ParseExpression();
				return new IncludeNode(expr, as);
			}
			return new IncludeNode(expr);
		}
		else {
			PushError("Expected an expression");
		}
	}
	if (Accept(TokenType::Return)) {
		Advance();
		ExprNode* return_value = ParseExpression();
		if (return_value) {
			return new ReturnStmtNode(return_value);
		}
		else {
			PushError("Expected an expression");
		}
	}
	if (Accept(TokenType::BreakPoint)) {
		Advance();
		StmtNode* stmt = ParseStatement();
		return new BreakPointNode(stmt);
	}
	// If nothing else works we try to parse it as a standalone expression
	ExprNode* expr = ParseExpression();
	if (expr) {
		return new ExprAsStmt(expr);
	}
	Advance();
	PushError("Unexpected Token Type in statement");
	return nullptr;
}

ExprNode* Parser::ParseExpression() {
	ExprNode* expression = ParseAccesor();
	if (expression) {
		return expression;
	}
	else {
		Advance();
		PushError("Unexpected Token Type in expression");
		return nullptr;
	}
}

ExprNode* Parser::ParseTernary() {
	ExprNode* truthy = ParseAccesor();
	if (Accept(TokenType::IfStmt)) {
		Advance();

		ExprNode* condition = ParseExpression();

		if (!Accept(TokenType::ElseStmt)) {
			PushError("Expected 'else' in ternary expression");
			return truthy;
		}
		Advance();

		ExprNode* falsy = ParseExpression();

		return new TernaryNode(condition, truthy, falsy);
	}
	return truthy;
}

ExprNode* Parser::ParseAccesor() {
	ExprNode* expression = ParseAssignment();
	if (Accept(TokenType::Dot)) {
		Advance();
		ExprNode* right = ParseExpression();
		return new DotAccessNode(expression, right);
	}
	if (Accept(TokenType::OpenBracket)) {
		Advance();
		ExprNode* index = ParseExpression();
		if (!Accept(TokenType::CloseBracket)) {
			PushError("Expected ]");
		}
		Advance();
		return new ArrayAccessNode(expression, index);
	}
	return expression;
}

ExprNode* Parser::ParseAssignment() {
	ExprNode* expression = ParseBoolean();
	if (Accept(TokenType::Assignment)) {
		Advance();
		ExprNode* right = ParseExpression();
		return new AssignmentNode(expression, right);
	}
	return expression;
}

ExprNode* Parser::ParseBoolean() {
	ExprNode* lhs = ParseEquality();

	while (Accept(TokenType::AndOp) || Accept(TokenType::OrOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseEquality();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParseEquality() {
	ExprNode* lhs = ParseComparison();

	while (Accept(TokenType::EqualOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseComparison();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParseComparison() {
	ExprNode* lhs = ParseArithmetic();

	while (Accept(TokenType::GreaterOp) || Accept(TokenType::LesserOp) ||
		Accept(TokenType::GtetOp) || Accept(TokenType::LtetOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseArithmetic();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParseArithmetic() {
	ExprNode* lhs = ParseTerm();

	while (Accept(TokenType::PlusOp) || Accept(TokenType::MinusOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseTerm();
		lhs = new OpNode(lhs, rhs, op);
	}
	while (Accept(TokenType::CPlusOp) or Accept(TokenType::CMinusOp) or Accept(TokenType::CMultOp) or Accept(TokenType::CDivOp)) {
		std::string op = Current().content;
		Advance();

		ExprNode* rhs = ParseExpression();
		if (rhs) {
			return new CompoundOp(op, lhs, rhs);
		}
		else {
			PushError("Expected an expression");
		}
	}
	return lhs;
}

ExprNode* Parser::ParseTerm() {
	ExprNode* lhs = ParsePrimary();

	while (Accept(TokenType::MultOp) || Accept(TokenType::DivOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParsePrimary();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParsePrimary() {
	if (Accept(TokenType::OpenParen)) {
		Advance();
		ExprNode* expr = ParseTernary();
		if (Accept(TokenType::CloseParen)) {
			Advance();
		}
		else {
			PushError("Expected )");
		}
		
		return expr;
	}
	if (Accept(TokenType::IntLit)) {
		int number = std::stoi(Current().content);
		Advance();
		return new IntLiteralNode(number);
	}
	else if (Accept(TokenType::FloatLit)) {
		float number = std::stof(Current().content);
		Advance();
		return new FloatLiteralNode(number);
	}
	else if (Accept(TokenType::StringLit)) {
		std::string content = Current().content;
		Advance();
		return new StringLiteralNode(content);
	}
	else if (Accept(TokenType::BoolLit)) {
		bool value = Current().content == "true" ? true : false;
		Advance();
		return new BoolLiteralNode(value);
	}
	else if (Accept(TokenType::Identifier)) {
		std::string id = Current().content;
		Advance();
		// Function Call
		if (Accept(TokenType::OpenParen)) {
			std::vector<ExprNode*> args;
			Advance();
			while (!Accept(TokenType::CloseParen)) {
				ExprNode* arg = ParseExpression();
				args.push_back(arg);
			}
			Advance();
			return new FuncCallNode(id, args);
		}
		return new VariableNode(id);
	}
	else if (Accept(TokenType::OpenBracket)) {
		std::vector<ExprNode*> values;
		Advance();
		while(!Accept(TokenType::CloseBracket)) {
			values.push_back(ParseExpression());
		}
		Advance();
		return new ArrayLiteralNode(values);
	}
	else if (Accept(TokenType::VectorLit2)) {
		std::vector<ExprNode*> values;
		Advance();
		if (Accept(TokenType::OpenParen)) {
			Advance();
			while (!Accept(TokenType::CloseParen)) {
				values.push_back(ParseExpression());
				if (values.size() > 4) {
					PushError("Vector size cannot go past 4");
					break;
				}
			}
			Advance();
			return new Vector2LiteralNode(values);
		}
	}
	else if (Accept(TokenType::VectorLit3)) {
		std::vector<ExprNode*> values;
		Advance();
		if (Accept(TokenType::OpenParen)) {
			Advance();
			while (!Accept(TokenType::CloseParen)) {
				values.push_back(ParseExpression());
				if (values.size() > 4) {
					PushError("Vector size cannot go past 4");
					break;
				}
			}
			Advance();
			return new Vector3LiteralNode(values);
		}
	}
	else if (Accept(TokenType::VectorLit4)) {
		std::vector<ExprNode*> values;
		Advance();
		if (Accept(TokenType::OpenParen)) {
			Advance();
			while (!Accept(TokenType::CloseParen)) {
				values.push_back(ParseExpression());
				if (values.size() > 4) {
					PushError("Vector size cannot go past 4");
					break;
				}
			}
			Advance();
			return new Vector4LiteralNode(values);
		}
	}
	return nullptr;
}

void Parser::PushError(const std::string& message) {
	force_stop = true;
	Engine* engine = Engine::GetInstance();
	engine->PushError("[Parser] " + message + " at " + std::to_string(Current().line) + ", " + std::to_string(Current().column) + ", TokenType is " + std::to_string(int(Current().type)));
}
