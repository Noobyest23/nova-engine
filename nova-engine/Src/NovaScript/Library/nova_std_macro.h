#ifndef NOVASCRIPT_STD_MACROS
#define NOVASCRIPT_STD_MACROS

#include "../Interpretor/Value.h"

static Value null_value = Value();

#define nova_std_decl(name) Value name(std::vector<Value*>& args)

#define objget(obj, window, cpptype) CPPObject& obj = std::get<CPPObject>(args[0]->data); Window* window = static_cast<Window*>(obj.ptr);

#endif