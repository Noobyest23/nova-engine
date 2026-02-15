#include "Engine.h"

Engine* Engine::engine_inst = nullptr;
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
	PushMessage("[Engine Init] Compiled in debug mode, adding developer camera (toggle with F1)");
	DevCamera2D* dev_cam = new DevCamera2D;
	scene->root->AddChild(dev_cam);
	dev_cam->active = false;
	scene->root->MoveChild(scene->root->GetChildrenCount() - 1, 0);
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
	Script* script = new Script("Scripts/LoopTest.ns");
	script->Release();
	return 0;
}

int Engine::Run() {
	scene->Ready();
	while (window->Update()) {
		if (should_stop) break;
		glClearColor(0.1f, 0.1f, 0.1f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		while (window->IsEventsQueued()) {
			InputEvent* e = window->EventGetNext();
			scene->Input(e);
			delete e;
		}
		*/
		scene->Update(0.16f);
		scene->Draw();

		glfwSwapBuffers(window->GetWindowHandle());
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
		std::cout << "\033[36m" << message << "\033[0m\n";
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
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

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
