#include "Lexer.h"

#include "../../Core/Engine.h"
#include <iostream>
#include <fstream>

Lexer::Lexer(const char* filepath) {
	
	std::ifstream file(filepath);
	std::string line;
	
	if (not file.is_open()) {
		Engine* engine = Engine::GetInstance();

		engine->PushError("Failed to open: " + std::string(filepath));
	}

	while (std::getline(file, line)) {
		text += line + "\n";
	}
}

std::vector<Token> Lexer::Parse() {
	std::vector<Token> result;

	while (pos < text.length()) {
		char c = Current();

		//if (c == '\n') {
		//	result.push_back({ TokenType::NewLine, "\n", line, column});
		//}

		// Skip whitespace
		if (isspace(c)) {
			Advance();
			continue;
		}

		// Number literal
		if (isdigit(c)) {
			result.push_back(Number());
			continue;
		}

		// Identifier (variables, keywords later)
		if (isalpha(c) || c == '_') {
			result.push_back(Identifier());
			continue;
		}

		switch (c) {
		case '+':
			Advance();
			if (Current() == '=') {
				result.push_back({ TokenType::CPlusOp, "+=", line, column});
				Advance();
			}
			else {
				result.push_back({ TokenType::PlusOp, "+", line, column });
			}
			continue;
		case '-':
			Advance();
			if (Current() == '=') {
				result.push_back({ TokenType::CMinusOp, "-=", line, column });
				Advance();
			}
			else {
				result.push_back({ TokenType::MinusOp, "-", line, column });
			}
			continue;
		case '*':
			Advance();
			if (Current() == '=') {
				result.push_back({ TokenType::CMultOp, "*=", line, column });
				Advance();
			}
			else {
				result.push_back({ TokenType::MultOp, "*", line, column });
			}
			continue;
		case '/':
			Advance();
			if (Current() == '=') {
				result.push_back({ TokenType::CDivOp, "/=", line, column });
				Advance();
			}
			else {
				result.push_back({ TokenType::DivOp, "/", line, column });
			}
			continue;
		case '=':
			Advance();
			if (Current() == '=') {
				result.push_back({ TokenType::EqualOp, "==", line, column });
				Advance();
			}
			else {
				result.push_back({ TokenType::Assignment, "=", line, column });
			}
			continue;
		case '&':
			Advance();
			if (Current() == '&') {
				result.push_back({ TokenType::AndOp, "&&", line, column });
			}
			continue;
		case '|':
			Advance();
			if (Current() == '|') {
				result.push_back({ TokenType::OrOp, "||", line, column });
			}
			else {
				result.push_back({ TokenType::BreakPoint, "|", line, column });
			}
			continue;
		case '(':
			Advance();
			result.push_back({ TokenType::OpenParen, "(", line, column });
			continue;
		case ')':
			Advance();
			result.push_back({ TokenType::CloseParen, ")", line, column });
			continue;
		case '{':
			Advance();
			result.push_back({ TokenType::OpenBracket, "{", line, column });
			continue;
		case '}':
			Advance();
			result.push_back({ TokenType::CloseBracket, "}", line, column });
			continue;
		case '>':
			Advance();
			if (Current() == '=') {
				Advance();
				result.push_back({ TokenType::GtetOp, ">=", line, column });
			}
			else {
				result.push_back({ TokenType::GreaterOp, ">", line, column });
			}
			continue;
		case '<':
			Advance();
			if (Current() == '=') {
				Advance();
				result.push_back({ TokenType::LtetOp, "<=", line, column });
			}
			else {
				result.push_back({ TokenType::LesserOp, "<", line, column });
			}
			continue;
		case ':':
			Advance();
			result.push_back({ TokenType::Mirror, ":", line, column });
			continue;
		case '#':
			while (Current() != '\n') {
				Advance();
			}
			Advance();
			continue;
		case '"':
			Advance();
			{
				std::string content;
				while (Current() != '"') {
					content += Current();
					Advance();
				}
				Advance();
				result.push_back({ TokenType::StringLit, content, line, column });
			}
			continue;
		case '[':
			Advance();
			result.push_back({ TokenType::OpenBracket, "[", line, column});
			continue;
		case ']':
			Advance();
			result.push_back({ TokenType::CloseBracket, "]", line, column});
			continue;
		case '.':
			Advance();
			result.push_back({ TokenType::Dot, ".", line, column });
			continue;
		}

		Advance();
	}

	result.push_back({ TokenType::Eof, "bye bye", line, column });
	return result;
}

void Lexer::Advance() {
	pos++;
	column++;
	if (Current() == '\n') {
		line++;
		column = 0;
	}
}

Token Lexer::Number() {
	std::string value;
	bool hasDot = false;

	while (true) {
		char c = Current();

		if (isdigit(c)) {
			value += c;
		}
		else if (c == '.' && !hasDot) {
			hasDot = true;
			value += c;
		}
		else {
			break;
		}

		Advance();
	}

	return Token{ hasDot ? TokenType::FloatLit : TokenType::IntLit, value, line, column };
}

Token Lexer::Identifier() {
	std::string value;

	while (isalnum(Current()) || Current() == '_') {
		value += Current();
		Advance();
	}
	if (value == "var") {
		return { TokenType::VarDecl, value, line, column };
	}
	else if (value == "func") {
		return { TokenType::FuncDecl, value, line, column };
	}
	else if (value == "if") {
		return { TokenType::IfStmt, value, line, column };
	}
	else if (value == "else") {
		return { TokenType::ElseStmt, value, line, column };
	}
	else if (value == "elif") {
		return { TokenType::ElifStmt, value, line, column };
	}
	else if (value == "and") {
		return { TokenType::AndOp, "&&", line, column };
	}
	else if (value == "or") {
		return { TokenType::OrOp, "||", line, column };
	}
	else if (value == "type") {
		return { TokenType::TypeDecl, value, line, column };
	}
	else if (value == "include") {
		return { TokenType::Include, value, line, column };
	}
	else if (value == "return") {
		return { TokenType::Return, value, line, column };
	}
	else if (value == "break") {
		return { TokenType::Break, value, line, column };
	}
	else if (value == "continue") {
		return { TokenType::Continue, value, line, column };
	}
	else if (value == "Vector2") {
		return { TokenType::VectorLit2, value, line, column };
	}
	else if (value == "Vector3") {
		return { TokenType::VectorLit3, value, line, column };
	}
	else if (value == "Vector4") {
		return { TokenType::VectorLit4, value, line, column };
	}
	else if (value == "true") {
		return { TokenType::BoolLit, value, line, column };
	}
	else if (value == "false") {
		return { TokenType::BoolLit, value, line, column };
	}
	return Token{ TokenType::Identifier, value, line, column };
}

char Lexer::Current() const {
	return pos < text.length() ? text[pos] : '\0';
}

char Lexer::Peek(int offset = 1) const {
	size_t index = pos + offset;
	return index < text.length() ? text[index] : '\0';
}