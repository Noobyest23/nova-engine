#ifndef NOVA_SCRIPT_H
#define NOVA_SCRIPT_H

#include "../Asset.h"
#include "../../nova-script/NovaScript.h"

class Script : public Asset {
public:

	Script(const std::string& filepath);

	


protected:

	virtual void OnDestroy() override;
	InterpretorHandle interpretor;

};



#endif