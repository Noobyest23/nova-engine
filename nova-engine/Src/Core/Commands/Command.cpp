#include "Command.h"
#include "Help.h"
#include "New.h"
std::unordered_map<std::string, Command*> Command::GetAll() {
	std::unordered_map<std::string, Command*> commands;
	commands["help"] = new HelpCommand;
	commands["new"] = new NewCommand;
	return commands;
}
