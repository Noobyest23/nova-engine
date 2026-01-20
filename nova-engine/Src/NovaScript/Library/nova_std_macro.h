#ifndef NOVASCRIPT_STD_MACROS
#define NOVASCRIPT_STD_MACROS

#include "../Interpretor/Value.h"

static Value null_value = Value();

#define nova_std_decl(name) Value name(std::vector<Value*>& args)
#define req_args(num) if (args.size() < num) { PushError("Expected " + std::string(#num) + " args in function call"); return Value(); }

#define objget(obj, window, cpptype) CPPObject& obj = std::get<CPPObject>(args[0]->data); Window* window = static_cast<Window*>(obj.ptr);
#define strget(str, arg_i) if (!args[arg_i]->IsString()) { PushError("Expected string as argument " + std::to_string(arg_i)); return Value(); } std::string& str = args[arg_i]->GetString();
#define intget(i, arg_i) if (!args[arg_i]->IsInt()) { PushError("Expected int as argument " + std::to_string(arg_i)); return Value(); } int& i = args[arg_i]->GetInt();

#endif