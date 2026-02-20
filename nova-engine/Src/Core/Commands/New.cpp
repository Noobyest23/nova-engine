#include "New.h"
#include "../Engine.h"
#include "../../Scene/SceneParser.h"
#include <algorithm>
#include <glm/glm.hpp>
#include "../../Assets/Asset.h"

void NewCommand::Execute(const std::vector<std::string>& args) {
	// new FileImage filepath "path/to/image"
	if (args.size() < 2) {
		Engine::GetInstance()->PushError("Usage: " + Usage());
		return;
	}
	LoadableValues values;
	if (args.size() > 2) {
		for (size_t i = 2; i < args.size(); i++) {
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
			else if (args[i] == "ENGINE_LAST") {
				values[prop] = Engine::cmd_current_obj->ptr;
				Engine::used_cmd_obj = true;
			}
			else {
				values[prop] = new std::string(args[i]);
			}

		}
	}

	SceneParser parser;
	SceneEntryInst* temp = new SceneEntryInst(parser.BuildInstance(args[1], values));
	
	if (Engine::cmd_current_obj != nullptr) {
		if (Engine::cmd_current_obj->kind == SceneEntryInst::Kind::Object) {
			if (!Engine::used_cmd_obj) {
				delete static_cast<Object*>(Engine::cmd_current_obj->ptr);
				Engine::GetInstance()->PushMessage("[Engine] Previous Object instance was unused and has been deleted", true);
			}
		}
		else {
			Asset* asset = static_cast<Asset*>(Engine::cmd_current_obj->ptr);
			asset->Release();
		}
		delete Engine::cmd_current_obj;
		Engine::cmd_current_obj = nullptr;
	}
	Engine::cmd_current_obj = temp;

}
