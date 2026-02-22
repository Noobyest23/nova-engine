#ifndef NOVA_DELETE_COMMAND_H
#define NOVA_DELETE_COMMAND_H

#include "Command.h"

struct DeleteCommand : public Command {
	std::string Name() { return "delete"; };
	std::string Usage() { return "delete [path/to/object]"; };
	std::string Desc() { return "Removes an object from the scene tree"; };
	void Execute(const std::vector<std::string>& args) override;
};


#endif
