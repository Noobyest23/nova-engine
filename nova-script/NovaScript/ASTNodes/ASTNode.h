#ifndef NOVASCRIPT_AST_H
#define NOVASCRIPT_AST_H

#include <string>

struct StmtNode;
struct ExprNode;

struct ASTNode {
	bool constant = false;
	virtual std::string Print() const {
		return "Not Implemented";
	};

	virtual void Delete() {
		delete this;
	};

	virtual std::vector<StmtNode*> GetStatements() const { return {}; };
	virtual std::vector<ExprNode*> GetExpressions() const { return {}; };
	// TODO - INCORPORATE INTO EVERY NODE AND OPTIMIZER
};


#endif