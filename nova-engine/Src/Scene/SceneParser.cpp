#include "SceneParser.h"
#include "Lexer.h"
#include "../Core/Engine.h"
#include "../Objects/Object.h"
#include "../Assets/Asset.h"
#include <glm/glm.hpp>
Object* SceneParser::Parse(const std::string& filepath) {
	tokens = Lexer((Engine::GetInstance()->GetProjectPath() + filepath).c_str()).Parse();

	
	Loop();
	Object* root_obj = new Object();
	if (success) {
		for (std::pair<std::string, Asset*> asset : assets) {
			asset.second->Release();
		}
		for (std::pair<std::string, Object*> obj : objects) {
			if (!obj.second->GetParent()) {
				root_obj->AddChild(obj.second);
			}
			else {
				// make sure there arent any cyclical parent chains
				std::vector<Object*> seen_objs;
				Object* parent = obj.second->GetParent();
				bool del = false;
				while (parent) {
					if (std::find(seen_objs.begin(), seen_objs.end(), parent) != seen_objs.end()) {
						PushError("Cyclical Parent chain, deleting all involved objects");
						del = true;
					}

					seen_objs.push_back(parent);
					parent = parent->GetParent();

				}

				if (del) {
					for (Object* insulent_garbage : seen_objs) {
						delete insulent_garbage;
					}
				}
			}
		}
	}
	return root_obj;
}

void SceneParser::Loop() {

	while (!Accept(NovaTokenType::Eof)) {
		if (Accept(NovaTokenType::Identifier)) {
			if (Current().content == "_ON_ROOT_ADD") {
				Advance();
				if (Accept(NovaTokenType::OpenBracket)) {
					Advance();
					while (!Accept(NovaTokenType::CloseBracket)) {
						on_root_add.push_back(Current().content);
						Advance();
					}
					Advance();
					continue;
				}
				PushError("Expected {");
				return;
			}
			PushError("Unexpected Identifier");
			return;
		}

		// [Sprite2D]
		const std::string& type = GetType();
		
		// image = 0
		// flip_h = true
		LoadableValues values;
		while (!Accept(NovaTokenType::OpenBracket)) {
			LValuePair pair = GetLoadableValue();
			if (pair.second) {
				values[pair.first] = pair.second;
			}
			else {
				PushError("Loaded value was null! " + pair.first);
			}
		}
		if (values.find("tag") == values.end()) {
			values["tag"] = new std::string(std::to_string(_auto_generated_tags));
			DO_NOT_DELETE_ME_PLS.push_back(false);
		}
		_auto_generated_tags++;
		SceneEntryInst inst = BuildInstance(type, values); // could be asset or object
		if (inst.kind == SceneEntryInst::Kind::Asset) {
			assets[*static_cast<std::string*>(values["tag"])] = static_cast<Asset*>(inst.ptr);
		}
		else {
			objects[*static_cast<std::string*>(values["tag"])] = static_cast<Object*>(inst.ptr);
		}

		int _val = 0;
		for (LValuePair pair : values) {
			if (!DO_NOT_DELETE_ME_PLS[_val]) {
				delete pair.second;
			}
		}
		DO_NOT_DELETE_ME_PLS.clear();
		GetType();
	}


}

const std::string& SceneParser::GetType() {
	if (!Accept(NovaTokenType::OpenBracket)) {
		PushError("Expected [");
		return "";
	}
	Advance();
	std::string& id = tokens[pos].content;
	if (Accept(NovaTokenType::DivOp)) { Advance(); }
	if (!Accept(NovaTokenType::Identifier)) {
		PushError("Expected a type");
		return "";
	}
	id = tokens[pos].content;
	Advance();
	if (!Accept(NovaTokenType::CloseBracket)) {
		PushError("Expected ]");
		return "";
	}
	Advance();
	return id;
}

LValuePair SceneParser::GetLoadableValue() {
	int size_now = DO_NOT_DELETE_ME_PLS.size();
	if (!Accept(NovaTokenType::Identifier)) {
		PushError("Expected loadable value name");
	}
	std::string first = Current().content;
	void* second = nullptr;
	Advance();
	if (!Accept(NovaTokenType::Assignment)) {
		PushError("Expected = after loadable value name");
	}
	Advance();

	if (Accept(NovaTokenType::BoolLit)) {
		if (Current().content == "true") {
			second = new bool(true);
		}
		else {
			second = new bool(false);
		}
		Advance();
	}
	else if (Accept(NovaTokenType::IntLit)) {
		second = new int(std::stoi(Current().content));
		Advance();
	}
	else if (Accept(NovaTokenType::FloatLit)) {
		second = new float(std::stof(Current().content));
		Advance();
	}
	else if (Accept(NovaTokenType::StringLit)) {
		second = new std::string(Current().content);
		Advance();
	}
	else if (Accept(NovaTokenType::VectorLit2)) {
		glm::vec2* vec2 = new glm::vec2();
		Advance();
		if (Accept(NovaTokenType::OpenParen)) {
			Advance();
			vec2->x = std::stof(Current().content);
			Advance();
			vec2->y = std::stof(Current().content);
			Advance();
			Advance();
		}
		second = vec2;
	}
	else if (Accept(NovaTokenType::VectorLit3)) {
		glm::vec3* vec3 = new glm::vec3();
		Advance();
		if (Accept(NovaTokenType::OpenParen)) {
			Advance();
			vec3->x = std::stof(Current().content);
			Advance();
			vec3->y = std::stof(Current().content);
			Advance();
			vec3->z = std::stof(Current().content);
			Advance();
			Advance();
		}
		second = vec3;
	}
	else if (Accept(NovaTokenType::VectorLit4)) {
		glm::vec4* vec4 = new glm::vec4();
		Advance();
		if (Accept(NovaTokenType::OpenParen)) {
			Advance();
			vec4->x = std::stof(Current().content);
			Advance();
			vec4->y = std::stof(Current().content);
			Advance();
			vec4->z = std::stof(Current().content);
			Advance();
			vec4->w = std::stof(Current().content);
			Advance();
			Advance();
		}
		second = vec4;
	}
	else if (Accept(NovaTokenType::Identifier)) {
		auto it = assets.find(Current().content);
		if (it != assets.end()) {
			DO_NOT_DELETE_ME_PLS.push_back(true);
			second = assets[Current().content];
			Advance();
		}
		auto it2 = objects.find(Current().content);
		if (it2 != objects.end()) {
			DO_NOT_DELETE_ME_PLS.push_back(true);
			second = objects[Current().content];
			Advance();
		}
		if (!second) {
			second = new std::string(Current().content);
			Advance();
		}
	}

	if (DO_NOT_DELETE_ME_PLS.size() == size_now) {
		DO_NOT_DELETE_ME_PLS.push_back(false);
	}

	return LValuePair(first, second);
}

void SceneParser::PushError(const std::string& message) const {
	success = false;
	Engine::GetInstance()->PushError("[SceneParser]" + message + " at " + std::to_string(tokens[pos].line) + ", " + std::to_string(tokens[pos].column), false);
}