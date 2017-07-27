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
			class StringInspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				StringInspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private slots:
				void OnTextChanged() const;
			};
		}
	}
}
