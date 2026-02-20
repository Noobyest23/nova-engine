#include "Help.h"
#include "../Engine.h"

void HelpCommand::Execute(const std::vector<std::string>& args) {
	auto commands = Command::GetAll();
	if (args.size() == 1) {
		Engine::GetInstance()->PushMessage("Available Commands:");
		for (const auto& [name, command] : commands) {
			Engine::GetInstance()->PushMessage(" - " + name + ": " + command->Desc());
		}
	}
	else {
		std::string command_name = args[1];
		if (commands.find(command_name) != commands.end()) {
			Command* command = commands[command_name];
			Engine::GetInstance()->PushMessage("Usage: " + command->Usage());
			Engine::GetInstance()->PushMessage(command->Desc());
		}
		else {
			Engine::GetInstance()->PushError("Command not found: " + command_name);
		}
	}
}