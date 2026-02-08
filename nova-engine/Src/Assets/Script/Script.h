#ifndef NOVA_SCRIPT_H
#define NOVA_SCRIPT_H

#include "../Asset.h"
#include "../../nova-script/NovaScript.h"
#include "../../nova-script/NovaScript/Interpretor/Value.h"

class Script : public Asset {
public:

	Script(const std::string& filepath);

	void Set(const std::string& name, Value val);
	Value* Get(const std::string& name);
	Scope* GetAll();
	Value Call(const std::string& name, std::vector<Value*> values);

protected:

	virtual void OnDestroy() override;
	InterpretorHandle interpretor;

};



#endif