#include "TCMSignalHandler/SignalHandler/SignalHandler.h"
#include "TCMLogger/Logger.h"
#include <mutex>
#include "TCMSignalHandler/Windows/WindowsSignalHandler.h"

typedef TCM::Engine::Signal::Windows::WindowsSignalHandler TCMOverridedSignalHandler;

namespace TCM
{
	namespace Engine
	{
		namespace Signal
		{
			SignalHandler& SignalHandler::GetInstance()
			{
				static std::unique_ptr<SignalHandler> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new TCMOverridedSignalHandler() );
				                }
				);

				return *m_instance.get();
			}
		}
	}
}
