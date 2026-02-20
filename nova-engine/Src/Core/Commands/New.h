#ifndef NOVA_COMMAND_NEW_H
#define NOVA_COMMAND_NEW_H

#include "Command.h"

struct NewCommand : public Command {
	std::string Name() override { return "new"; }
	std::string Usage() override { return "new [asset/object] [args..]"; }
	std::string Desc() override { return "Creates a new Asset or Object"; }
	void Execute(const std::vector<std::string>& args) override;
};

#endif