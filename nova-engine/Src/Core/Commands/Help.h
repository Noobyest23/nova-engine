#ifndef NOVA_COMMAND_HELP_H
#define NOVA_COMMAND_HELP_H

#include "Command.h"

struct HelpCommand : public Command {
	std::string Name() override { return "help"; }
	std::string Usage() override { return "help [command]"; }
	std::string Desc() override { return "Provides information about available commands. If a command is specified, provides detailed information about that command."; }

	void Execute(const std::vector<std::string>& args) override;

};

#endif