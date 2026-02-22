#include "Command.h"
#include "Help.h"
#include "New.h"
#include "AddChild.h"
#include "PrintTree.h"
#include "Clear.h"
#include "Delete.h"

std::unordered_map<std::string, Command*> Command::GetAll() {
	std::unordered_map<std::string, Command*> commands;
	commands["help"] = new HelpCommand;
	commands["new"] = new NewCommand;
	commands["add_child"] = new AddChildCommand;
	commands["print_tree"] = new PrintTreeCommand;
	commands["clear"] = new ClearCommand;
	commands["delete"] = new DeleteCommand;
	return commands;
}
