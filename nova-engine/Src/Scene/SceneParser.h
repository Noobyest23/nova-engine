#ifndef NOVA_SCENE_PARSER_H
#define NOVA_SCENE_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
class Object;
class Asset;
#include "Tokens.h"
using LoadableValues = std::unordered_map<std::string, void*>;
using LValuePair = std::pair<std::string, void*>;

struct SceneEntryInst {
	enum class Kind{Object, Asset};
	Kind kind;
	void* ptr = nullptr;
};

class SceneParser {
public:

	Object* Parse(const std::string& filepath);

private:
	
	void Advance() { pos++; }
	bool Accept(const NovaTokenType& type) const { return (tokens[pos].type == type); }
	Token& Next() { return tokens[pos + 1]; }
	Token& Current() { return tokens[pos]; }
	void Loop();

	SceneEntryInst BuildInstance(const std::string& type, LoadableValues&);

	const std::string& GetType();
	LValuePair GetLoadableValue();
	int pos = 0;
	std::vector<Token> tokens;
	mutable bool success = true;
	void PushError(const std::string& message) const;

	std::unordered_map<std::string, Asset*> assets;
	std::unordered_map<std::string, Object*> objects;

	std::vector<bool> DO_NOT_DELETE_ME_PLS;

	std::vector<std::string> on_root_add;

	int _auto_generated_tags = 0;
};

#endif