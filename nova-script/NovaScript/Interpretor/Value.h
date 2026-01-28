#ifndef NOVASCRIPT_VALUE_H
#define NOVASCRIPT_VALUE_H

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Scope.h"
#include "../../NovaScript_API.h"
struct Value;
class Interpretor;
struct FuncDeclNode;
struct TypeDeclNode;

using CPPFunction = Value (*)(std::vector<Value*>&);
using NovaFunction = FuncDeclNode*;
using NovaType = Scope;
using CPPVariable = std::variant<
	std::monostate,
	std::reference_wrapper<int>,
	std::reference_wrapper<float>,
	std::reference_wrapper<bool>,
	std::reference_wrapper<std::string>,
	std::reference_wrapper<glm::vec2>,
	std::reference_wrapper<glm::vec3>,
	std::reference_wrapper<glm::vec4>
>;
using ReferenceValue = std::reference_wrapper<Value>;
using NovaTypeDecl = TypeDeclNode*;
struct CPPObject {
	void* ptr = nullptr;
	Scope scope;
};

struct NOVASCRIPT_API Value {
	Value() = default;
	explicit Value(int data) : data(data) {};
	explicit Value(float data) : data(data) {};
	explicit Value(std::string data) : data(data) {};
	explicit Value(bool data) : data(data) {};
	Value(CPPFunction data) : data(data) {};
	Value(NovaFunction data) : data(data) {};
	Value(std::vector<Value>& data) : data(data) {};
	Value(NovaType& data) : data(data) {};
	Value(NovaTypeDecl& data) : data(data) {};
	Value(CPPObject data) : data(data) {};
	Value(glm::vec2 data) : data(data) {};
	Value(glm::vec3 data) : data(data) {};
	Value(glm::vec4 data) : data(data) {};
	explicit Value(CPPVariable data) : data(data) {};
	explicit Value(ReferenceValue data) : data(data) {};

	~Value();

	std::variant <
		std::monostate, // 0
		int,
		float,
		bool,
		std::string,
		std::vector<Value>, // 5
		NovaType,
		NovaFunction,
		CPPFunction,
		CPPVariable,
		NovaTypeDecl, // 10
		CPPObject,
		glm::vec2,
		glm::vec3,
		glm::vec4,
		ReferenceValue // 15
	> data;

	std::string Type() const;
	std::string ToString() const;

	std::string& GetString();
	int& GetInt();
	float& GetFloat();
	float GetNum();
	bool& GetBool();
	glm::vec2& GetVec2();
	glm::vec3& GetVec3();
	glm::vec4& GetVec4();
	std::vector<Value>& GetArray();


	bool IsInt() const;
	bool IsFloat() const;
	bool IsNum() const;
	bool IsString() const;
	bool IsBool() const;
	bool IsFunction() const;
	bool IsVec() const;
	bool IsVec2() const;
	bool IsVec3() const;
	bool IsVec4() const;
	bool IsCPP() const;
	bool IsObj() const;
	bool IsArray() const;
	bool IsReference() const;
	//bool IsCallback() const;

	bool is_manually_created = false;
	void Release() {
		if (is_manually_created) {
			delete this;
		}
	};
};


#endif