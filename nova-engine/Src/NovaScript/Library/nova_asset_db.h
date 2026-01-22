#ifndef NOVASCRIPT_ASSET_DB_LIB
#define NOVASCRIPT_ASSET_DB_LIB

#include "nova_std_macro.h"

namespace nova_asset_db_init {

	bool basic_2D_shapes = false;

	Scope GetModule() {
		Scope scope;
		scope.Set("add_basic_2D_shapes", Value(CPPVariable(basic_2D_shapes)));
		
	}

}

#endif
