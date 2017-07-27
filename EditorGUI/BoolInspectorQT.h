#pragma once
#include <QObject>
#include "VarInspectorQT.h"
#include "VarHelper.h"

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class BoolInspectorQT : public VarInspectorQT
			{
				Q_OBJECT
			public:
				BoolInspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private slots:
				void OnStateChanged( bool state ) const;
			};
		}
	}
}
