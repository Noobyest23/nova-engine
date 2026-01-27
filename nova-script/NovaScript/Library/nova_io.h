#ifndef NOVASCRIPT_IO_H
#define NOVASCRIPT_IO_H

#ifdef _WIN32
#include <Windows.h>
#include <string>
#include "../../NovaErrorPush.h"

struct InputDlgData {
	std::string* userInput;
	std::string prompt;
};

INT_PTR CALLBACK InputDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		InputDlgData* data = reinterpret_cast<InputDlgData*>(lParam);
		SetWindowLongPtr(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

		SetDlgItemTextA(hwndDlg, 1100, data->prompt.c_str());

		SetDlgItemTextA(hwndDlg, 1001, "");

		return TRUE;
	}

	case WM_COMMAND: {
		InputDlgData* data = reinterpret_cast<InputDlgData*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
		if (!data) return FALSE;

		if (LOWORD(wParam) == IDOK) {
			char buf[256] = {};
			GetDlgItemTextA(hwndDlg, 1001, buf, sizeof(buf));
			*data->userInput = buf;
			EndDialog(hwndDlg, IDOK);
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL) {
			data->userInput->clear();
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	}
	return FALSE;
}
#endif
#include "nova_std_macro.h"
#include "../Interpretor/Scope.h"

#include <fstream>
#include <iostream>



namespace nova_std_io {
	static void PushError(const std::string& message) {
		Callbacker::PushError(message.c_str(), 2);
	}

	nova_std_decl(Print) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 0);
		return null_value;
	}

	nova_std_decl(PrintWarning) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 1);
		return null_value;
	}

	nova_std_decl(PrintError) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 2);
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
		filepath = Callbacker::_proj_path + filepath;
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
		filepath = Callbacker::_proj_path + filepath;
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

	nova_std_decl(Input) {
		req_args(1);
		strget(string, 0);
		std::string in;
		#ifdef USE_CONSOLE
		std::cout << string << " >: ";
		std::getline(std::cin, in);
		return Value(in);
		#else
		#ifdef _WIN32
		
		InputDlgData dlgData;
		dlgData.userInput = &in;
		dlgData.prompt = string;

		DialogBoxParamA(
			GetModuleHandle(nullptr),
			MAKEINTRESOURCEA(101),
			nullptr,
			InputDlgProc,
			reinterpret_cast<LPARAM>(&dlgData)
		);

		return Value(in);
		#endif
		#endif
	}

	static Value GetModule() {
		Scope scope;
		// I
		scope.Set("LoadText", LoadText);
		scope.Set("Input", Input);
		// O
		scope.Set("Print", Print);
		scope.Set("PrintWarning", PrintWarning);
		scope.Set("PrintError", PrintError);
		scope.Set("SaveText", SaveText);
		return scope;
	}
}


#endif
