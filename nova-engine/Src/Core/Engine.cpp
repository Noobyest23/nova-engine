#include "Engine.h"

Engine* Engine::engine_inst = nullptr;
SceneEntryInst* Engine::cmd_current_obj = nullptr;
bool Engine::used_cmd_obj = false;
#include "../Scene/Scene.h"
#include "../Platform/Agnostic/Window.h"
#include <iostream>
#include "AssetDB.h"
#include <fstream>
#include "../../nova-script/NovaScript.h"
#include "../../nova-script/NovaErrorPush.h"
#include "../Assets/Script/Script.h"
#include "../Objects/2D/_Internal/DevCamera2D.h"
#include <filesystem>
#include <chrono>
#include "Input/Input.h"
#include "Commands/Command.h"

#ifdef _WIN32
#include "Windows.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#endif


static void ScriptPushError(const char* message, int sevarity) {
	Engine* engine = Engine::GetInstance();
	switch (sevarity) {
	case 0:
		engine->PushMessage(message);
		break;
	case 1:
		engine->PushMessage(message, true);
		break;
	case 2:
		engine->PushError(message);
		break;
	default:
		engine->PushError("[Error Sevarity Unknown]" + std::string(message));
		return;
	}
}

static void ScriptExit(const char* message) {
	Engine* engine = Engine::GetInstance();
	engine->PushError(message, true);
}

static void ScriptSetProjectPath(const char* path) {
	Engine* engine = Engine::GetInstance();
	engine->SetterProjectPath(path);
}

void Engine::Init() {
	Engine::engine_inst = this;
	ShowBootMessage();

	PushMessage("[Engine Init] Initializing window...");
	Window::Init();
	window = new Window("Nova Engine", 800, 600);

	PushMessage("[Engine Init] Initializing Input...");
	Input::Init(window);

	PushMessage("[Engine Init] Linking NovaScript Library...");
	SetErrorCallback(ScriptPushError);
	SetExitCallback(ScriptExit);
	SetProjectPath(ScriptSetProjectPath);
#ifdef USE_CONSOLE
	_use_console = true;
#endif

	PushMessage("[Engine Init] Initializing AssetDB...");
	AssetDB::Init();

	PushMessage("[Engine Init] Running Custom Init Script...");
	Script* init_script = new Script("NovaData/CustomInit.ns");
	init_script->Release();
	
	PushMessage("[Engine Init] Loading main scene");
	scene = new Scene(initial_scene);

	#ifdef _DEBUG
	PushMessage("[Engine Init] Compiled in debug mode");
	PushMessage("[Engine Init] adding developer camera (toggle with F1)");
	DevCamera2D* dev_cam = new DevCamera2D;
	scene->root->AddChild(dev_cam);
	dev_cam->active = false;
	scene->root->MoveChild(scene->root->GetChildrenCount() - 1, 0);
	#ifdef USE_CONSOLE
	PushMessage("[Engine Init] Commands are enabled, Press F2 and type 'help' to get started");
	commands = Command::GetAll();
	#endif
	#endif

	PushMessage("[Engine Init] Finished Engine init");
	PushMessage("[Engine] Warning Nova Engine and all parts of Nova Engine (Especially NovaScript) are still heavily in development", true);
	PushMessage("[Engine] Things will most likely change, and crashes / memory leaks should be expected", true);
	
}

void Engine::Shutdown() {
	delete window;
	if (scene) {
		scene->Shutdown();
	}
	AssetDB::Shutdown();
	Window::Cleanup();
}

int Engine::TestEnv() {
	
	return 0;
}

int Engine::Run() {
	scene->Ready();
	float last_frame_time = 0.0f;
	while (window->Update()) {
		if (should_stop) break;
		glClearColor(0.1f, 0.1f, 0.1f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float current_frame_time = static_cast<float>(glfwGetTime());
		float delta_time = current_frame_time - last_frame_time;

		#ifdef _DEBUG
		#ifdef USE_CONSOLE
		if (Input::IsKeyDown(GLFW_KEY_F2)) {
			ProcessCommand();
		}
		#endif
		#endif

		scene->Update(delta_time);
		scene->Draw();

		glfwSwapBuffers(window->GetWindowHandle());
		last_frame_time = current_frame_time;
		Input::Update();
	}

	

	return 0;
}

void Engine::Stop() {
	should_stop = true;
}

void Engine::ShowBootMessage() {
	const std::string filepath = "NovaData/BootMessage.txt";

	std::ifstream file(filepath, std::ios::binary);
	std::string line;

	if (!file) {
		PushError("Failed to open: " + std::string(filepath));
		return;
	}

	std::cout << file.rdbuf() << "\n";
}

void Engine::SetterProjectPath(const std::string& path) {
	std::filesystem::current_path(path);
	PushMessage("[Engine] Set Executable Filepath to " + path);
}

#ifdef USE_CONSOLE
void Engine::PushMessage(const std::string& message, bool important) {
	if (important and not suppress_warning_popup) {
		std::cout << "\033[33m" << message << "\033[0m\n";
	}
	else if (not important) {
		std::cout << message << "\n";
	}
	
}

void Engine::PushError(const std::string& message, bool stop_execution) {
	if (not suppress_error_popup) {
		std::cout << "\033[31m" << message << "\033[0m\n";
	}
	if (stop_execution) {
		Stop();
	}
}


#elif _WIN32

std::wstring ToWide(const std::string& str)
{
	if (str.empty()) return {};

	int size = MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		-1,
		nullptr,
		0
	);

	std::wstring result(size, 0);

	MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		-1,
		result.data(),
		size
	);

	return result;
}

void Engine::PushMessage(const std::string& message, bool important) {
	if (important and not suppress_warning_popup) {
		HWND hwnd = glfwGetWin32Window(window->GetWindowHandle());

		MessageBoxW(hwnd, ToWide(message).c_str(), NULL, MB_OK | MB_ICONWARNING);

	}
}

void Engine::PushError(const std::string& message, bool stop_execution) {
	if (not suppress_error_popup) {
		HWND hwnd = glfwGetWin32Window(window->GetWindowHandle());

		MessageBoxW(hwnd, ToWide(message).c_str(), NULL, MB_OK | MB_ICONERROR);
	}
	if (stop_execution) {
		Stop();
	}
}

#endif

void Engine::ProcessCommand() {
	// Search for the terminal process and then bring it to the foreground.
#ifdef _WIN32

	HWND consoleHandle = GetConsoleWindow();
	if (consoleHandle) {
		ShowWindow(consoleHandle, SW_SHOW);
		SetForegroundWindow(consoleHandle);
	}
#endif
	std::string command;
	std::cout << "> ";
	std::getline(std::cin, command);
	std::istringstream iss(command);
	std::vector<std::string> parts;
	std::string token;
	while (iss >> token) {
		if (token.front() == '"') {
			std::string str_token = token.substr(1);
			while (str_token.back() != '"' && iss >> token) {
				str_token += " " + token;
			}
			if (str_token.back() == '"') {
				str_token.pop_back();
				parts.push_back(str_token);
			}
			else {
				PushError("Unterminated string in command");
				return;
			}
		}
		else {
			parts.push_back(token);
		}
	}

	if (!parts.empty()) {
		auto it = commands.find(parts[0]);
		bool do_not_tab_back = false;
		if (parts.back() == "||") {
			do_not_tab_back = true;
			parts.pop_back();
		}
		if (it != commands.end()) {
			it->second->Execute(parts);
		}
		else {
			PushMessage("[Engine] Command not found: " + parts[0]);
		}
		if (do_not_tab_back) {
			ProcessCommand();
		}
		else {
			#ifdef _WIN32
			HWND hwnd = glfwGetWin32Window(window->GetWindowHandle());
			SetForegroundWindow(hwnd);
			#endif
		}
	}
}
