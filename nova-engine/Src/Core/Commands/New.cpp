#include "New.h"
#include "../Engine.h"
#include "../../Scene/SceneParser.h"
#include <algorithm>
#include <glm/glm.hpp>
#include "../../Assets/Asset.h"

void NewCommand::Execute(const std::vector<std::string>& args) {
	// new FileImage player_image filepath "path/to/image"
	if (args.size() < 3) {
		Engine::GetInstance()->PushError("Usage: " + Usage());
		return;
	}
	std::vector<int> cmd_objs_used;
	LoadableValues values;
	if (args.size() > 3) {
		for (size_t i = 3; i < args.size(); i++) {
			std::string prop = args[i];
			i++;
			if (i == args.size()) {
				Engine::GetInstance()->PushError("Too few arguments to construct a loadable value");
				Engine::GetInstance()->PushError("Usage: " + Usage());
				return;
			}
			if (args[i] == "true") {
				values[prop] = new bool(true);
			}
			else if (args[i] == "false") {
				values[prop] = new bool(false);
			}
			else if (std::all_of(args[i].begin(), args[i].end(), [](char c) { return std::isdigit(c) || c == '.'; })) {
				if (args[i].find('.') != std::string::npos) {
					values[prop] = new float(std::stof(args[i]));
				}
				else {
					values[prop] = new int(std::stoi(args[i]));
				}
			}
			else if (args[i].find("Vector2(") == 0) {
				std::string vec = args[i].substr(8, args[i].size() - 9);
				size_t comma = vec.find(',');
				float x = std::stof(vec.substr(0, comma));
				float y = std::stof(vec.substr(comma + 1));
				values[prop] = new glm::vec2(x, y);
			}
			else if (args[i].find("Vector3(") == 0) {
				std::string vec = args[i].substr(8, args[i].size() - 9);
				size_t first_comma = vec.find(',');
				size_t second_comma = vec.find(',', first_comma + 1);
				float x = std::stof(vec.substr(0, first_comma));
				float y = std::stof(vec.substr(first_comma + 1, second_comma - first_comma - 1));
				float z = std::stof(vec.substr(second_comma + 1));
				values[prop] = new glm::vec3(x, y, z);
			}
			else if (args[i].find("Vector4(") == 0) {
				std::string vec = args[i].substr(8, args[i].size() - 9);
				size_t first_comma = vec.find(',');
				size_t second_comma = vec.find(',', first_comma + 1);
				size_t third_comma = vec.find(',', second_comma + 1);
				float x = std::stof(vec.substr(0, first_comma));
				float y = std::stof(vec.substr(first_comma + 1, second_comma - first_comma - 1));
				float z = std::stof(vec.substr(second_comma + 1, third_comma - second_comma - 1));
				float w = std::stof(vec.substr(third_comma + 1));
				values[prop] = new glm::vec4(x, y, z, w);
			}
			else if (args[i].starts_with('"')) {
				if (args[i].ends_with('"') && args[i].size() >= 2) {
					values[prop] = new std::string(args[i].substr(1, args[i].size() - 2));
				}
				else {
					Engine::GetInstance()->PushError("Invalid string format for property: " + prop);
					return;
				}
			}
			else if (Engine::command_objects.find(args[i]) != Engine::command_objects.end()) {
				values[prop] = Engine::command_objects[args[i]]->ptr;
				cmd_objs_used.push_back(i);
			}
			else {
				values[prop] = new std::string(args[i]);
			}

		}
	}

	SceneParser parser;
	SceneEntryInst* temp = new SceneEntryInst(parser.BuildInstance(args[1], values));
	Engine::command_objects[args[2]] = temp;

	for (int i : cmd_objs_used) {
		SceneEntryInst* obj = Engine::command_objects[args[i]];
		if (obj->kind == SceneEntryInst::Kind::Object) {
			Engine::command_objects.erase(args[i]);
		}
		else {
			Asset* asset = static_cast<Asset*>(obj->ptr);
			asset->Release();
			Engine::command_objects.erase(args[i]);
		}
	}

}
