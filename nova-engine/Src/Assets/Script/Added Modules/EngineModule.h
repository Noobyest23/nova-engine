#ifndef NOVA_SCRIPT_ENGINE_MODULE_H
#define NOVA_SCRIPT_ENGINE_MODULE_H

#include "../../../nova-script/NovaScript/Library/NovaModule.h"
#include "../../../Core/Engine.h"
#include "../../../Platform/Agnostic/Window.h"
class NovaModule;

class NovaEngineModule : public NovaModule {
public:
	NovaEngineModule() {
		module_name = "engine";
	}

	nova_std_decl(window_get_size) {
		req_args(1);
		objget(window, Window, 0);
		Value size = (window->GetSize());
		return size;
	}
	nova_std_decl(window_get_name) {
		req_args(1);
		objget(window, Window, 0);
		Value name = Value(window->GetName());
		return name;
	}
	nova_std_decl(window_set_size) {
		req_args(2);
		objget(window, Window, 0);
		glm::vec2 size = std::get<glm::vec2>(args[1]->data);
		window->SetSize(size);
		return null_value;
	}
	nova_std_decl(window_set_name) {
		req_args(2);
		objget(window, Window, 0);
		std::string name = args[1]->GetString();
		window->SetName(name);
		return null_value;
	}
	nova_std_decl(GetWindow) {
		CPPObject WindowType;
		WindowType.ptr = Engine::GetInstance()->window;
		WindowType.scope.Set("GetSize", window_get_size);
		WindowType.scope.Set("SetSize", window_set_size);
		WindowType.scope.Set("SetName", window_set_name);
		WindowType.scope.Set("GetName", window_get_name);
		return WindowType;
	}

	nova_std_decl(InitProjectPath) {
		req_args(1);
		strget(string, 0);
		if (!string.ends_with("/") or !string.ends_with("\\")) {
			string.append("/");
		}
		Engine* engine = Engine::GetInstance();
		engine->InitProjectPath(string);
		return null_value;
	}

	nova_std_decl(Exit) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Engine* engine = Engine::GetInstance();
		engine->PushError(output, true);
		return null_value;
	}

	Scope GetModule() override {
		Engine* engine = Engine::GetInstance();
		Scope scope;
		scope.Set("Exit", Exit);
		scope.Set("GetWindow", GetWindow);
		scope.Set("SupressWarningPopup", Value(CPPVariable(engine->suppress_warning_popup)));
		scope.Set("SupressErrorPopup", Value(CPPVariable(engine->suppress_error_popup)));
		scope.Set("InitProjectPath", InitProjectPath);
		return scope;
	}
};


#endif
