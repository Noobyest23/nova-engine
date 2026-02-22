#ifndef NOVA_COMMAND_NEW_H
#define NOVA_COMMAND_NEW_H

#include "Command.h"

struct NewCommand : public Command {
	std::string Name() override { return "new"; }
	std::string Usage() override { return "new [asset/object] [alias] [prop_name1] [prop_value1] [prop_name2] [prop_value2]..."; }
	std::string Desc() override { return "Creates a new Asset or Object, Syntax is the same as scene files"; }
	void Execute(const std::vector<std::string>& args) override;
};

#endif