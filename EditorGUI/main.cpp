#include "Launcher.h"

int main( int argc, char* argv[] )
{
	int result = TCM::Editor::Launcher::LaunchEditor( argc, argv );
	//TCM::Core::Core::Shutdown();
	return result;
}
