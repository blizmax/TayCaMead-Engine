#include "LoggerView.h"
#include "MainWindow.h"

namespace TCM
{
	namespace Editor
	{
		LoggerView::LoggerView()
		{
			MainWindow::GetPlayEvent().Push( &LoggerView::OnPlay, this );
			Debug::Logger::GetEvent()->Push( &LoggerView::Push, this );
		}

		LoggerView::~LoggerView()
		{
			MainWindow::GetPlayEvent().Pop( &LoggerView::OnPlay, this );
			Debug::Logger::GetEvent()->Pop( &LoggerView::Push, this );
		}

		void LoggerView::OnPlay()
		{
			if ( m_clearOnPlay )
				Clear();
		}

		void LoggerView::Clear()
		{
			m_listLog.clear();
			OnClear();
		}

		void LoggerView::Push( Debug::LogData log )
		{
			m_listLog.push_back( log );
			OnPush( log );
		}

		void LoggerView::Refresh( Engine::Project* project, Engine::TCMScene* scene )
		{
			Clear();
		}
	}
}
