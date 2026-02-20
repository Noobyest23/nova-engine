#ifndef NOVA_COMMAND_H
#define NOVA_COMMAND_H

#include <string>
#include <unordered_map>

struct Command {
	virtual std::string Name() = 0;
	virtual std::string Usage() = 0;
	virtual std::string Desc() = 0;
	virtual void Execute(const std::vector<std::string>& args) = 0;

	static std::unordered_map<std::string, Command*> GetAll();
};


#endif