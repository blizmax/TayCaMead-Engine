#pragma once
#include <vector>
#include "TCMLogger/Logger.h"
#include "EditorComponent.h"

namespace TCM
{
	namespace Editor
	{
		class LoggerView abstract : public EditorComponent
		{
		public:
			LoggerView();
			virtual ~LoggerView();

			void OnPlay();
			void Clear();
			void Push( Debug::LogData log );
			void Refresh( Engine::Project* project, Engine::TCMScene* scene ) override;

		protected:
			virtual void OnClear()
			{
			};

			virtual void OnPush( Debug::LogData log )
			{
			}

			bool m_clearOnPlay = true;
			std::vector<Debug::LogData> m_listLog;
		};
	}
}
