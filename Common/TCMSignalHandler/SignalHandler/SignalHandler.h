#pragma once
#include "TCMSignalHandler/SignalHandlerDefines.h"

namespace TCM
{
	namespace Engine
	{
		namespace Signal
		{
			/**
			 * \brief \n Overridable Handled Signals. \n
			 */
			typedef enum TCMSIGNALHANDLER_DLL ESignals : int
			{
				ABORT = 0,
				FLOATING_POINT_EXCEPTION,
				ILLEGAL_INSTRUCTION,
				INTERRUPTION,
				SEGMENTATION_FAULT,
				TERMINATION,
			} Signals;

			/**
			 * \brief \n Abstract Class for overridable signal handler. \n
			 * \remarks Cannot be instanciated. Needs to be implemented.
			 */
			class TCMSIGNALHANDLER_DLL SignalHandler
			{
			public:
				/**
				 * \brief \n Default Destructor. \n
				 */
				virtual ~SignalHandler() = default;

				/**
				 * \brief \n Function used to override a signal handler.\n
				 * \param signalToHandle _IN_	The Signal to handle.
				 * \param functionToExecuteIfSignalIsRaised _IN_	The function executed when the \p signalToHandle is raised.
				 */
				static void RegisterSignalHandler( Signals signalToHandle
				                                   , void (*functionToExecuteIfSignalIsRaised)() = []()
				                                   {
				                                   } )
				{
					GetInstance().CoreRegisterSignalHandler( signalToHandle
					                                         , functionToExecuteIfSignalIsRaised );
				};

			protected:
				SignalHandler() = default;

				/**
				 * \brief \n Core function to override handler for \p signalToHandle signal. Needs to be overriden. \n
				 * \param signalToHandle _IN_	The Signal to handle.
				 * \param functionToExecuteIfSignalIsRaised _IN_	The function executed when the \p signalToHandle is raised.
				 */
				virtual void CoreRegisterSignalHandler( Signals signalToHandle, void (*functionToExecuteIfSignalIsRaised)() ) = 0;

			private:
				/**
				 * \brief \n Class's Singleton. \n
				 * \return An instance of the class.
				 */
				static SignalHandler& GetInstance();
			};
		}
	}
}
