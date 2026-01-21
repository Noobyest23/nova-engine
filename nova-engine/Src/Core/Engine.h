#ifndef NOVA_ENGINE_H
#define NOVA_ENGINE_H

#include "Scene.h"
#include "../Platform/Agnostic/Window.h"

//#define USE_CONSOLE
#ifdef _MSC_VER

#ifdef USE_CONSOLE
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:WinMainCRTStartup")
#endif
#endif

class Engine {
public:

	void Init();
	void Shutdown();

	int TestEnv();
	int Run();

	void PushMessage(const std::string& text, bool important = false);
	void PushError(const std::string& text, bool stop_execution = false);
	void Stop();

	Scene* scene = nullptr;
	Window* window = nullptr;

	static Engine* GetInstance() { return engine_inst; };

	bool suppress_warning_popup = false;
	bool suppress_error_popup = false;

private:

	void ShowBootMessage();

	bool should_stop = false;
	static Engine* engine_inst;

};

#endif
