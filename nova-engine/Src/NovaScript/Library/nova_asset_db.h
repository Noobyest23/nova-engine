#ifndef NOVASCRIPT_ASSET_DB_LIB
#define NOVASCRIPT_ASSET_DB_LIB

#include "nova_std_macro.h"

namespace nova_asset_db_init {

	static bool basic_2D = false;
	static bool basic_2D_shapes = false;
	static bool basic_2D_shaders = false;

	static Scope GetModule() {
		Scope scope;
		scope.Set("add_basic_2D", Value(CPPVariable(basic_2D)));
		scope.Set("add_basic_2D_shapes", Value(CPPVariable(basic_2D_shapes)));
		scope.Set("add_basic_2D_shaders", Value(CPPVariable(basic_2D_shaders)));
		return scope;
	}

}

#endif
