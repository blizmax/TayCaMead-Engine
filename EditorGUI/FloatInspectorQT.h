#pragma once
#include "VarInspectorQT.h"
#include "VarHelper.h"

struct SerializedInterface;

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class FloatInspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				FloatInspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private slots:
				void OnValueChanged( const float& value );
			};
		}
	}
}
