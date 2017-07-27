#pragma once
#include "VarInspectorQT.h"
#include "VarHelper.h"

QT_FORWARD_DECLARE_CLASS( QValidator )

namespace TCM
{
	namespace Editor
	{
		namespace QT
		{
			class IntInspectorQT : public VarInspectorQT
			{
				Q_OBJECT

			public:
				IntInspectorQT( SerializedInterface* var );
				void UpdateFromValue() override;

			private slots:
				void OnValueChanged( const int value );

			private:
				QValidator* m_validator;
			};
		}
	}
}
