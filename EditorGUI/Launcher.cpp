#include "Launcher.h"
#include <memory>
#include <mutex>
#include "TCMCore/Core.h"

#if defined(TCM_QT_API)
#include "LauncherQt.h"
typedef TCM::Editor::QT::LauncherQt TCMCoreEditor;
#endif //  _WIN32

int TCM::Editor::Launcher::LaunchEditor( int& argc, char** argv )
{
	return GetInstance().CoreLaunchEditor( argc, argv );
}

TCM::Editor::Launcher& TCM::Editor::Launcher::GetInstance()
{
	static std::unique_ptr<TCM::Editor::Launcher> m_instance;
	static std::once_flag m_onceFlag;
	std::call_once( m_onceFlag,
	                []
	                {
		                m_instance.reset( new TCMCoreEditor );
	                } );
	return *m_instance.get();
}
