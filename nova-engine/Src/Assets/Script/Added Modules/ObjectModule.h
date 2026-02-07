#ifndef NOVA_SCRIPT_OBJECT_MODULE_H
#define NOVA_SCRIPT_OBJECT_MODULE_H

#include "../../../nova-script/NovaScript/Library/NovaModule.h"
#include "../../../Objects/Object.h"

class NovaModule;

class NovaEngineModule : public NovaModule {
public:
	NovaEngineModule() {
		module_name = "object";
	}

	nova_std_decl(_get_self) {
		req_args(1);
		objget(args, Object, 0);

	}

	Scope GetModule() override {
		Scope scope;
		
		return scope;
	}
};

#endif