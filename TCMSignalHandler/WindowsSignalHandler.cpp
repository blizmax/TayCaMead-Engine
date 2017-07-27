#include "../Common/TCMSignalHandler/Windows/WindowsSignalHandler.h"
#include <signal.h>
#include "TCMLogger/Logger.h"
#include <string>

namespace
{
	std::string SigIntToString( int sig )
	{
		switch ( sig )
		{
			case SIGABRT:
				return "Abort signal";
			case SIGFPE:
				return "Floating point exception signal";
			case SIGILL:
				return "Illegal Instruction signal";
			case SIGINT:
				return "Interruption signal";
			case SIGSEGV:
				return "Segmentation fault signal";
			case SIGTERM:
				return "Termination Signal";
			default:
				return "Unknow Signal";
		}
	}

	void (*functionToExecuteIfRaised)() = nullptr;
}

namespace TCM
{
	namespace Engine
	{
		namespace Signal
		{
			namespace Windows
			{
				void WindowsSignalHandler::CoreRegisterSignalHandler( Signals signalToHandle, void (*functionToExecuteIfSignalIsRaised)() )
				{
					int wantedSignal = 0;
					switch ( signalToHandle )
					{
						case ABORT:
						{
							wantedSignal = SIGABRT;
							break;
						}
						case FLOATING_POINT_EXCEPTION:
						{
							wantedSignal = SIGFPE;
							break;
						}
						case ILLEGAL_INSTRUCTION:
						{
							wantedSignal = SIGILL;
							break;
						}
						case INTERRUPTION:
						{
							wantedSignal = SIGINT;
							break;
						}
						case SEGMENTATION_FAULT:
						{
							wantedSignal = SIGSEGV;
							break;
						}
						case TERMINATION:
						{
							wantedSignal = SIGTERM;
							break;
						}
						default:
						{
							break;
						}
					}

					if ( wantedSignal == 0 )
						return;

					functionToExecuteIfRaised = functionToExecuteIfSignalIsRaised;

					void (*signalFunc)( int ) = [](int signalHandled)
							{
								std::string sigStr = SigIntToString( signalHandled );
								TCMFAILURE("=-=-=-=-= " + sigStr + " received" + " =-=-=-=-=");
								TCMFAILURE("Executing " + sigStr + " handler function ...");
								auto func = functionToExecuteIfRaised;
								func();
								TCMFAILURE("If you see this line, your signal handler did not stop the program !");
								TCMFATALERROR("And so I will close it for you, you're welcome.");
							};

					signal( wantedSignal, signalFunc );
				}
			}
		}
	}
}
