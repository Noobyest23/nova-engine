#ifndef NOVA_CLEAR_COMMAND_H
#define NOVA_CLEAR_COMMAND_H

#include "Command.h"

struct ClearCommand : public Command {
	std::string Name() override { return "clear"; }
	std::string Usage() override { return "clear"; }
	std::string Desc() override { return "Clears the console"; }
	void Execute(const std::vector<std::string>& args) override;
};

#endif