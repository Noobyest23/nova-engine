#ifndef NOVA_PRINT_TREE_COMMAND_H
#define NOVA_PRINT_TREE_COMMAND_H

#include "Command.h"

struct PrintTreeCommand : public Command {
	std::string Name() override { return "print_tree"; }
	std::string Usage() override { return "print_tree [path/to/object]"; }
	std::string Desc() override { return "Prints the tree of the given object, or the entire scene if no object is given"; }
	void Execute(const std::vector<std::string>& args) override;
};

#endif