#ifndef NOVA_SCRIPT_PUSH_ERROR
#define NOVA_SCRIPT_PUSH_ERROR

#include <string>

static bool _use_console = false;

class Callbacker {
public:

	static void PushError(const std::string& message, int sevarity);
	static void ExitCallback(const std::string& message);
	static void ProjectSetCallback(const std::string& message);

	static void(*_PushErrorCallback)(const char* message, int sevarity);
	static void(*_ExitCallback)(const char* message);
	static std::string _proj_path;
	static void(*_ProjectPathSetCallback)(const char* path);
	
};

#endif