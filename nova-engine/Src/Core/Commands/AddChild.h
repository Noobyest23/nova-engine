#ifndef NOVA_ADD_CHILD_COMMAND_H
#define NOVA_ADD_CHILD_COMMAND_H

#include "Command.h"

struct AddChildCommand : public Command {
	std::string Name() override { return "add_child"; }
	std::string Usage() override { return "add_child [path/to/parent]"; }
	std::string Desc() override { return "Adds the last object created with the new command to the parent object"; }
	void Execute(const std::vector<std::string>& args) override;
};

#endif
