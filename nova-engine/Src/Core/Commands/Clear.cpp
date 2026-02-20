#include "Clear.h"

void ClearCommand::Execute(const std::vector<std::string>& args) {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}