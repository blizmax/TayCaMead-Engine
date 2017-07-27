// GraphicalInterface.cpp : Defines the entry point for the console application.
//

#pragma comment (lib, "TCMCore/TCMCore.lib")

#include "TCMCore/Core.h"

int main()
{
	TCM::Engine::Core::Initialize();
	TCM::Engine::Core::RunExampleScene();
	TCM::Engine::Core::Shutdown();

	return 0;
}
