#include "main.h"
#include <iostream>
#include <string>
#include "../../Core/Engine.h"
int main()
{
   
	Engine engine;
	engine.Init();
	engine.TestEnv();
	engine.Run();
	engine.Shutdown();


	return 0;
}