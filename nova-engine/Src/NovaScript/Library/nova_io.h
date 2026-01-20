#ifndef NOVASCRIPT_IO_H
#define NOVASCRIPT_IO_H

#include "nova_std_macro.h"
#include "../../Core/Engine.h"
#include "../Interpretor/Scope.h"

#include <fstream>

namespace nova_std_io {
	static void PushError(const std::string& message) {
		Engine* engine = Engine::GetInstance();
		engine->PushError("[NovaScript IO Library] " + message);
	}

	nova_std_decl(Print) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Engine* engine = Engine::GetInstance();
		engine->PushMessage(output);
		return null_value;
	}

	nova_std_decl(PrintWarning) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Engine* engine = Engine::GetInstance();
		engine->PushMessage(output, true);
		return null_value;
	}

	nova_std_decl(PrintError) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Engine* engine = Engine::GetInstance();
		engine->PushError(output);
		return null_value;
	}

	nova_std_decl(LoadText) {
		std::string filepath;
		if (args[0]->IsString()) {
			filepath = args[0]->GetString();
		}
		else {
			PushError("Argument 0 is not string");
			return Value();
		}

		std::ifstream file(filepath, std::ios::binary);
		std::string text;
		std::string line;

		if (!file) {
			PushError("Failed to open: " + std::string(filepath));
			return Value(false);
		}

		while (std::getline(file, line)) {
			text += line + "\n";
		}

		return Value(text);
	}

	nova_std_decl(SaveText) {
		std::string filepath;
		std::string content;
		if (args[0]->IsString()) {
			filepath = args[0]->GetString();
		}
		else {
			PushError("Argument 0 is not string");
		}
		if (args[1]->IsString()) {
			content = args[1]->GetString();
		}
		else {
			PushError("Argument 1 is not string");
		}

		std::ofstream file(filepath, std::ios::binary);
		std::string text;
		std::string line;

		if (!file) {
			PushError("Failed to open: " + std::string(filepath));
			return Value(false);
		}

		file << content;

		return Value(true);
	}

	static Value GetModule() {
		Scope scope;
		// I
		scope.Set("LoadText", LoadText);
		// O
		scope.Set("Print", Print);
		scope.Set("PrintWarning", PrintWarning);
		scope.Set("PrintError", PrintError);
		scope.Set("SaveText", SaveText);
		return scope;
	}
}


#endif
