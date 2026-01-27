#ifndef NOVASCRIPT_AST_H
#define NOVASCRIPT_AST_H

#include <string>

struct ASTNode {
	
	virtual std::string Print() const {
		return "Not Implemented";
	};

	virtual void Delete() {
		delete this;
	};

};


#endif