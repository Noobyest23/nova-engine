#ifndef NOVASCRIPT_STRING_H
#define NOVASCRIPT_STRING_H

#include "../../Core/Engine.h"
#include "../Interpretor/Scope.h"
#include "nova_std_macro.h"
#include <string>

namespace nova_std_string {
	static void PushError(const std::string& message) {
		Engine* engine = Engine::GetInstance();
		engine->PushError("[NovaScript String Library] " + message);
	}

	nova_std_decl(Length) {
		req_args(1);
		strget(string, 0);
		return Value(int(string.size()));
	}

	nova_std_decl(IsEmpty) {
		req_args(1);
		strget(string, 0);
		return Value(string.empty());
	}

	nova_std_decl(StartsWith) {
		req_args(2);
		strget(string, 0);
		strget(prefix, 1);
		return Value(string.starts_with(prefix));
	}

	nova_std_decl(EndsWith) {
		req_args(2);
		strget(string, 0);
		strget(suffix, 1);
		return Value(string.ends_with(suffix));
	}

	nova_std_decl(Contains) {
		req_args(2);
		strget(string, 0);
		strget(search, 1);
		return Value(string.find(search) != std::string::npos);
	}

	nova_std_decl(ToString) {
		Value* val = args[0];
		return Value(val->ToString());
	}
	
	nova_std_decl(Substr) {
		req_args(3);
		strget(string, 0);
		intget(start, 1);
		intget(len, 2);

		if (start < 0 || len < 0 || start >= string.size()) {
			PushError("Invalid substring range");
			return Value("");
		}

		return Value(string.substr(start, len));
	}

	nova_std_decl(Find) {
		req_args(2);
		strget(string, 0);
		strget(search, 1);

		size_t pos = string.find(search);
		return Value(pos == std::string::npos ? -1 : int(pos));
	}

	// thanks stack overflow very cool
	static std::string TrimImpl(const std::string& s) {
		size_t start = s.find_first_not_of(" \t\n\r");
		if (start == std::string::npos) return "";

		size_t end = s.find_last_not_of(" \t\n\r");
		return s.substr(start, end - start + 1);
	}

	nova_std_decl(Trim) {
		req_args(1);
		strget(string, 0);
		return Value(TrimImpl(string));
	}

	nova_std_decl(Replace) {
		req_args(3);
		strget(string, 0);
		strget(from, 1);
		strget(to, 2);

		size_t pos = string.find(from);
		if (pos == std::string::npos) return Value(string);

		std::string result = string;
		result.replace(pos, from.size(), to);
		return Value(result);
	}

	nova_std_decl(ToUpper) {
		req_args(1);
		strget(string, 0);
		std::string out = string;
		for (char& c : out) {
			c = std::toupper((unsigned char)c);
		}

		return Value(out);
	}

	nova_std_decl(ToLower) {
		req_args(1);
		strget(string, 0);
		std::string out = string;
		for (char& c : out) {
			c = std::tolower((unsigned char)c);
		}

		return Value(out);
	}

	static Value GetModule() {
		Scope scope;
		
		scope.Set("Length", Length);
		scope.Set("IsEmpty", IsEmpty);
		scope.Set("StartsWith", StartsWith);
		scope.Set("EndsWith", EndsWith);
		scope.Set("Contains", Contains);
		
		scope.Set("Substr", Substr);
		scope.Set("Find", Find);
		scope.Set("Trim", Trim);
		scope.Set("Replace", Replace);
		scope.Set("ToUpper", ToUpper);
		scope.Set("ToLower", ToLower);
		
		scope.Set("ToString", ToString);

		return scope;
	}
}


#endif