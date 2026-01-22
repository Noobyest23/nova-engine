#ifndef NOVASCRIPT_STD_MACROS
#define NOVASCRIPT_STD_MACROS

#include "../Interpretor/Value.h"

static Value null_value = Value();

#define nova_std_decl(name) Value name(std::vector<Value*>& args)
#define req_args(num) if (args.size() < num) { PushError("Expected " + std::string(#num) + " args in function call"); return Value(); }

#define strget(str, arg_i) if (!args[arg_i]->IsString()) { PushError("Expected string as argument " + std::to_string(arg_i)); return Value(); } std::string& str = args[arg_i]->GetString();
#define intget(i, arg_i) if (!args[arg_i]->IsInt()) { PushError("Expected int as argument " + std::to_string(arg_i)); return Value(); } int& i = args[arg_i]->GetInt();
#define floatget(f, arg_i) if (!args[arg_i]->IsFloat()) { PushError("Expected float as argument " + std::to_string(arg_i)); return Value(); } float& f = args[arg_i]->GetFloat();
#define vec2get(v, arg_i) if (!args[arg_i]->IsVec2()) {PushError("Expected Vector2 as argument " + std::to_string(arg_i)); return Value(); } glm::vec2& v = args[arg_i]->GetVec2();
#define vec3get(v, arg_i) if (!args[arg_i]->IsVec3()) {PushError("Expected Vector3 as argument " + std::to_string(arg_i)); return Value(); } glm::vec3& v = args[arg_i]->GetVec3();
#define vec4get(v, arg_i) if (!args[arg_i]->IsVec4()) {PushError("Expected Vector4 as argument " + std::to_string(arg_i)); return Value(); } glm::vec4& v = args[arg_i]->GetVec4();

template<typename T>
T* ObjGet(std::vector<Value*>&args, int i) {
	CPPObject& obj = std::get<CPPObject>(args[i]->data);
	return static_cast<T*>(obj.ptr);
}

#define objget(obj, type, arg_i) type* obj = ObjGet<type>(args, arg_i);

#endif