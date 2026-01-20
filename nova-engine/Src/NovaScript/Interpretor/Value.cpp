#include "Value.h"
#include <iostream>
#include "Scope.h"

std::string Value::Type() const {
	int v_index = data.index();

	switch (v_index) {
	case 0: // null
		return "null";
		break;
	case 1: // int
		return "int";
		break;
	case 2: // float
		return "float";
		break;
	case 3: // bool
		return "bool";
		break;
	case 4: // string
		return "string";
		break;

	case 5: { // Array 
		return "array";
		break;
	}
	case 6: // NovaType
		return "ObjectType";
		break;
	case 7: // NovaScript func
		return "Function";
		break;
	case 8: // cpp function
		return "C++ Function";
		break;

	case 9: {// C++ Variable
		CPPVariable var = std::get<CPPVariable>(data);
		int cpp_v_index = var.index();
		switch (cpp_v_index) {
		case 0:
			return "C++ null";
			break;
		case 1: // int
			return "C++ int";
			break;
		case 2: // float
			return "C++ float";
			break;
		case 3: // bool
			return "C++ bool";
			break;
		case 4: // string
			return "C++ array";
			break;
		}
		break;
	}
	case 10: // NovaType Decl
		return "ObjectType Declaration";
		break;
	case 11: // CPP
		return "C++ Object";
		break;
	case 12: // Vec2
		return "Vec2";
		break;
	case 13: // Vec3
		return "Vec3";
		break;
	case 14: // Vec4
		return "Vec4";
		break;
	}
}
 /*
 std::variant<
		std::monostate,
		int,
		float,
		bool,
		std::string,
		std::vector<Value*>,
		NovaType,
		NovaFunction,
		CPPFunction,
		CPPVariable,
		NovaTypeDecl,
		CPPTypeDecl
	> data;
 */
std::string Value::ToString() const {
	int v_index = data.index();

	switch (v_index) {
	case 0: // null
		return "null";
		break;
	case 1: // int
		return std::to_string(std::get<int>(data));
		break;
	case 2: // float
		return std::to_string(std::get<float>(data));
		break;
	case 3: // bool
		return std::get<bool>(data) ? "true" : "false";
		break;
	case 4: // string
		return std::get<std::string>(data);
		break;

	case 5: { // Array 
		std::vector<Value> list = std::get<std::vector<Value>>(data);
		std::string output = "[";
		int i = 0;
		for (Value& other_var : list) {
			output += other_var.ToString();
			if (i != list.size() - 1) {
				output += ", ";
			}
			else {
				output += "]";
			}
			i++;
		}
		return output;
		break;
	}
	case 6: // NovaType
		return "ObjectType";
		break;
	case 7: // NovaScript func
		return "Function";
		break;
	case 8: // cpp function
		return "C++ Function";
		break;

	case 9: {// C++ Variable 
			CPPVariable var = std::get<CPPVariable>(data);
			int cpp_v_index = var.index();
			switch (cpp_v_index) {
			case 0: // null 
				return "null";
				break;
			case 1: // int
				return std::to_string(std::get<std::reference_wrapper<int>>(var));
				break;
			case 2: // float
				return std::to_string(std::get<std::reference_wrapper<float>>(var));
				break;
			case 3: // bool
				return std::get<std::reference_wrapper<bool>>(var) ? "true" : "false";
				break;
			case 4: // string
				return std::get<std::reference_wrapper<std::string>>(var);
				break;
			}
			break;
		}
	case 10: // NovaType Decl
		return "ObjectType Declaration";
		break;
	case 11: // CPP Object
		return "C++ Object";
		break;
	case 12: {// Vec2
		glm::vec2 vector = std::get<glm::vec2>(data);
		std::string output = "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ")";
		return output;
		break;
	}
	case 13: {// Vec3 
		glm::vec3 vector = std::get<glm::vec3>(data);
		std::string output = "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z) + ")";
		return output;
		break;
	}
	case 14: {// Vec4
		glm::vec4 vector = std::get<glm::vec4>(data);
		std::string output = "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z) + ", " + std::to_string(vector.w) + ")";
		return output;
		break;
	}

	}
	
}

std::string& Value::GetString() {
	if (IsString()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<std::string>>(std::get<CPPVariable>(data));
		}
		return std::get<std::string>(data);
	}
	throw std::exception("Type is not string");
}

int& Value::GetInt() {
	if (IsInt()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<int>>(std::get<CPPVariable>(data));
		}
		return std::get<int>(data);
	}
	throw std::exception("Type is not int");
}

float& Value::GetFloat() {
	if (IsFloat()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<float>>(std::get<CPPVariable>(data));
		}
		return std::get<float>(data);
	}
	throw std::exception("Type is not float");
}

float Value::GetNum() {
	if (IsNum()) {
		if (IsInt()) {
			return float(GetInt());
		}
		else {
			return GetFloat();
		}
	}
	throw std::exception("Type is not num");
}

bool& Value::GetBool() {
	if (IsBool()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<bool>>(std::get<CPPVariable>(data));
		}
		return std::get<bool>(data);
	}
	throw std::exception("Type is not bool");
}

glm::vec2& Value::GetVec2() {
	if (data.index() == 12) {
		return std::get<glm::vec2>(data);
	}
	throw std::exception("Type is not vec2");
}

glm::vec3& Value::GetVec3() {
	if (data.index() == 13) {
		return std::get<glm::vec3>(data);
	}
	throw std::exception("Type is not vec3");
}

glm::vec4& Value::GetVec4() {
	if (data.index() == 14) {
		return std::get<glm::vec4>(data);
	}
	throw std::exception("Type is not vec4");
}

std::vector<Value>& Value::GetArray() {
	if (IsArray()) {
		return std::get<std::vector<Value>>(data);
	}
	throw std::exception("Type is not array");
}

bool Value::IsInt() const {
	int v_index = data.index();
	if (IsCPP()) {
		int cpp_v_index = std::get<CPPVariable>(data).index();
		return cpp_v_index == 0;
	}
	return v_index == 1;
}

bool Value::IsFloat() const {
	int v_index = data.index();
	if (IsCPP()) {
		int cpp_v_index = std::get<CPPVariable>(data).index();
		return cpp_v_index == 1;
	}
	return v_index == 2;
}

bool Value::IsNum() const {
	return (IsFloat() or IsInt());
}

bool Value::IsString() const {
	int v_index = data.index();
	if (IsCPP()) {
		int cpp_v_index = std::get<CPPVariable>(data).index();
		return cpp_v_index == 3;
	}
	return v_index == 4;
}

bool Value::IsBool() const {
	int v_index = data.index();
	if (IsCPP()) {
		int cpp_v_index = std::get<CPPVariable>(data).index();
		return cpp_v_index == 3;
	}
	return v_index == 3;
}

bool Value::IsFunction() const {
	int v_index = data.index();
	return v_index == 7 or v_index == 8;
}

bool Value::IsCPP() const {
	int v_index = data.index();
	return v_index == 8 or v_index == 9 or v_index == 11;
}

bool Value::IsObj() const {
	return data.index() == 6 or 11;
}

bool Value::IsVec() const {
	return data.index() == 12 or 13 or 14;
}

bool Value::IsArray() const {
	return data.index() == 5;
}

Value::~Value() {
	//int v_index = data.index();
}