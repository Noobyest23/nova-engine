#include "Value.h"
#include <iostream>
#include "Scope.h"
#include "../ASTNodes/StmtNode.h"
#include "../../pch.h"
std::string Value::Type() const {
	int v_index = static_cast<int>(data.index());

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
		int cpp_v_index = static_cast<int>(var.index());
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
	default:
		return "Unknown type in Value.Type";
	}
	return "";
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
	int v_index = static_cast<int>(data.index());

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
			int cpp_v_index = static_cast<int>(var.index());
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
	default:
		return "Unknown type in Value.ToString";
	}
	return "";
}

std::string& Value::GetString() {
	if (IsString()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<std::string>>(std::get<CPPVariable>(data));
		}
		if (IsReference()) {
			return std::get<ReferenceValue>(data).get().GetString();
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
		if (IsReference()) {
			return std::get<ReferenceValue>(data).get().GetInt();
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
		if (IsReference()) {
			return std::get<ReferenceValue>(data).get().GetFloat();
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
		if (IsReference()) {
			return std::get<ReferenceValue>(data).get().GetBool();
		}
		return std::get<bool>(data);
	}
	throw std::exception("Type is not bool");
}

glm::vec2& Value::GetVec2() {
	if (IsVec3()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<glm::vec2>>(std::get<CPPVariable>(data));
		}
		if (IsReference()) {
			return std::get<ReferenceValue>(data).get().GetVec2();
		}
		return std::get<glm::vec2>(data);
	}
	throw std::exception("Type is not vec2");
}

glm::vec3& Value::GetVec3() {
	if (IsVec3()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<glm::vec3>>(std::get<CPPVariable>(data));
		}
		if (IsReference()) {
			return std::get<ReferenceValue>(data).get().GetVec3();
		}
		return std::get<glm::vec3>(data);
	}
	throw std::exception("Type is not vec3");
}

glm::vec4& Value::GetVec4() {
	if (IsVec4()) {
		if (IsCPP()) {
			return std::get<std::reference_wrapper<glm::vec4>>(std::get<CPPVariable>(data));
		}
		if (IsReference()) {
			return std::get<ReferenceValue>(data).get().GetVec4();
		}
		return std::get<glm::vec4>(data);
	}
	throw std::exception("Type is not vec4");
}

std::vector<Value>& Value::GetArray() {
	if (IsArray()) {
		return std::get<std::vector<Value>>(data);
	}
	if (IsReference()) {
		return std::get<ReferenceValue>(data).get().GetArray();
	}
	throw std::exception("Type is not array");
}

bool Value::IsInt() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsInt();
	if (IsCPP()) {
		int cpp_v_index = static_cast<int>(std::get<CPPVariable>(data).index());
		return cpp_v_index == 0;
	}
	return v_index == 1;
}

bool Value::IsFloat() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsFloat();
	if (IsCPP()) {
		int cpp_v_index = static_cast<int>(std::get<CPPVariable>(data).index());
		return cpp_v_index == 1;
	}
	return v_index == 2;
}

bool Value::IsNum() const {
	return (IsFloat() or IsInt());
}

bool Value::IsString() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsString();
	if (IsCPP()) {
		int cpp_v_index = static_cast<int>(std::get<CPPVariable>(data).index());
		return cpp_v_index == 3;
	}
	return v_index == 4;
}

bool Value::IsBool() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsBool();
	if (IsCPP()) {
		int cpp_v_index = static_cast<int>(std::get<CPPVariable>(data).index());
		return cpp_v_index == 3;
	}
	return v_index == 3;
}

bool Value::IsFunction() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsFunction();
	return v_index == 7 or v_index == 8;
}

bool Value::IsCPP() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsCPP();
	return v_index == 8 or v_index == 9 or v_index == 11;
}

bool Value::IsObj() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsObj();
	return v_index == 6 or v_index == 11;
}

bool Value::IsVec() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsVec();
	if (IsCPP()) {
		int v_v_index = static_cast<int>(std::get<CPPVariable>(data).index());
		return v_v_index == 5 or v_v_index == 6 or v_v_index == 7;
	}
	return v_index == 12 or v_index == 13 or v_index == 14;
}

bool Value::IsVec2() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsVec2();
	if (IsCPP()) {
		return std::get<CPPVariable>(data).index() == 5;
	}
	return data.index() == 12;
}

bool Value::IsVec3() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsVec3();
	if (IsCPP()) {
		return std::get<CPPVariable>(data).index() == 6;
	}
	return data.index() == 13;
}

bool Value::IsVec4() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsVec4();
	if (IsCPP()) {
		return std::get<CPPVariable>(data).index() == 7;
	}
	return data.index() == 14;
}

bool Value::IsArray() const {
	int v_index = static_cast<int>(data.index());
	if (v_index == 15) return std::get<ReferenceValue>(data).get().IsArray();
	return v_index == 5;
}

bool Value::IsReference() const {
	return data.index() == 15;
}

Value::~Value() {
	//int v_index = data.index();
}