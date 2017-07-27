#pragma once
#include "TCMSignalHandler/SignalHandler/SignalHandler.h"

namespace TCM
{
	namespace Engine
	{
		namespace Signal
		{
			namespace Windows
			{
				class WindowsSignalHandler final : public SignalHandler
				{
				public:
					WindowsSignalHandler() = default;
					~WindowsSignalHandler() = default;

				private:
					void CoreRegisterSignalHandler( Signals signalToHandle, void (*functionToExecuteIfSignalIsRaised)() ) override;
				};
			}
		}
	}
}
