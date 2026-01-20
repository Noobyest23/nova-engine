#ifndef NOVASCRIPT_PARSER_H
#define NOVASCRIPT_PARSER_H

#include "Tokens.h"
#include <vector>
#include "../ASTNodes/ProgramNode.h"

class Parser {
public:

	Parser(std::vector<Token>& tokens) : tokens(tokens) {};

	ProgramNode* Parse();


private:

	StmtNode* ParseStatement();
	ExprNode* ParseExpression();

	ExprNode* ParseTernary();
	ExprNode* ParseAccesor();
	ExprNode* ParseAssignment();
	ExprNode* ParseBoolean();
	ExprNode* ParseEquality();
	ExprNode* ParseComparison();
	ExprNode* ParseArithmetic();
	ExprNode* ParseTerm();
	ExprNode* ParsePrimary();
	
	void Advance() { t_index++; };
	Token& Current() const { return tokens[t_index]; };
	bool Accept(TokenType type) const { return (tokens[t_index].type == type); };
	bool AcceptNext(TokenType type) const { return (Next().type == type); };
	Token Last() const { return tokens[t_index - 1]; };
	Token Next() const { return tokens[t_index + 1]; };
	unsigned int t_index = 0;
	std::vector<Token>& tokens;

	bool force_stop = false;
	bool soft_stop = false;

	void PushError(const std::string& message);

};



#endif
