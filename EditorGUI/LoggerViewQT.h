#pragma once

#include <QWidget>
#include "LoggerView.h"

QT_FORWARD_DECLARE_CLASS( QTableWidget )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class LoggerViewQT : public QWidget, public LoggerView
			{
				Q_OBJECT

			public:
				LoggerViewQT( QWidget* parent = nullptr );

			private slots:
				void ClearQT();

			private :
				void OnClear() override;
				void OnPush( Debug::LogData log ) override;

				QTableWidget* m_table;
			};
		}
	}
}
