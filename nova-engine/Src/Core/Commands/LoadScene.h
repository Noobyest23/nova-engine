#ifndef NOVA_LOAD_SCENE_COMMAND_H
#define NOVA_LOAD_SCENE_COMMAND_H

#include "Command.h"

struct LoadSceneCommand : public Command {
public:
	virtual std::string Name() { return "load_scene"; };
	virtual std::string Usage() { return "load_scene [path/to/scene/file]"; };
	virtual std::string Desc() { return "Loads a scene"; };
	virtual void Execute(const std::vector<std::string>& args);
};

#endif